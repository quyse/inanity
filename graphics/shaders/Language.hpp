#ifndef ___INANITY_SHADERS_LANGUAGE_HPP___
#define ___INANITY_SHADERS_LANGUAGE_HPP___

#include "shaders.hpp"
#include "../../Exception.hpp"
#include <vector>
#include <sstream>

/* Файл содержит специальные классы для формирования текстов шейдеров.
*/

/*
#if !defined(___INANITY_SHADER_LANGUAGE_HLSL) // && !defined(___INANITY_SHADER_LANGUAGE_GLSL)
#error Current shader language should be defined.
#endif
*/

BEGIN_INANITY_SHADERS

class Composer;

/// Абстрактный класс данных.
/** Указывает источник данных */
template <typename DataType>
class Data : public Object
{
public:
	virtual void expression(std::ostream& stream) const = 0;
};

/// Класс константных данных.
template <typename DataType>
class ConstData : public Data<DataType>
{
private:
	DataType data;
public:
	ConstData(DataType data) : data(data) {}

	void expression(std::ostream& stream) const
	{
		stream << data;
	}
};

// прототип Variable
template <typename DataType>
class Variable;

// прототип CastData
template <typename DataTypeFrom, typename DataTypeTo>
class CastData;

/// Класс указателя на источник данных.
template <typename DataType>
class DataPtr
{
private:
	ptr<Data<DataType> > data;

public:
	DataPtr() : data(NEW(Variable<DataType>())) {}
	DataPtr(DataType data) : data(NEW(ConstData<DataType>(data))) {}
	DataPtr(ptr<Data<DataType> > data) : data(data) {}

	Data<DataType>* operator->() const
	{
		return data;
	}

	/// Оператор присваивания.
	void operator=(DataPtr<DataType> b)
	{
		std::ostream& stream = Composer::getCurrent()->statementStream();
		data->expression(stream);
		stream << " = (";
		b->expression(stream);
		stream << ");\n";
	}

	template <typename DataTypeTo>
	DataPtr<DataTypeTo> Cast() const
	{
		return NEW(CastData<DataType, DataTypeTo>(data));
	}
};

