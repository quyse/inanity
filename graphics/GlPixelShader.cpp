#include "GlPixelShader.hpp"

GlPixelShader::GlPixelShader(ptr<GlDevice> device, GLuint programName, GLuint shaderName)
: device(device), programName(programName), shaderName(shaderName)
{
}
