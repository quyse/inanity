#ifndef ___INANITY_GRAPHICS_SAMPLER_SETTINGS_HPP___
#define ___INANITY_GRAPHICS_SAMPLER_SETTINGS_HPP___

#include "graphics.hpp"

BEGIN_INANITY_GRAPHICS

struct SamplerSettings
{
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
		filterLinear
	};

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
	vec4 borderColor;

	SamplerSettings();
};

END_INANITY_GRAPHICS

#endif
