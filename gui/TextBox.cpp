#include "TextBox.hpp"
#include "Visualizer.hpp"
#include "Visuals.hpp"
#include "../deps/utf8.h"
#include <iterator>

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
	case Keys::ShiftL:
	case Keys::ShiftR:
		shiftPressed = true;
		break;
	case Keys::Left:
		if(selectionBegin > 0)
			selectionBegin--;
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case Keys::Right:
		if(selectionBegin < text.length())
			selectionBegin++;
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case Keys::Home:
		selectionBegin = 0;
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case Keys::End:
		selectionBegin = text.length();
		if(!shiftPressed)
			selectionEnd = selectionBegin;
		break;
	case Keys::BackSpace:
		if(selectionEnd != selectionBegin)
		{
			text.erase(text.begin() + std::min(selectionBegin, selectionEnd), text.begin() + std::max(selectionBegin, selectionEnd));
			selectionBegin = std::min(selectionBegin, selectionEnd);
		}
		else if(selectionBegin > 0)
			text.erase(text.begin() + (--selectionBegin));
		selectionEnd = selectionBegin;
		break;
	case Keys::Delete:
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
	case Keys::ShiftL:
	case Keys::ShiftR:
		shiftPressed = false;
		break;
	}
}

void TextBox::EventKeyPress(wchar_t key)
{
	if(key == Keys::BackSpace)
		return;
	text.erase(text.begin() + std::min(selectionBegin, selectionEnd), text.begin() + std::max(selectionBegin, selectionEnd));
	selectionBegin = std::min(selectionBegin, selectionEnd);
	size_t initialLength = text.length();
	utf8::unchecked::append(key, std::inserter(text, text.begin() + selectionBegin));
	selectionBegin += text.length() - initialLength;
	selectionEnd = selectionBegin;
}

END_INANITY_GUI
