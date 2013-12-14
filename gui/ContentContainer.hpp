#ifndef ___INANITY_GUI_CONTENT_CONTAINER_HPP___
#define ___INANITY_GUI_CONTENT_CONTAINER_HPP___

#include "ContainerElement.hpp"

BEGIN_INANITY_GUI

/// Container element with only one child.
/** Child is stretched over all client area. */
class ContentContainer : public ContainerElement
{
private:
	ptr<Element> contentElement;

	virtual void DoLayout();

protected:
	ContentContainer(ptr<Visualizer> visualizer);

public:
	void SetContent(ptr<Element> contentElement);

	//*** ContainerElement's methods.
	ptr<Element> TryGetElementByPosition(Position position) const;

	//*** Element's methods.
	void SetSize(Size size);
};

END_INANITY_GUI

#endif
