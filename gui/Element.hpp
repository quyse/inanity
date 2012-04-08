#ifndef ___INANITY_GUI_ELEMENT_HPP___
#define ___INANITY_GUI_ELEMENT_HPP___

#include "gui.hpp"
#include "types.hpp"

BEGIN_INANITY_GUI

class Visualizer;
class ContainerElement;
/// Класс элемента пользовательского интерфейса.
/** Класс, служащий базовым для всех элементов
интерфейса. Предоставляет заготовки для всех событий
элемента GUI.
*/
class Element : public Object
{
protected:
	/// Визуализатор.
	ptr<Visualizer> visualizer;
	/// Родительский элемент.
	ContainerElement* parent;
	/// Положение элемента (относительно родительского элемента).
	Position position;
	/// Размеры элемента.
	Size size;
	/// Находится ли элемент в фокусе.
	bool focused;
	/// Находится ли мышь над элементом.
	bool moused;

	/// Отправить уведомление.
	void Notify(int code);

public:
	/// Создать элемент.
	Element(ptr<Visualizer> visualizer);

	/// Получить родительский элемент.
	ContainerElement* GetParent() const;
	/// Указать родительский элемент.
	void SetParent(ContainerElement* parent);

	/// Получить положение элемента.
	Position GetPosition() const;
	/// Указать положение элемента.
	void SetPosition(Position position);
	/// Получить размеры элемента.
	Size GetSize() const;
	/// Указать размеры элемента.
	void SetSize(Size size);
	/// Получить, находится ли элемент в фокусе.
	bool IsFocused() const;
	/// Установить фокус на элемент.
	void SetFocus();

	/// Проверить, принадлежит ли точка элементу.
	virtual bool IsPositionInto(Position position) const;

	/// Нарисовать элемент.
	virtual void Draw(Position offset = Position(0, 0)) = 0;

	/// Над элементом пролетает мышь.
	virtual void EventMouseMove(Position position);
	/// На элементе нажата мышь.
	/**
	\param position Положение мыши относительно элемента.
	\param button Нажатая кнопка мыши.
	*/
	virtual void EventMouseDown(Position position, MouseButton button);
	/// На элементе отжата мышь.
	/**
	\param position Положение мыши относительно элемента.
	\param button Отжатая кнопка мыши.
	*/
	virtual void EventMouseUp(Position position, MouseButton button);
	/// В пространство элемента вошла мышь.
	virtual void EventMouseEnter();
	/// Мышь вышла из пространства элемента.
	virtual void EventMouseLeave();
	/// Элемент получил фокус.
	virtual void EventSetFocus();
	/// Элемент потерял фокус.
	virtual void EventLostFocus();
	/// Нажата клавиша клавиатуры.
	virtual void EventKeyDown(Key key);
	/// Отжата клавиша клавиатуры.
	virtual void EventKeyUp(Key key);
	/// Нажата клавиша клавиатуры (в смысле для печати).
	virtual void EventKeyPress(wchar_t key);
};

END_INANITY_GUI

#endif