//******* имена типов данных
template <typename DataType>
const char* GetTypeName()
{
	THROW_PRIMARY_EXCEPTION("Unknown type name");
}
#define DECLARE_TYPE_NAME(t) \
	template <> const char* GetTypeName<t>() { return #t; }
DECLARE_TYPE_NAME(float);
DECLARE_TYPE_NAME(float2);
DECLARE_TYPE_NAME(float3);
DECLARE_TYPE_NAME(float4);
DECLARE_TYPE_NAME(float4x4);
#undef DECLARE_TYPE_NAME

/// Класс оператора приведения типа.
template <typename DataTypeFrom, typename DataTypeTo>
class CastData : public Data<DataTypeTo>
{
private:
	DataPtr<DataTypeFrom> data;
public:
	CastData(DataPtr<DataTypeFrom> data) : data(data) {}

	void expression(std::ostream& stream) const
	{
		stream << "(" << GetTypeName<DataTypeTo>() << ")(";
		data->expression(stream);
		stream << ")";
	}
};

/// Класс оператора плюс.
template <typename DataTypeA, typename DataTypeB>
class AddData : public Data<decltype(DataTypeA() + DataTypeB())>
{
private:
	DataPtr<DataTypeA> a;
	DataPtr<DataTypeB> b;
public:
	AddData(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) : a(a), b(b) {}

	void expression(std::ostream& stream) const
	{
		stream << "(";
		a->expression(stream);
		stream << ") + (";
		b->expression(stream);
		stream << ")";
	}
};

/// Класс оператора минус.
template <typename DataTypeA, typename DataTypeB>
class SubtractData : public Data<decltype(DataTypeA() - DataTypeB())>
{
private:
	DataPtr<DataTypeA> a;
	DataPtr<DataTypeB> b;
public:
	SubtractData(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) : a(a), b(b) {}

	void expression(std::ostream& stream) const
	{
		stream << "(";
		a->expression(stream);
		stream << ") - (";
		b->expression(stream);
		stream << ")";
	}
};

/// Класс оператора умножить.
template <typename DataTypeA, typename DataTypeB>
class MultiplyData : public Data<decltype(DataTypeA() * DataTypeB())>
{
private:
	DataPtr<DataTypeA> a;
	DataPtr<DataTypeB> b;
public:
	MultiplyData(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) : a(a), b(b) {}

	void expression(std::ostream& stream) const
	{
		stream << "(";
		a->expression(stream);
		stream << ") * (";
		b->expression(stream);
		stream << ")";
	}
};

/// Класс оператора разделить.
template <typename DataTypeA, typename DataTypeB>
class DivideData : public Data<decltype(DataTypeA() / DataTypeB())>
{
private:
	DataPtr<DataTypeA> a;
	DataPtr<DataTypeB> b;
public:
	DivideData(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) : a(a), b(b) {}

	void expression(std::ostream& stream) const
	{
		stream << "(";
		a->expression(stream);
		stream << ") / (";
		b->expression(stream);
		stream << ")";
	}
};

/// Класс вызова функции с одним аргументом.
template <typename ReturnType, typename Arg1Type>
class Call1Data : public Data<ReturnType>
{
private:
	const char* const functionName;
	DataPtr<Arg1Type> arg1;
public:
	Call1Data(const char* functionName, DataPtr<Arg1Type> arg1) : functionName(functionName), arg1(arg1) {}

	void expression(std::ostream& stream) const
	{
		stream << functionName << "((";
		arg1->expression(stream);
		stream << "))";
	}
};

/// Класс вызова функции с двумя аргументами.
template <typename ReturnType, typename Arg1Type, typename Arg2Type>
class Call2Data : public Data<ReturnType>
{
private:
	const char* const functionName;
	DataPtr<Arg1Type> arg1;
	DataPtr<Arg2Type> arg2;
public:
	Call2Data(const char* functionName, DataPtr<Arg1Type> arg1, DataPtr<Arg2Type> arg2) : functionName(functionName), arg1(arg1), arg2(arg2) {}

	void expression(std::ostream& stream) const
	{
		stream << functionName << "((";
		arg1->expression(stream);
		stream << "), (";
		arg2->expression(stream);
		stream << "))";
	}
};

/// Класс вызова функции с тремя аргументами.
template <typename ReturnType, typename Arg1Type, typename Arg2Type, typename Arg3Type>
class Call3Data : public Data<ReturnType>
{
private:
	const char* const functionName;
	DataPtr<Arg1Type> arg1;
	DataPtr<Arg2Type> arg2;
	DataPtr<Arg3Type> arg3;
public:
	Call3Data(const char* functionName, DataPtr<Arg1Type> arg1, DataPtr<Arg2Type> arg2, DataPtr<Arg3Type> arg3) : functionName(functionName), arg1(arg1), arg2(arg2), arg3(arg3) {}

	void expression(std::ostream& stream) const
	{
		stream << functionName << "((";
		arg1->expression(stream);
		stream << "), (";
		arg2->expression(stream);
		stream << "), (";
		arg3->expression(stream);
		stream << "))";
	}
};

//******* определения для упрощения жизни
/* По идее, они не нужны. Есть только переменные,
с данными работать не надо, они только внутри выражений.
typedef DataPtr<float> dfloat;
typedef DataPtr<float2> dfloat2;
typedef DataPtr<float3> dfloat3;
typedef DataPtr<float4> dfloat4;
typedef DataPtr<float4x4> dfloat4x4;*/

/// Класс переменной.
template <typename DataType>
class Variable : public Data<DataType>
{
private:
	/// Флаг, добавлено ли объявление переменной.
	/** Переменные объявляются лениво, для того, чтобы лишних
	не было, и чтобы сделать метод declare виртуальным. */
	mutable bool declared;
protected:
	/// Добавить объявление переменной в шейдер.
	virtual void declare(Composer* composer) const
	{
		std::ostream& stream = composer->declarationStream();
		stream << GetTypeName<DataType>() << " v" << this << ";\n";
	}
public:
	Variable() : declared(false) {}

	/// Убедиться, что переменная объявлена в текущем шейдере.
	void ensureDeclared() const
	{
		if(!declared)
		{
			declare(Composer::getCurrent());
			declared = true;
		}
	}
	
	void expression(std::ostream& stream) const
	{
		ensureDeclared();
		stream << "v" << this;
	}
};

/// Класс переменной, передающейся в/из/между шейдерами.
template <typename DataType, typename StructType>
class InterVariable : public Variable<DataType>
{
private:
	DataType StructType::* field;

public:
};

/// Класс входной переменной.
template <typename DataType, typename StructType>
class InputVariable : public Variable<DataType>
{
private:
	DataType StructType::* field;
protected:
	void declare(Composer* composer) const
	{
		composer->addInputDeclaration<DataType, StructType>(this, field);
	}
public:
	InputVariable(DataType StructType::* field) : field(field) {}
	void expression(std::ostream& stream) const
	{
		ensureDeclared();
		stream << "input.i" << this;
	}
};

template <typename DataType, typename StructType>
DataPtr<DataType> input(DataType StructType::* field)
{
	return NEW(InputVariable<DataType, StructType>(field));
}

/// Класс выходной переменной.
template <typename DataType, typename StructType>
class OutputVariable : public Variable<DataType>
{
private:
	DataType StructType::* field;
protected:
	void declare(Composer* composer) const
	{
		composer->addOutputDeclaration<DataType, StructType>(this, field);
	}
public:
	OutputVariable(DataType StructType::* field) : field(field) {}
	void expression(std::ostream& stream) const
	{
		ensureDeclared();
		stream << "output.o" << this;
	}
};

template <typename DataType, typename StructType>
DataPtr<DataType> output(DataType StructType::* field)
{
	return NEW(OutputVariable<DataType, StructType>(field));
}

//******* определения для упрощения жизни

typedef DataPtr<float> float_;
typedef DataPtr<float2> float2_;
typedef DataPtr<float3> float3_;
typedef DataPtr<float4> float4_;
typedef DataPtr<float4x4> float4x4_;

//******* операторы
/* Мы разрешаем выполнять операции с разными типами.
Предполагается, что в шейдерном языке операции выполняются так же,
как у нас в C++. Следовательно, неверные операции будут отброшены.
Если что, можно дописывать специализированные операторы -
плохие операторы будут отброшены в соответствии с принципом SFINAE. */
/// Плюс.
template <typename DataTypeA, typename DataTypeB>
auto operator+(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) -> DataPtr<decltype(DataTypeA() + DataTypeB())>
{
	return NEW(AddData<DataTypeA, DataTypeB>(a, b));
}
/// Минус.
template <typename DataTypeA, typename DataTypeB>
auto operator-(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) -> DataPtr<decltype(DataTypeA() - DataTypeB())>
{
	return NEW(SubtractData<DataTypeA, DataTypeB>(a, b));
}
/// Умножить.
template <typename DataTypeA, typename DataTypeB>
auto operator*(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) -> DataPtr<decltype(DataTypeA() * DataTypeB())>
{
	return NEW(MultiplyData<DataTypeA, DataTypeB>(a, b));
}
/// Разделить.
template <typename DataTypeA, typename DataTypeB>
auto operator/(DataPtr<DataTypeA> a, DataPtr<DataTypeB> b) -> DataPtr<decltype(DataTypeA() / DataTypeB())>
{
	return NEW(DivideData<DataTypeA, DataTypeB>(a, b));
}

/// Класс, выполняющий создание шейдера.
/** Одновременно не должно быть более одного такого объекта. */
class Composer
{
public:
	/// Текущий композер.
	static Composer* current;

private:
	/// Текст декларации переменных.
	std::ostringstream declarations;
	/// Текст выражений.
	std::ostringstream statements;
	/// Объявления входных переменных.
	/** Они составляют структуру входных данных. */
	std::vector<std::pair<size_t, std::string> > inputDeclarations;
	/// Объявления выходных переменных.
	/** Они составляют структуру выходных данных. */
	std::vector<std::pair<size_t, std::string> > outputDeclarations;

public:
	Composer()
	{
#ifdef _DEBUG
		if(current)
			THROW_PRIMARY_EXCEPTION("Using of more then one ShaderLanguage::Composer`s at a time is not allowed");
#endif
		current = this;
	}

	~Composer()
	{
		current = nullptr;
	}

	static Composer* getCurrent()
	{
		if(!current)
			THROW_PRIMARY_EXCEPTION("No current composer");
		return current;
	}

	template <typename DataType, typename StructType>
	void addInputDeclaration(const Variable<DataType>* variable, DataType StructType::* field)
	{
		std::ostringstream s;
		s << GetTypeName<DataType>() << " i" << variable << ";\n";
		inputDeclarations.push_back(std::make_pair(
			(char*)&(((StructType*)nullptr)->*field) - (char*)nullptr,
			s.str()));
	}

	template <typename DataType, typename StructType>
	void addOutputDeclaration(const Variable<DataType>* variable, DataType StructType::*field)
	{
		std::ostringstream s;
		s << GetTypeName<DataType>() << " o" << variable << ";\n";
		outputDeclarations.push_back(std::make_pair(
			(char*)&(((StructType*)nullptr)->*field) - (char*)nullptr,
			s.str()));
	}

	std::ostringstream& declarationStream()
	{
		return declarations;
	}

	std::ostringstream& statementStream()
	{
		return statements;
	}

	std::string finalize() const
	{
		std::ostringstream res;
		res << "struct Input {\n";
		for(size_t i = 0; i < inputDeclarations.size(); ++i)
			res << inputDeclarations[i].second;
		res << "};\n";
		res << "struct Output {\n";
		for(size_t i = 0; i < outputDeclarations.size(); ++i)
			res << outputDeclarations[i].second;
		res << "};\n";
		res << "Output F(Input input) {\nOutput output;\n" << declarations.str() << statements.str() << "return output;\n}\n";
		return res.str();
	}
};

END_INANITY_SHADERS

#endif
