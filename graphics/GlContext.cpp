#include "GlContext.hpp"
#include "GlDevice.hpp"
#include "GlSystem.hpp"
#include "GlInternalProgramCache.hpp"
#include "GlInternalProgram.hpp"
#include "GlFrameBuffer.hpp"
#include "GlRenderBuffer.hpp"
#include "GlDepthStencilBuffer.hpp"
#include "GlTexture.hpp"
#include "GlSamplerState.hpp"
#include "GlUniformBuffer.hpp"
#include "GlVertexShader.hpp"
#include "GlPixelShader.hpp"
#include "GlAttributeBinding.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlBlendState.hpp"
#include "GlInternalTexture.hpp"
#include "VertexLayout.hpp"
#include "../Exception.hpp"
#include <memory.h>

BEGIN_INANITY_GRAPHICS

GlContext::GlContext(ptr<GlDevice> device) :
	device(device),
	boundFrameBuffer(0), boundAttributesCount(0)
{
	programCache = NEW(GlInternalProgramCache(device));

	// set front face like in Direct3D
	glFrontFace(GL_CW);
}

void GlContext::Update()
{
	// framebuffer
	{
		GlFrameBuffer* targetFrameBuffer = fast_cast<GlFrameBuffer*>(&*targetState.frameBuffer);
		GLuint targetFrameBufferName = targetFrameBuffer->GetName();
		// check if need to bind
		if(forceReset || targetState.frameBuffer != boundState.frameBuffer || boundFrameBuffer != targetFrameBufferName || targetFrameBuffer->IsDirty())
		{
			// bind framebuffer
			targetFrameBuffer->Apply();

			// remember new state
			boundState.frameBuffer = targetState.frameBuffer;
			boundFrameBuffer = targetFrameBufferName;
		}
	}

	// текстуры
	for(int i = 0; i < ContextState::textureSlotsCount; ++i)
		if(forceReset || targetState.textures[i] != boundState.textures[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			Texture* abstractTexture = targetState.textures[i];
			glBindTexture(GL_TEXTURE_2D, abstractTexture ? fast_cast<GlTexture*>(abstractTexture)->GetName() : 0);

			boundState.textures[i] = targetState.textures[i];
		}
	// семплеры
	for(int i = 0; i < ContextState::textureSlotsCount; ++i)
		if(forceReset || targetState.samplerStates[i] != boundState.samplerStates[i])
		{
			SamplerState* abstractSamplerState = targetState.samplerStates[i];
			glBindSampler(i, abstractSamplerState ? fast_cast<GlSamplerState*>(abstractSamplerState)->GetName() : 0);

			boundState.samplerStates[i] = targetState.samplerStates[i];
		}
	GlSystem::CheckErrors("Can't bind textures and samplers");

	// uniform-буферы
	for(int i = 0; i < ContextState::uniformBufferSlotsCount; ++i)
		if(forceReset || targetState.uniformBuffers[i] != boundState.uniformBuffers[i])
		{
			UniformBuffer* abstractUniformBuffer = targetState.uniformBuffers[i];
			glBindBufferBase(GL_UNIFORM_BUFFER, i, abstractUniformBuffer ? fast_cast<GlUniformBuffer*>(abstractUniformBuffer)->GetName() : 0);

			boundState.uniformBuffers[i] = targetState.uniformBuffers[i];
		}
	GlSystem::CheckErrors("Can't bind uniform buffers");

	// вершинный и пиксельный шейдеры
	if(forceReset || targetState.vertexShader != boundState.vertexShader || targetState.pixelShader != boundState.pixelShader)
	{
		ptr<GlInternalProgram> program = programCache->GetProgram(fast_cast<GlVertexShader*>(&*targetState.vertexShader), fast_cast<GlPixelShader*>(&*targetState.pixelShader));
		if(forceReset || boundProgram != program)
		{
			glUseProgram(program->GetName());
			GlSystem::CheckErrors("Can't bind program");
			boundProgram = program;
		}

		boundState.vertexShader = targetState.vertexShader;
		boundState.pixelShader = targetState.pixelShader;
	}

	// если вершинные буферы поддерживаются, используем их
	if(GLEW_ARB_vertex_attrib_binding)
	{
		// привязка атрибутов
		if(forceReset || targetState.attributeBinding != boundState.attributeBinding)
		{
			glBindVertexArray(fast_cast<GlAttributeBinding*>(&*targetState.attributeBinding)->GetVertexArrayName());
			GlSystem::CheckErrors("Can't bind attribute binding");

			boundState.attributeBinding = targetState.attributeBinding;
		}

		// вершинные буферы
		for(int i = 0; i < ContextState::vertexBufferSlotsCount; ++i)
			if(forceReset || targetState.vertexBuffers[i] != boundState.vertexBuffers[i])
			{
				VertexBuffer* abstractVertexBuffer = targetState.vertexBuffers[i];
				if(abstractVertexBuffer)
				{
					GlVertexBuffer* vertexBuffer = fast_cast<GlVertexBuffer*>(abstractVertexBuffer);
					glBindVertexBuffer((GLuint)i, vertexBuffer->GetName(), 0, vertexBuffer->GetLayout()->GetStride());
				}
				else
					glBindVertexBuffer((GLuint)i, 0, 0, 0);
				GlSystem::CheckErrors("Can't bind vertex buffer");

				boundState.vertexBuffers[i] = targetState.vertexBuffers[i];
			}
	}
	// иначе выполняем привязку "вручную"
	else
	{
		GlAttributeBinding* attributeBinding = fast_cast<GlAttributeBinding*>(&*targetState.attributeBinding);
		// если привязка атрибутов изменилась, перепривязываем все слоты
		// а если только вершинные буферы изменились, можно перепривязать
		// только изменившиеся слоты
		bool rebindAllSlots = forceReset || targetState.attributeBinding != boundState.attributeBinding;

		const GlAttributeBinding::Slots& slots = attributeBinding->GetSlots();

		int maxUsedElement = 0;

		for(int i = 0; i < ContextState::vertexBufferSlotsCount; ++i)
			if(rebindAllSlots || targetState.vertexBuffers[i] != boundState.vertexBuffers[i])
			{
				if(i < (int)slots.size())
				{
					VertexBuffer* abstractVertexBuffer = targetState.vertexBuffers[i];
					if(abstractVertexBuffer)
					{
						GlVertexBuffer* vertexBuffer = fast_cast<GlVertexBuffer*>(abstractVertexBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetName());

						int stride = vertexBuffer->GetLayout()->GetStride();

						const GlAttributeBinding::Elements& elements = slots[i].elements;
						for(int j = 0; j < (int)elements.size(); ++j)
						{
							const GlAttributeBinding::Element& element = elements[j];
							glEnableVertexAttribArray(element.index);
							if(element.integer)
								glVertexAttribIPointer(element.index, element.size, element.type, stride, element.pointer);
							else
								glVertexAttribPointer(element.index, element.size, element.type, element.normalized, stride, element.pointer);
							glVertexAttribDivisor(element.index, slots[i].divisor);
							if((int)element.index > maxUsedElement)
								maxUsedElement = element.index;
						}

						GlSystem::CheckErrors("Can't bind vertex buffer");
					}
				}

				boundState.vertexBuffers[i] = targetState.vertexBuffers[i];
			}

		// если привязка менялась, отключить неиспользуемые атрибуты
		if(rebindAllSlots)
		{
			for(int i = maxUsedElement + 1; i < boundAttributesCount; ++i)
				glDisableVertexAttribArray(i);
			boundAttributesCount = maxUsedElement + 1;
			GlSystem::CheckErrors("Can't disable unused attributes");

			boundState.attributeBinding = targetState.attributeBinding;
		}
	}

	// индексный буфер
	if(forceReset || targetState.indexBuffer != boundState.indexBuffer)
	{
		IndexBuffer* abstractIndexBuffer = targetState.indexBuffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, abstractIndexBuffer ? fast_cast<GlIndexBuffer*>(abstractIndexBuffer)->GetName() : 0);
		GlSystem::CheckErrors("Can't bind index buffer");

		boundState.indexBuffer = targetState.indexBuffer;
	}

	if(forceReset || targetState.fillMode != boundState.fillMode)
	{
		switch(targetState.fillMode)
		{
		case ContextState::fillModeWireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case ContextState::fillModeSolid:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			THROW("Unknown fill mode");
		}
		GlSystem::CheckErrors("Can't bind fill mode");

		boundState.fillMode = targetState.fillMode;
	}

	if(forceReset || targetState.cullMode != boundState.cullMode)
	{
		switch(targetState.cullMode)
		{
		case ContextState::cullModeNone:
			glDisable(GL_CULL_FACE);
			break;
		case ContextState::cullModeBack:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case ContextState::cullModeFront:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		default:
			THROW("Unknown cull mode");
		}
		GlSystem::CheckErrors("Can't bind cull mode");

		boundState.cullMode = targetState.cullMode;
	}

	if(forceReset || targetState.viewportWidth != boundState.viewportWidth || targetState.viewportHeight != boundState.viewportHeight)
	{
		glViewport(0, 0, targetState.viewportWidth, targetState.viewportHeight);
		GlSystem::CheckErrors("Can't bind viewport");

		boundState.viewportWidth = targetState.viewportWidth;
		boundState.viewportHeight = targetState.viewportHeight;
	}

	if(forceReset || targetState.depthTestFunc != boundState.depthTestFunc || targetState.depthWrite != boundState.depthWrite)
	{
		((targetState.depthTestFunc != ContextState::depthTestFuncAlways || targetState.depthWrite) ? glEnable : glDisable)(GL_DEPTH_TEST);
	}

	if(forceReset || targetState.depthTestFunc != boundState.depthTestFunc)
	{
		GLenum func;
		switch(targetState.depthTestFunc)
		{
		case ContextState::depthTestFuncNever:
			func = GL_NEVER;
			break;
		case ContextState::depthTestFuncLess:
			func = GL_LESS;
			break;
		case ContextState::depthTestFuncLessOrEqual:
			func = GL_LEQUAL;
			break;
		case ContextState::depthTestFuncEqual:
			func = GL_EQUAL;
			break;
		case ContextState::depthTestFuncNonEqual:
			func = GL_NOTEQUAL;
			break;
		case ContextState::depthTestFuncGreaterOrEqual:
			func = GL_GEQUAL;
			break;
		case ContextState::depthTestFuncGreater:
			func = GL_GREATER;
			break;
		case ContextState::depthTestFuncAlways:
			func = GL_ALWAYS;
			break;
		default:
			THROW("Unknown depth test func");
		}

		glDepthFunc(func);

		GlSystem::CheckErrors("Can't bind depth test func");

		boundState.depthTestFunc = targetState.depthTestFunc;
	}

	if(forceReset || targetState.depthWrite != boundState.depthWrite)
	{
		glDepthMask(targetState.depthWrite ? GL_TRUE : GL_FALSE);

		GlSystem::CheckErrors("Can't bind depth write");

		boundState.depthWrite = targetState.depthWrite;
	}

	if(forceReset || targetState.blendState != boundState.blendState)
	{
		if(targetState.blendState)
			fast_cast<GlBlendState*>(&*targetState.blendState)->Apply();
		else
			GlBlendState::ApplyDefault();

		GlSystem::CheckErrors("Can't bind blend state");

		boundState.blendState = targetState.blendState;
	}

	GlSystem::CheckErrors("Can't update context");

	forceReset = false;
}

