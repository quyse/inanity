#include "HlslGeneratorInstance.hpp"
#include "StatementObject.hpp"
#include "StatementObjects.hpp"
#include "ExpressionObject.hpp"
#include "ExpressionObjects.hpp"
#include "../../Exception.hpp"
#include <algorithm>
#include <iomanip>

BEGIN_INANITY_SHADERS

HlslGeneratorInstance::HlslGeneratorInstance(const Shader& shader)
: shader(shader), tabsCount(0) {}

void HlslGeneratorInstance::PrintTabs()
{
	for(int i = 0; i < tabsCount; ++i)
		hlsl << '\t';
}

void HlslGeneratorInstance::PrintDataType(DataType dataType)
{
	static const char* const names[DataTypes::_Count] = { "float", "float2", "float3", "float4", "float4x4", "uint", "uint2", "uint3", "uint4" };
	hlsl << names[dataType];
}

void HlslGeneratorInstance::PrintVariables(Shader::Variables variables, const char* variableNamePrefix, bool printPackOffsets)
{
	try
	{
		// сортировщик переменных по смещениям
		struct Sorter
		{
			bool operator()(const Shader::Variable& a, const Shader::Variable& b) const
			{
				return a.offset < b.offset;
			}
		};
		// отсортировать переменные по смещениям
		std::sort(variables.begin(), variables.end(), Sorter());

		for(size_t i = 0; i < variables.size(); ++i)
			try
			{
				const Shader::Variable& variable = variables[i];

				// переменная должна лежать на границе float'а, как минимум
				if(variable.offset % sizeof(float))
					THROW_PRIMARY_EXCEPTION("Wrong variable offset: should be on 4-byte boundary");

				// печатаем определение переменной

				// тип переменной
				PrintTabs();
				PrintDataType(variable.dataType);
				// имя переменной
				hlsl << ' ' << variableNamePrefix << variable.offset;

				//** семантика

				const char* semanticStr = 0;
				char semanticBuffer[16];
				// автоматическая семантика
				if(variable.semantic == Semantics::None)
				{
					// получается по смещению элемента
					strcpy(semanticBuffer, "AUTO");
					int o = variable.offset;
					int i;
					for(i = 4; o; ++i)
					{
						semanticBuffer[i] = 'a' + o % 26;
						o /= 26;
					}
					semanticBuffer[i] = 0;
					semanticStr = semanticBuffer;
				}
				// специальная семантика
				else if(variable.semantic >= Semantics::_SpecialBegin && variable.semantic < Semantics::_SpecialEnd)
				{
					if(variable.semantic == Semantics::Instance)
						semanticStr = "SV_InstanceID";
					else if(variable.semantic == Semantics::VertexPosition)
						semanticStr = "SV_Position";
					else if(variable.semantic >= Semantics::_TargetColorBegin && variable.semantic < Semantics::_TargetColorEnd)
					{
						sprintf(semanticBuffer, "SV_Target%d", variable.semantic - Semantics::TargetColor0);
						semanticStr = semanticBuffer;
					}
					else if(variable.semantic >= Semantics::_TargetDepthBegin && variable.semantic < Semantics::_TargetDepthEnd)
					{
						sprintf(semanticBuffer, "SV_Depth%d", variable.semantic - Semantics::TargetDepth0);
						semanticStr = semanticBuffer;
					}
				}
				// пользовательская семантика
				else if(variable.semantic >= Semantics::_CustomBegin && variable.semantic < Semantics::_CustomEnd)
				{
					strcpy(semanticBuffer, "CUSTOM");
					int o = variable.semantic - Semantics::Custom0;
					int i;
					for(i = 6; o; ++i)
					{
						semanticBuffer[i] = 'a' + o % 26;
						o /= 26;
					}
					semanticBuffer[i] = 0;
					semanticStr = semanticBuffer;
				}

				// если семантика осталась не указанной
				if(!semanticStr)
					THROW_PRIMARY_EXCEPTION("Can't determine semantic");

				hlsl << " : " << semanticStr;

				if(printPackOffsets)
				{
					// регистр и положение в нём переменной
					hlsl << " : packoffset(c" << (variable.offset / sizeof(float4));
					// если переменная не начинается ровно на границе регистра, нужно дописать ещё компоненты регистра
					int registerOffset = variable.offset % sizeof(float4);
					if(registerOffset)
					{
						// получить размер данных
						int variableSize = GetDataTypeSize(variable.dataType);
						// переменная не должна пересекать границу регистра
						if(registerOffset + variableSize > sizeof(float4))
							THROW_PRIMARY_EXCEPTION("Variable should not intersect a register boundary");
						// выложить столько буков, сколько нужно
						registerOffset /= sizeof(float);
						int endRegisterOffset = registerOffset + variableSize / sizeof(float);
						hlsl << '.';
						for(int j = registerOffset; j < endRegisterOffset; ++j)
							hlsl << "xyzw"[j];
					}
					// конец упаковки
					hlsl << ")";
				}

				// конец переменной
				hlsl << ";\n";
			}
			catch(Exception* exception)
			{
				std::ostringstream s;
				s << "Can't print variable " << i;
				THROW_SECONDARY_EXCEPTION(s.str(), exception);
			}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION(String("Can't print variables (prefix ") + variableNamePrefix + ")", exception);
	}
}

