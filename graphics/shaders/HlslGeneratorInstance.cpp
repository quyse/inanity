#include "HlslGeneratorInstance.hpp"
#include "StatementObject.hpp"
#include "StatementObjects.hpp"
#include "ExpressionObject.hpp"
#include "ExpressionObjects.hpp"
#include "../../Exception.hpp"
#include <algorithm>

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

void HlslGeneratorInstance::PrintVariables(Shader::Variables variables, const char* variableNamePrefix)
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

		// семантика
		static const char* semantics[Semantics::_Count] = { 0, "SV_InstanceID", "SV_Position", "SV_Target", "SV_Depth" };
		const char* semantic = semantics[variable.semantic];
		if(semantic)
			hlsl << " : " << semantic;

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

		// конец переменной
		hlsl << ";\n";
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
		hlsl << 'i' << fast_cast<InExpressionObject*>(object)->GetOffset();
		break;
	case ExpressionObject::typeOut:
		hlsl << 'o' << fast_cast<OutExpressionObject*>(object)->GetOffset();
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
	case ExpressionObject::typeNegate:
		hlsl << "(-";
		PrintExpression(fast_cast<NegateExpressionObject*>(object)->GetInner());
		hlsl << ')';
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
			hlsl << call1Object->GetName() << '(';
			PrintExpression(call1Object->GetArg1());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeCall2:
		{
			Call2ExpressionObject* call2Object = fast_cast<Call2ExpressionObject*>(object);
			hlsl << call2Object->GetName() << '(';
			PrintExpression(call2Object->GetArg1());
			hlsl << ", ";
			PrintExpression(call2Object->GetArg2());
			hlsl << ')';
		}
		break;
	case ExpressionObject::typeCall3:
		{
			Call3ExpressionObject* call3Object = fast_cast<Call3ExpressionObject*>(object);
			hlsl << call3Object->GetName() << '(';
			PrintExpression(call3Object->GetArg1());
			hlsl << ", ";
			PrintExpression(call3Object->GetArg2());
			hlsl << ", ";
			PrintExpression(call3Object->GetArg3());
			hlsl << ')';
		}
		break;
	default:
		THROW_PRIMARY_EXCEPTION("Invalid expression type");
	}
}

std::string HlslGeneratorInstance::Generate()
{
	// установить количество табуляций для вызываемых функций
	tabsCount = 1;

	// структура входных данных
	hlsl << "struct Input\n{\n";
	PrintVariables(shader.GetInputVariables(), "i");
	hlsl << "};\n";

	// структура выходных данных
	hlsl << "struct Output\n{\n";
	PrintVariables(shader.GetOutputVariables(), "o");
	hlsl << "};\n";

	// структуры константных буферов
	const std::vector<Shader::Variables>& uniformsVariables = shader.GetUniformsVariables();
	for(size_t i = 0; i < uniformsVariables.size(); ++i)
	{
		const Shader::Variables& variables = uniformsVariables[i];

		hlsl << "cbuffer : register(b" << i << ")\n{\n";
		char bufferPrefix[20];
		sprintf(bufferPrefix, "u%u_", (unsigned int)i);
		PrintVariables(variables, bufferPrefix);
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
			hlsl << "Sampler s" << i << " : register(s" << i << ");\n";
		}

	// функция шейдера
	hlsl << "Output Main(Input input)\n{\n";
	// код шейдера
	PrintStatement(shader.GetCode());
	// возврат значения и конец
	hlsl << "\treturn output;\n}\n";

	return hlsl.str();
}

END_INANITY_SHADERS
