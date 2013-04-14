#include "LayoutBinding.hpp"
#include "Layout.hpp"
#include "../Exception.hpp"
#include <unordered_map>
#include <algorithm>
#include <cstring>

BEGIN_INANITY_GRAPHICS

LayoutBinding::Element::Element(int sourceOffset, int destOffset, int size)
: sourceOffset(sourceOffset), destOffset(destOffset), size(size) {}

bool operator<(const LayoutBinding::Element& a, const LayoutBinding::Element& b)
{
	return a.destOffset < b.destOffset;
}

LayoutBinding::LayoutBinding(const std::vector<Element>& e)
: elements(e)
{
	// упорядочим элементы
	std::sort(elements.begin(), elements.end());
	// слияние элементов, стоящих подряд
	int elementsCount = (int)elements.size();
	int k = 0;
	for(int i = 0; i < elementsCount; )
	{
		int j;
		for(j = i + 1;
			j < elementsCount
				// если элемент идёт строго за предыдущим элементом
				&& elements[j].sourceOffset == elements[j - 1].sourceOffset + elements[j - 1].size
				&& elements[j].destOffset == elements[j - 1].destOffset + elements[j - 1].size
				; ++j);
		// j указывает на первый неправильный элемент
		elements[k].sourceOffset = elements[i].sourceOffset;
		elements[k].destOffset = elements[i].destOffset;
		elements[k].size = elements[j - 1].sourceOffset - elements[i].sourceOffset + elements[j - 1].size;
		++k;
		i = j;
	}
	// сократить список
	elements.resize(k);
}

const std::vector<LayoutBinding::Element>& LayoutBinding::GetElements() const
{
	return elements;
}

void LayoutBinding::Apply(const void* sourceData, void* destData)
{
	int elementsCount = (int)elements.size();
	for(int i = 0; i < elementsCount; ++i)
		memcpy((char*)destData + elements[i].destOffset, (const char*)sourceData + elements[i].sourceOffset, elements[i].size);
}

ptr<LayoutBinding> LayoutBinding::Create(Layout* sourceLayout, Layout* destLayout)
{
	try
	{
		// отношение семантика-номер элемента для исходной разметки
		std::unordered_map<int, int> sourceSemantics;
		
		const std::vector<Layout::Element>& sourceElements = sourceLayout->GetElements();
		int sourceElementsCount = sourceElements.size();
		for(int i = 0; i < sourceElementsCount; ++i)
			sourceSemantics[sourceElements[i].semantic] = i;

		const std::vector<Layout::Element>& destElements = destLayout->GetElements();
		int destElementsCount = destElements.size();

		std::vector<Element> elements;
		for(int i = 0; i < destElementsCount; ++i)
		{
			std::unordered_map<int, int>::const_iterator j = sourceSemantics.find(destElements[i].semantic);
			if(j == sourceSemantics.end())
				THROW_PRIMARY_EXCEPTION("Source layout miss semantic");
			const Layout::Element& sourceElement = sourceElements[j->second];
			const Layout::Element& destElement = destElements[i];
			if(sourceElement.dataType != destElement.dataType)
				THROW_PRIMARY_EXCEPTION("Types not conform");
			elements.push_back(Element(sourceElement.offset, destElement.offset, GetDataTypeSize(sourceElement.dataType)));
		}

		return NEW(LayoutBinding(elements));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create layout binding", exception);
	}
}

END_INANITY_GRAPHICS
