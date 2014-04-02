#ifndef ___INANITY_GRAPHICS_BLEND_STATE_HPP___
#define ___INANITY_GRAPHICS_BLEND_STATE_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс настроек смешивания.
/** Смешивание выполняется по формуле:
resultRGB = (sourceRGB * <ColorA>) <color operation> (destRGB * <ColorB>)
resultAlpha = (sourceAlpha * <AlphaA>) <alpha operation> (destAlpha * <AlphaB>)
*/
class BlendState : public Object
{
public:
	/// Источник цвета.
	/** А точнее, на что нужно умножать цвет.
	Источники с inv предварительно выполняют 1-X. */
	enum ColorSource
	{
		colorSourceZero,
		colorSourceOne,
		colorSourceSrc,
		colorSourceInvSrc,
		colorSourceSrcAlpha,
		colorSourceInvSrcAlpha,
		colorSourceDest,
		colorSourceInvDest,
		colorSourceDestAlpha,
		colorSourceInvDestAlpha,
		colorSourceSecondSrc,
		colorSourceInvSecondSrc,
		colorSourceSecondSrcAlpha,
		colorSourceInvSecondSrcAlpha
	};

	/// Источник альфа-значения.
	enum AlphaSource
	{
		alphaSourceZero,
		alphaSourceOne,
		alphaSourceSrc,
		alphaSourceInvSrc,
		alphaSourceDest,
		alphaSourceInvDest,
		alphaSourceSecondSrc,
		alphaSourceInvSecondSrc
	};

	/// Операция.
	enum Operation
	{
		operationAdd,
		operationSubtractAB,
		operationSubtractBA,
		operationMin,
		operationMax
	};

protected:
	/// Включение смешивания.
	bool enable;
	// Цвета для смешивания.
	ColorSource sourceColor, destColor;
	/// Операция над цветами.
	Operation colorOperation;
	// Alpha для смешивания.
	AlphaSource sourceAlpha, destAlpha;
	/// Операция над alpha.
	Operation alphaOperation;

	/// Общий флажок "грязности".
	bool dirty;

	/// Инициализация.
	BlendState();

public:
	/// Установить смешивание цвета.
	void SetColor(ColorSource sourceColor, ColorSource destColor, Operation colorOperation);
	/// Установить смешивание альфы.
	void SetAlpha(AlphaSource sourceAlpha, AlphaSource destAlpha, Operation alphaOperation);
};

END_INANITY_GRAPHICS

#endif
