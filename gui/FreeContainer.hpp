#ifndef ___INANITY_GUI_FREE_CONTAINER_HPP___
#define ___INANITY_GUI_FREE_CONTAINER_HPP___

#include "ContainerElement.hpp"
#include <vector>

BEGIN_INANITY_GUI

/// Free container element.
/** Elements are placed with explicit position and size. */
class FreeContainer : public ContainerElement
{
public:
	/// Place of the element.
	struct Place
	{
		enum HorizontalAnchor
		{
			anchorLeft,
			anchorRight,
			anchorHorizontalStretch
		};
		union
		{
			HorizontalAnchor anchor;
			struct
			{
				Distance left, width;
			} left;
			struct
			{
				Distance width, right;
			} right;
			struct
			{
				Distance left, right;
			} stretch;
		} horizontal;

		enum VerticalAnchor
		{
			anchorTop,
			anchorBottom,
			anchorVerticalStretch
		};
		union
		{
			VerticalAnchor anchor;
			struct
			{
				Distance top, height;
			} top;
			struct
			{
				Distance height, bottom;
			} bottom;
			struct
			{
				Distance top, bottom;
			} stretch;
		} vertical;
	};

private:
	std::vector<std::pair<Place, ptr<Element> > > elements;

	void DoLayout(size_t number);

public:
	void AddElement(const Place& place, ptr<Element> element);
	void RemoveElement(ptr<Element> element);

	//*** ContainerElement's methods.
	ptr<Element> TryGetElementByPosition(Position position) const;

	//*** Element's methods.
	void SetSize(Size size);
	bool IsPositionInto(Position position) const;
	void Draw(Visualizer* visualizer, Position offset);
};

END_INANITY_GUI

#endif