ptr<GlRenderBuffer> GlContext::GetDummyRenderBuffer(int width, int height)
{
	std::pair<int, int> key(width, height);
	std::map<std::pair<int, int>, ptr<GlRenderBuffer> >::const_iterator i = dummyRenderBuffers.find(key);
	if(i != dummyRenderBuffers.end())
		return i->second;

	try
	{
		GLuint textureName;
		glGenTextures(1, &textureName);
		GlSystem::CheckErrors("Can't gen texture");
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(device, textureName));
		glBindTexture(GL_TEXTURE_2D, textureName);
		GlSystem::CheckErrors("Can't bind texture");

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		GlSystem::CheckErrors("Can't initialize texture");

		ptr<GlRenderBuffer> renderBuffer = NEW(GlRenderBuffer(internalTexture, 0));
		dummyRenderBuffers[key] = renderBuffer;
		return renderBuffer;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't get dummy render buffer", exception);
	}
}

void GlContext::ClearColor(int colorBufferIndex, const float* color)
{
	glClearBufferfv(GL_COLOR, (GLint)colorBufferIndex, color);
	GlSystem::CheckErrors("Can't clear color buffer");
}

void GlContext::ClearDepth(float depth)
{
	// nesessary for depth clear
	glEnable(GL_DEPTH_TEST);
	boundState.depthTestFunc = ContextState::depthTestFuncAlways;
	glDepthFunc(GL_ALWAYS);
	boundState.depthWrite = true;
	glDepthMask(GL_TRUE);

	glClearBufferfv(GL_DEPTH, 0, &depth);
	GlSystem::CheckErrors("Can't clear depth");
}

