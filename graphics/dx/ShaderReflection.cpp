#include "ShaderReflection.hpp"
#include "../GeometryFormat.hpp"
#include "ShaderLayout.hpp"
#include "System.hpp"
#include "../../File.hpp"
#include "../../Strings.hpp"
#include "../../Exception.hpp"

ShaderReflection::ShaderReflection(ptr<File> shader) : shader(shader)
{
	try
	{
		//получить интерфейс рефлексии
		ID3D11ShaderReflection* reflection;
		if(FAILED(D3DReflect(shader->GetData(), shader->GetSize(), IID_ID3D11ShaderReflection, (void**)&reflection)))
			THROW_PRIMARY_EXCEPTION("Can't get shader reflection interface");
		this->reflection = reflection;

		//получить информацию о шейдере
		D3D11_SHADER_DESC desc;
		if(FAILED(reflection->GetDesc(&desc)))
			THROW_PRIMARY_EXCEPTION("Can't get shader desc");

		//запомнить некоторую информацию
		inputParametersCount = desc.InputParameters;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create shader reflection", exception);
	}
}

ptr<ShaderLayout> ShaderReflection::CreateShaderLayout(System* system, GeometryFormat* geometryFormat)
{
	try
	{
		//создать массив элементов
		std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
		elements.reserve(inputParametersCount);
		//цикл по входным параметрам
		for(unsigned i = 0; i < inputParametersCount; ++i)
			try
			{
				//получить входной параметр
				D3D11_SIGNATURE_PARAMETER_DESC desc;
				if(FAILED(reflection->GetInputParameterDesc(i, &desc)))
					THROW_PRIMARY_EXCEPTION("Can't get input parameter desc");

				//если семантика является системной
				if(desc.SystemValueType != D3D10_NAME_UNDEFINED)
					//пропустить ее, потому что данные тогда берутся не из вершины
					continue;
			
				//найти соответствующую семантику
				const GeometrySemantic& semantic = geometryFormat->GetSemantic(desc.SemanticName + Strings::ToString(desc.SemanticIndex));

				//сформировать элемент
				D3D11_INPUT_ELEMENT_DESC element;
				element.SemanticName = desc.SemanticName;
				element.SemanticIndex = desc.SemanticIndex;
				switch(semantic.type)
				{
				case GeometrySemantic::typeFloat:
					switch(semantic.count)
					{
					case 1:
						element.Format = DXGI_FORMAT_R32_FLOAT;
						break;
					case 2:
						element.Format = DXGI_FORMAT_R32G32_FLOAT;
						break;
					case 3:
						element.Format = DXGI_FORMAT_R32G32B32_FLOAT;
						break;
					case 4:
						element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
						break;
					default:
						THROW_PRIMARY_EXCEPTION("Invalid element count");	
					}
					break;
				case GeometrySemantic::typeInt:
					switch(semantic.count)
					{
					case 1:
						element.Format = DXGI_FORMAT_R32_SINT;
						break;
					case 2:
						element.Format = DXGI_FORMAT_R32G32_SINT;
						break;
					case 3:
						element.Format = DXGI_FORMAT_R32G32B32_SINT;
						break;
					case 4:
						element.Format = DXGI_FORMAT_R32G32B32A32_SINT;
						break;
					default:
						THROW_PRIMARY_EXCEPTION("Invalid element count");	
					}
					break;
				case GeometrySemantic::typeUint:
					switch(semantic.count)
					{
					case 1:
						element.Format = DXGI_FORMAT_R32_UINT;
						break;
					case 2:
						element.Format = DXGI_FORMAT_R32G32_UINT;
						break;
					case 3:
						element.Format = DXGI_FORMAT_R32G32B32_UINT;
						break;
					case 4:
						element.Format = DXGI_FORMAT_R32G32B32A32_UINT;
						break;
					default:
						THROW_PRIMARY_EXCEPTION("Invalid element count");	
					}
					break;
				default:
					THROW_PRIMARY_EXCEPTION("Invalid element type");
				}
				element.InputSlot = 0;
				element.AlignedByteOffset = semantic.offset;
				element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				element.InstanceDataStepRate = 0;
				elements.push_back(element);
			}
			catch(Exception* exception)
			{
				THROW_SECONDARY_EXCEPTION("Can't create semantic " + Strings::ToString(i), exception);
			}

		//создать layout
		ID3D11InputLayout* layout;
		HRESULT hr;
		if(FAILED(hr = system->GetDevice()->CreateInputLayout(&*elements.begin(), (UINT)elements.size(), shader->GetData(), shader->GetSize(), &layout)))
			THROW_PRIMARY_EXCEPTION("Can't create input layout");

		return NEW(ShaderLayout(layout));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create shader layout", exception);
	}
}
