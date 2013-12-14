#include "TextBox.hpp"
#include "Visualizer.hpp"
#include "Visuals.hpp"
// FIXME: зависимость от винды для макросов клавиш VK_*
#include "../platform/windows.hpp"

BEGIN_INANITY_GUI

TextBox::TextBox(ptr<Visualizer> visualizer)
: Element(visualizer), selectionBegin(0), selectionEnd(0), shiftPressed(false)
{}

const String& TextBox::GetText() const
{
	return text;
}

void TextBox::SetText(const String& text)
{
	this->text = text;
}

void TextBox::Draw(Canvas* canvas, Position offset)
{
	offset += position;

	//нарисовать рамку
	visualizer->DrawVisualElement(canvas, moused || focused ? VisualElements::TextBoxHighlighted : VisualElements::TextBox, offset.x, offset.y, offset.x + size.x, offset.y + size.y);

	//сдвинуть все на величину сдвига текста, потому что все далее будет рисоваться с этим сдвигом
//	offset.x += visualizer->GetVisualMetric(VisualMetrics::TextBoxTextX);

	//нарисовать выделение
	if(selectionBegin != selectionEnd)
	{
		float selectionBeginX = visualizer->GetTextSize(VisualFonts::UserText, text.substr(0, std::min(selectionBegin, selectionEnd))).x;
		float selectionEndX = visualizer->GetTextSize(VisualFonts::UserText, text.substr(0, std::max(selectionBegin, selectionEnd))).x;
		visualizer->DrawVisualElement(canvas, VisualElements::Selection, offset.x + selectionBeginX, offset.y, offset.x + selectionEndX, offset.y + size.y);
	}

	//напечатать текст
	visualizer->DrawRectText(canvas, VisualFonts::UserText, text, offset.x, offset.y, offset.x + size.x, offset.y + size.y);

	//нарисовать курсор
	if(focused)
	{
		float caretX = visualizer->GetTextSize(VisualFonts::UserText, text.substr(0, selectionBegin)).x;
		visualizer->DrawVisualElement(canvas, VisualElements::Caret, offset.x + caretX, offset.y, offset.x + caretX, offset.y + size.y);
	}
}

void TextBox::EventMouseDown(Position position, MouseEvent::Button button)
{
	Element::EventMouseDown(position, button);

	SetFocus();
}

void TextBox::EventKeyDown(Key key)
{
	switch(key)
	{
	case VK_SHIFT:
		shiftPressed = true;
		break;
	case VK_LEFT:
		if(selectionBegin > 0)
			selectionBegin--;
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case VK_RIGHT:
		if(selectionBegin < text.length())
			selectionBegin++;
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case VK_HOME:
		selectionBegin = 0;
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case VK_END:
		selectionBegin = text.length();
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case VK_BACK:
		if(selectionEnd != selectionBegin)
		{
			text.erase(text.begin() + std::min(selectionBegin, selectionEnd), text.begin() + std::max(selectionBegin, selectionEnd));
			selectionBegin = std::min(selectionBegin, selectionEnd);
		}
		else if(selectionBegin > 0)
			text.erase(text.begin() + (--selectionBegin));
		selectionEnd = selectionBegin;
		break;
	case VK_DELETE:
		if(selectionEnd != selectionBegin)
		{
			text.erase(text.begin() + std::min(selectionBegin, selectionEnd), text.begin() + std::max(selectionBegin, selectionEnd));
			selectionBegin = std::min(selectionBegin, selectionEnd);
		}
		else if(selectionBegin < text.length())
			text.erase(text.begin() + selectionBegin);
		selectionEnd = selectionBegin;
		break;
	}
}

void TextBox::EventKeyUp(Key key)
{
	switch(key)
	{
	case VK_SHIFT:
		shiftPressed = false;
		break;
	}
}

void TextBox::EventKeyPress(wchar_t key)
{
	if(key == VK_BACK)
		return;
	text.erase(text.begin() + std::min(selectionBegin, selectionEnd), text.begin() + std::max(selectionBegin, selectionEnd));
	selectionBegin = std::min(selectionBegin, selectionEnd);
	text.insert(text.begin() + (selectionBegin++), key);
	selectionEnd = selectionBegin;
}

END_INANITY_GUI
