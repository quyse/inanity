#ifndef ___INANITY_GUI_VISUALIZER_HPP___
#define ___INANITY_GUI_VISUALIZER_HPP___

#include "types.hpp"
#include "Visuals.hpp"
#include "../graphics/FontAlignment.hpp"
#include "../String.hpp"

BEGIN_INANITY_GUI

class Canvas;

/// Абстрактный класс рисователя интерфейса.
/** Придуман для того, чтобы отделить рисование интерфейса
от обработки его команд. Система координат такая же, как принята во всем GUI.
Кроме рисования, он также возвращает полезные размеры, которые используются
для позиционирования элементов. */
class Visualizer : public Object
{
public:
	/// Получить заданную визуальную метрику.
	virtual float GetVisualMetric(VisualMetric metric) const = 0;
	/// Нарисовать визуальный элемент.
	virtual void DrawVisualElement(Canvas* canvas, VisualElement element, float x1, float y1, float x2, float y2) = 0;
	/// Получить размеры текста.
	virtual Size GetTextSize(VisualFont font, const String& text) const = 0;
	/// Напечатать текст.
	virtual void DrawRectText(Canvas* canvas, VisualFont font, const String& text, float x1, float y1, float x2, float y2) = 0;
};

END_INANITY_GUI

#endif