void HlslGeneratorInstance::PrintStatement(Statement statement)
{
	StatementObject* object = statement.GetObject();

	switch(object->GetType())
	{
	case StatementObject::typeExpression:
		PrintTabs();
		PrintExpression(static_cast<ExpressionStatementObject*>(object)->GetExpression());
		hlsl << ";\n";
		break;
	case StatementObject::typeComma:
		{
			CommaStatementObject* commaObject = static_cast<CommaStatementObject*>(object);
			PrintStatement(commaObject->GetLeft());
			PrintStatement(commaObject->GetRight());
		}
		break;
	case StatementObject::typeIf:
		{
			IfStatementObject* ifObject = static_cast<IfStatementObject*>(object);
			// печатаем условие
			PrintTabs();
			hlsl << "\tif(";
			PrintExpression(ifObject->GetCondition());
			hlsl << ")\n";
			// печатаем блок после if
			PrintTabs();
			hlsl << "{\n";
			++tabsCount;
			PrintStatement(ifObject->GetTrueBody());
			--tabsCount;
			PrintTabs();
			hlsl << "}\n";
			// если есть блок else, напечатать его
			Statement falseBody = ifObject->GetFalseBody();
			if(falseBody.GetObject())
			{
				PrintTabs();
				hlsl << "else\n";
				PrintTabs();
				hlsl << "{\n";
				++tabsCount;
				PrintStatement(falseBody);
				--tabsCount;
				PrintTabs();
				hlsl << "}\n";
			}
		}
		break;
	default:
		THROW_PRIMARY_EXCEPTION("Invalid statement type");
	}
}

