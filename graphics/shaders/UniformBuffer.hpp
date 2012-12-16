#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_BUFFER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_BUFFER_HPP___

class UniformBuffer
{
private:
public:
	/// Добавить uniform-переменную в буфер.
	/** \param offset Смещение в буфере. Если не задано, то буфер сам
	выберет положение для переменной наилучшим образом. */
	void AddUniform(const UniformBase& uniform, int offset = -1);
};

#endif
