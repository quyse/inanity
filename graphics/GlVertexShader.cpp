#include "GlVertexShader.hpp"
#include "GlDevice.hpp"

GlVertexShader::GlVertexShader(ptr<GlDevice> device, GLuint programName, GLuint shaderName)
: device(device), programName(programName), shaderName(shaderName)
{
}