void GlContext::ClearStencil(unsigned stencil)
{
	GLint s = stencil;
	glClearBufferiv(GL_STENCIL, 0, &s);
	GlSystem::CheckErrors("Can't clear stencil");
}

void GlContext::ClearDepthStencil(float depth, unsigned stencil)
{
	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
	GlSystem::CheckErrors("Can't clear depth and stencil");
}

void GlContext::SetBufferData(GLenum target, GLuint bufferName, const void* data, int size, int bufferSize)
{
	// проверить, что размер правильный
	if(size > bufferSize)
		THROW("Size of data to set into OpenGL buffer is too big");

	// http://www.opengl.org/wiki/GLAPI/glBindBuffer
	// Сказано, что такая привязка не снимает буферы, привязанные
	// через glBindBufferBase. Верим, но надо проверить.
	glBindBuffer(target, bufferName);
	glBufferData(target, size, data, GL_DYNAMIC_DRAW);
	GlSystem::CheckErrors("Can't upload data into OpenGL buffer");
}

void GlContext::SetUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, int size)
{
	GlUniformBuffer* uniformBuffer = fast_cast<GlUniformBuffer*>(&*abstractUniformBuffer);

	SetBufferData(GL_UNIFORM_BUFFER, uniformBuffer->GetName(), data, size, uniformBuffer->GetSize());
}

