#ifndef ___INANITY_GRAPHICS_SAMPLER_HPP___
#define ___INANITY_GRAPHICS_SAMPLER_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

/// Абстрактный класс семплера.
/** Хранит настройки семплирования. */
class Sampler : public Object
{
public:
	/// Режим свёртки координат, выходящих за пределы текстуры.
	enum Wrap
	{
		/// Повторять текстуру.
		wrapRepeat,
		/// Повторять, чередуя перевёрнутую и неперевёрнутую текстуру.
		wrapRepeatMirror,
		/// Ограничивать координаты рамками текстуры (брать цвет с крайнего пиксела).
		wrapClamp,
		/// Использовать цвет границы.
		wrapBorder
	};
	/// Режимы фильтрации.
	enum Filter
	{
		filterPoint,
		filterLinear,
		filterCount
	};

protected:
	/// Фильтрация для уменьшения.
	Filter minFilter;
	/// Фильтрация между mip-map уровнями.
	/** По сути, тоже только для уменьшения. */
	Filter mipFilter;
	/// Фильтрация для увеличения.
	Filter magFilter;

	/// Использовать mip-mapping.
	bool mipMapping;

	// Режимы свёртки координат.
	Wrap wrapU;
	Wrap wrapV;
	Wrap wrapW;

	/// Минимальный разрешённый LOD.
	float minLOD;
	/// Максимальный разрешённый LOD.
	float maxLOD;

	/// Цвет границы.
	float borderColor[4];

	/// Общий флажок "грязности".
	bool dirty;

protected:
	/// Инициализация.
	Sampler();

public:
	/// Установить фильтрацию.
	void SetFilter(Filter minFilter, Filter mipFilter, Filter magFilter);
	/// Установить использование Mip-Map.
	void SetMipMapping(bool mipMapping);
	/// Установить режим свёртки.
	void SetWrap(Wrap wrapU, Wrap wrapV, Wrap wrapW);
	/// Установить границы LOD.
	void SetLODLimits(float minLOD = -1000, float maxLOD = 1000);
	/// Установить цвет границы.
	void SetBorderColor(const float* borderColor);
};

END_INANITY_GRAPHICS

#endif
