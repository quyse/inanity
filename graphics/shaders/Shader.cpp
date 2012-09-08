#include "Shader.hpp"
#include "StatementObject.hpp"
#include "../../Exception.hpp"

Shader::Variable::Variable(DataType dataType, int offset)
: dataType(dataType), offset(offset), semantic(Semantics::None) {}

Shader::Shader() : samplerSlotsMask(0) {}

void Shader::RegisterVariable(Variables& variables, DataType dataType, int offset)
{
	// попробовать найти эту переменную, может, она уже есть
	for(size_t i = 0; i < variables.size(); ++i)
		if(variables[i].offset == offset)
		{
			if(variables[i].dataType != dataType)
				THROW_PRIMARY_EXCEPTION("Data type is not match for already registered variable");
			return;
		}

	// переменная не найдена, добавить её
	variables.push_back(Variable(dataType, offset));
}

void Shader::SetCode(Statement code)
{
	this->code = code;
}

Statement Shader::GetCode() const
{
	return code;
}

const Shader::Variables& Shader::GetInputVariables() const
{
	return inputVariables;
}

const Shader::Variables& Shader::GetOutputVariables() const
{
	return outputVariables;
}

const Shader::Variables& Shader::GetTempVariables() const
{
	return tempVariables;
}

const std::vector<Shader::Variables>& Shader::GetUniformsVariables() const
{
	return uniformsVariables;
}

int Shader::GetSamplerSlotsMask() const
{
	return samplerSlotsMask;
}