void HlslGeneratorInstance::PrintExpression(Expression expression)
{
	ExpressionObject* object = expression.GetObject();

	switch(object->GetType())
	{
	case ExpressionObject::typeIn:
		hlsl << "input.i" << fast_cast<InExpressionObject*>(object)->GetOffset();
		break;
	case ExpressionObject::typeOut:
		hlsl << "output.o" << fast_cast<OutExpressionObject*>(object)->GetOffset();
		break;
	case ExpressionObject::typeUniform:
		{
			UniformExpressionObject* uniformObject = fast_cast<UniformExpressionObject*>(object);
			hlsl << 'u' << uniformObject->GetBuffer() << '_' << uniformObject->GetOffset();
		}
		break;
	case ExpressionObject::typeSampler:
		// в живом виде семплер не должен попадаться
		THROW_PRIMARY_EXCEPTION("Invalid use of sampler");
	case ExpressionObject::typeTemp:
		hlsl << 'loc' << fast_cast<TempExpressionObject*>(object)->GetOffset();
		break;
	case ExpressionObject::typeConstFloat:
		hlsl << std::fixed << std::setprecision(10) << fast_cast<ConstFloatExpressionObject*>(object)->GetValue() << 'f';
		break;
	case ExpressionObject::typeConstInt:
		hlsl << fast_cast<ConstIntExpressionObject*>(object)->GetValue();
		break;
	case ExpressionObject::typeNegate:
		hlsl << "(-";
		PrintExpression(fast_cast<NegateExpressionObject*>(object)->GetInner());
		hlsl << ')';
		break;
	case ExpressionObject::typeSwizzle:
		{
			SwizzleExpressionObject* swizzleObject = fast_cast<SwizzleExpressionObject*>(object);
			hlsl << '(';
			PrintExpression(swizzleObject->GetInner());
			hlsl << '.' << swizzleObject->GetComponents() << ')';
		}
		break;
	case ExpressionObject::typeAssign:
		{
			AssignExpressionObject* assignObject = fast_cast<AssignExpressionObject*>(object);
			hlsl << '(';
			PrintExpression(assignObject->GetLeft());
			hlsl << " = ";
			PrintExpression(assignObject->GetRight());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeSubscript:
		{
			SubscriptExpressionObject* subscriptObject = fast_cast<SubscriptExpressionObject*>(object);
			PrintExpression(subscriptObject->GetLeft());
			hlsl << '[';
			PrintExpression(subscriptObject->GetRight());
			hlsl << ']';
		}
		break;
	case ExpressionObject::typeAdd:
		{
			AddExpressionObject* addObject = fast_cast<AddExpressionObject*>(object);
			hlsl << '(';
			PrintExpression(addObject->GetLeft());
			hlsl << " + ";
			PrintExpression(addObject->GetRight());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeSubtract:
		{
			SubtractExpressionObject* subtractObject = fast_cast<SubtractExpressionObject*>(object);
			hlsl << '(';
			PrintExpression(subtractObject->GetLeft());
			hlsl << " - ";
			PrintExpression(subtractObject->GetRight());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeMultiply:
		{
			MultiplyExpressionObject* multiplyObject = fast_cast<MultiplyExpressionObject*>(object);
			hlsl << '(';
			PrintExpression(multiplyObject->GetLeft());
			hlsl << " * ";
			PrintExpression(multiplyObject->GetRight());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeDivide:
		{
			DivideExpressionObject* divideObject = fast_cast<DivideExpressionObject*>(object);
			hlsl << '(';
			PrintExpression(divideObject->GetLeft());
			hlsl << " / ";
			PrintExpression(divideObject->GetRight());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeCall1:
		{
			Call1ExpressionObject* call1Object = fast_cast<Call1ExpressionObject*>(object);

			const char* functionName = GetRealFunctionName(call1Object->GetName());

			hlsl << functionName << '(';
			PrintExpression(call1Object->GetArg1());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeCall2:
		{
			Call2ExpressionObject* call2Object = fast_cast<Call2ExpressionObject*>(object);

			const char* functionName = GetRealFunctionName(call2Object->GetName());

			// особая функция sample
			if(strcmp(functionName, "sample") == 0)
			{
				// первый параметр должен быть семплером
				ExpressionObject* a1 = call2Object->GetArg1().GetObject();
				if(a1->GetType() != ExpressionObject::typeSampler)
					THROW_PRIMARY_EXCEPTION("First parameter of sample function should be a sampler");
				int samplerSlot = fast_cast<SamplerExpressionObject*>(a1)->GetSlot();
				hlsl << 't' << samplerSlot << ".Sample(s" << samplerSlot << ", ";
				PrintExpression(call2Object->GetArg2());
				hlsl << ')';
			}
			// остальные функции
			else
			{
				hlsl << functionName << '(';
				PrintExpression(call2Object->GetArg1());
				hlsl << ", ";
				PrintExpression(call2Object->GetArg2());
				hlsl << ')';
			}
		}
		break;
	case ExpressionObject::typeCall3:
		{
			Call3ExpressionObject* call3Object = fast_cast<Call3ExpressionObject*>(object);

			const char* functionName = GetRealFunctionName(call3Object->GetName());

			hlsl << functionName << '(';
			PrintExpression(call3Object->GetArg1());
			hlsl << ", ";
			PrintExpression(call3Object->GetArg2());
			hlsl << ", ";
			PrintExpression(call3Object->GetArg3());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeCall4:
		{
			Call4ExpressionObject* call4Object = fast_cast<Call4ExpressionObject*>(object);

			const char* functionName = GetRealFunctionName(call4Object->GetName());

			hlsl << functionName << '(';
			PrintExpression(call4Object->GetArg1());
			hlsl << ", ";
			PrintExpression(call4Object->GetArg2());
			hlsl << ", ";
			PrintExpression(call4Object->GetArg3());
			hlsl << ", ";
			PrintExpression(call4Object->GetArg4());
			hlsl << ')';
		}
		break;
	default:
		THROW_PRIMARY_EXCEPTION("Invalid expression type");
	}
}

const char* HlslGeneratorInstance::GetRealFunctionName(const char* functionName)
{
	static const char* const names[][2] =
	{
		{ "float4_xyz_w", "float4" },
		{ "float4_xy_z_w", "float4" },
		{ "cast4x4to3x3", "(float3x3)" }
	};
	for(int i = 0; i < sizeof(names) / sizeof(names[0]); ++i)
		if(strcmp(names[i][0], functionName) == 0)
			return names[i][1];
	return functionName;
}

std::string HlslGeneratorInstance::Generate()
{
	// установить количество табуляций для вызываемых функций
	tabsCount = 1;

	// структура входных данных
	hlsl << "struct Input\n{\n";
	PrintVariables(shader.GetInputVariables(), "i", false);
	hlsl << "};\n";

	// структура выходных данных
	hlsl << "struct Output\n{\n";
	PrintVariables(shader.GetOutputVariables(), "o", false);
	hlsl << "};\n";

	// структуры константных буферов
	const std::vector<Shader::Variables>& uniformsVariables = shader.GetUniformsVariables();
	for(size_t i = 0; i < uniformsVariables.size(); ++i)
	{
		const Shader::Variables& variables = uniformsVariables[i];
		if(variables.empty())
			continue;

		hlsl << "cbuffer CBuffer" << i << " : register(b" << i << ")\n{\n";
		char bufferPrefix[20];
		sprintf(bufferPrefix, "u%u_", (unsigned int)i);
		PrintVariables(variables, bufferPrefix, true);
		hlsl << "};\n";
	}

	// семплеры
	int samplerSlotMask = shader.GetSamplerSlotsMask();
	for(int i = 0; i < 32; ++i)
		if(samplerSlotMask & (1 << i))
		{
			// текстура
			hlsl << "Texture t" << i << " : register(t" << i << ");\n";
			// семплер
			hlsl << "SamplerState s" << i << " : register(s" << i << ");\n";
		}

	// функция шейдера
	hlsl << "Output Main(Input input)\n{\n\tOutput output;\n";
	// код шейдера
	PrintStatement(shader.GetCode());
	// возврат значения и конец
	hlsl << "\treturn output;\n}\n";

	return hlsl.str();
}

END_INANITY_SHADERS