void GlContext::SetVertexBufferData(VertexBuffer* abstractVertexBuffer, const void* data, int size)
{
	GlVertexBuffer* vertexBuffer = fast_cast<GlVertexBuffer*>(&*abstractVertexBuffer);

	SetBufferData(GL_ARRAY_BUFFER, vertexBuffer->GetName(), data, size, vertexBuffer->GetSize());
}

void GlContext::Draw(int count)
{
	Update();

	if(boundState.indexBuffer)
	{
		GlIndexBuffer* indexBuffer = fast_cast<GlIndexBuffer*>(&*boundState.indexBuffer);
		glDrawElements(GL_TRIANGLES, count >= 0 ? count : indexBuffer->GetIndicesCount(), indexBuffer->GetIndicesType(), (void*)0);
	}
	else
		glDrawArrays(GL_TRIANGLES, 0, count >= 0 ? count : boundState.vertexBuffers[0]->GetVerticesCount());
	GlSystem::CheckErrors("Can't draw");
}

void GlContext::DrawInstanced(int instancesCount, int count)
{
	Update();

	if(boundState.indexBuffer)
	{
		GlIndexBuffer* indexBuffer = fast_cast<GlIndexBuffer*>(&*boundState.indexBuffer);
		glDrawElementsInstanced(GL_TRIANGLES, count >= 0 ? count : indexBuffer->GetIndicesCount(), indexBuffer->GetIndicesType(), (void*)0, instancesCount);
	}
	else
		glDrawArraysInstanced(GL_TRIANGLES, 0, count >= 0 ? count : boundState.vertexBuffers[0]->GetVerticesCount(), instancesCount);
	GlSystem::CheckErrors("Can't draw instanced");
}

END_INANITY_GRAPHICS
