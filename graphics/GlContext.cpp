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
#include "GlDepthStencilState.hpp"
#include "GlBlendState.hpp"
#include "GlInternalTexture.hpp"
#include "VertexLayout.hpp"
#include "../MemoryFile.hpp"
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

void GlContext::UpdateFramebuffer()
{
	// framebuffer
	THROW_ASSERT(cellFrameBuffer.top);
	if(!cellFrameBuffer.IsActual())
	{
		THROW_ASSERT(((LetFrameBuffer*)cellFrameBuffer.top)->frameBuffer);

		LetFrameBuffer* top = (LetFrameBuffer*)cellFrameBuffer.top;
		LetFrameBuffer* current = (LetFrameBuffer*)cellFrameBuffer.current;

		GlFrameBuffer* frameBuffer = fast_cast<GlFrameBuffer*>(&*top->frameBuffer);
		GLuint frameBufferName = frameBuffer->GetName();
		if(!current || current->frameBuffer != top->frameBuffer || boundFrameBuffer != frameBufferName || frameBuffer->IsDirty())
			frameBuffer->Apply();

		cellFrameBuffer.Actual();
	}
}

void GlContext::Update()
{
	UpdateFramebuffer();

	// samplers
	for(int i = 0; i < samplersCount; ++i)
		if(!cellSamplers[i].IsActual())
		{
			LetSampler* let = (LetSampler*)cellSamplers[i].top;

			glActiveTexture(GL_TEXTURE0 + i);

			Texture* abstractTexture = let ? let->texture : nullptr;
			glBindTexture(GL_TEXTURE_2D, abstractTexture ? fast_cast<GlTexture*>(abstractTexture)->GetName() : 0);

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
			SamplerState* abstractSamplerState = let ? let->samplerState : nullptr;
			glBindSampler(i, abstractSamplerState ? fast_cast<GlSamplerState*>(abstractSamplerState)->GetName() : 0);
#endif

			cellSamplers[i].Actual();
		}
	GlSystem::CheckErrors("Can't bind textures and samplers");

	// vertex and pixel shaders
	if(!cellVertexShader.IsActual() || !cellPixelShader.IsActual())
	{
		LetVertexShader* letV = (LetVertexShader*)cellVertexShader.top;
		VertexShader* abstractVertexShader = letV->vertexShader;
		LetPixelShader* letP = (LetPixelShader*)cellPixelShader.top;
		PixelShader* abstractPixelShader = letP->pixelShader;
		ptr<GlInternalProgram> program = programCache->GetProgram(
			fast_cast<GlVertexShader*>(abstractVertexShader),
			fast_cast<GlPixelShader*>(abstractPixelShader)
		);
		if(boundProgram != program)
		{
			glUseProgram(program->GetName());
			GlSystem::CheckErrors("Can't bind program");
			boundProgram = program;

			// if uniform buffer object is unsupported, reset all uniform buffers
			if(!(device->GetInternalCaps() & GlDevice::InternalCaps::uniformBufferObject))
			{
				for(int i = 0; i < uniformBuffersCount; ++i)
					cellUniformBuffers[i].Reset();
			}
		}

		cellVertexShader.Actual();
		cellPixelShader.Actual();
	}

	// uniform buffers
	// if uniform buffer object is supported
	if(device->GetInternalCaps() & GlDevice::InternalCaps::uniformBufferObject)
	{
		for(int i = 0; i < uniformBuffersCount; ++i)
			if(!cellUniformBuffers[i].IsActual())
			{
				LetUniformBuffer* let = (LetUniformBuffer*)cellUniformBuffers[i].top;
				UniformBuffer* abstractUniformBuffer = let ? let->uniformBuffer : nullptr;
				glBindBufferBase(GL_UNIFORM_BUFFER, i, abstractUniformBuffer ? fast_cast<GlUniformBuffer*>(abstractUniformBuffer)->GetName() : 0);

				cellUniformBuffers[i].Actual();
			}
		GlSystem::CheckErrors("Can't bind uniform buffers");
	}
	// else upload uniforms one by one
	else
	{
		const GlInternalProgram::UniformBindings& uniformBindings = boundProgram->GetUniformBindings();
		for(size_t i = 0; i < uniformBindings.size(); ++i)
		{
			const GlInternalProgram::UniformBinding& uniformBinding = uniformBindings[i];
			const Cell& cell = cellUniformBuffers[uniformBinding.slot];
			LetUniformBuffer* let = (LetUniformBuffer*)cell.top;
			THROW_ASSERT(let);
			GlUniformBuffer* uniformBuffer = fast_cast<GlUniformBuffer*>(&*let->uniformBuffer);
			THROW_ASSERT(uniformBuffer);
			if(cell.IsActual() && !uniformBuffer->IsDirty())
				continue;
			MemoryFile* file = uniformBuffer->GetFile();
			THROW_ASSERT(file);
			const void* data = (const char*)file->GetData() + uniformBinding.offset;
			switch(uniformBinding.dataType)
			{
			case DataTypes::_float:
				glUniform1fv(uniformBinding.location, uniformBinding.count, (const GLfloat*)data);
				break;
			case DataTypes::_vec2:
				glUniform2fv(uniformBinding.location, uniformBinding.count, (const GLfloat*)data);
				break;
			case DataTypes::_vec3:
				glUniform3fv(uniformBinding.location, uniformBinding.count, (const GLfloat*)data);
				break;
			case DataTypes::_vec4:
				glUniform4fv(uniformBinding.location, uniformBinding.count, (const GLfloat*)data);
				break;
			case DataTypes::_mat3x3:
				glUniformMatrix3fv(uniformBinding.location, uniformBinding.count, GL_FALSE, (const GLfloat*)data);
				break;
			case DataTypes::_mat4x4:
				glUniformMatrix4fv(uniformBinding.location, uniformBinding.count, GL_FALSE, (const GLfloat*)data);
				break;
#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
			case DataTypes::_uint:
				glUniform1uiv(uniformBinding.location, uniformBinding.count, (const GLuint*)data);
				break;
			case DataTypes::_uvec2:
				glUniform2uiv(uniformBinding.location, uniformBinding.count, (const GLuint*)data);
				break;
			case DataTypes::_uvec3:
				glUniform3uiv(uniformBinding.location, uniformBinding.count, (const GLuint*)data);
				break;
			case DataTypes::_uvec4:
				glUniform4uiv(uniformBinding.location, uniformBinding.count, (const GLuint*)data);
				break;
#endif
			case DataTypes::_int:
			case DataTypes::_bool:
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
			case DataTypes::_uint:
#endif
				glUniform1iv(uniformBinding.location, uniformBinding.count, (const GLint*)data);
				break;
			case DataTypes::_ivec2:
			case DataTypes::_bvec2:
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
			case DataTypes::_uvec2:
#endif
				glUniform2iv(uniformBinding.location, uniformBinding.count, (const GLint*)data);
				break;
			case DataTypes::_ivec3:
			case DataTypes::_bvec3:
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
			case DataTypes::_uvec3:
#endif
				glUniform3iv(uniformBinding.location, uniformBinding.count, (const GLint*)data);
				break;
			case DataTypes::_ivec4:
			case DataTypes::_bvec4:
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
			case DataTypes::_uvec4:
#endif
				glUniform4iv(uniformBinding.location, uniformBinding.count, (const GLint*)data);
				break;
			}
			GlSystem::CheckErrors("Can't set uniform");
		}

		// make all uniform buffers actual and clean
		for(size_t i = 0; i < uniformBindings.size(); ++i)
		{
			const GlInternalProgram::UniformBinding& uniformBinding = uniformBindings[i];
			Cell& cell = cellUniformBuffers[uniformBinding.slot];
			cell.Actual();
			LetUniformBuffer* let = (LetUniformBuffer*)cell.top;
			GlUniformBuffer* uniformBuffer = fast_cast<GlUniformBuffer*>(&*let->uniformBuffer);
			uniformBuffer->SetDirty(false);
		}
	}

	// if vertex arrays supported, use them
	if(device->GetInternalCaps() & GlDevice::InternalCaps::vertexArrayObject)
	{
		// attribute binding
		THROW_ASSERT(cellAttributeBinding.top);
		if(!cellAttributeBinding.IsActual())
		{
			LetAttributeBinding* let = (LetAttributeBinding*)cellAttributeBinding.top;
			AttributeBinding* abstractAttributeBinding = let ? let->attributeBinding : nullptr;
			glBindVertexArray(fast_cast<GlAttributeBinding*>(abstractAttributeBinding)->GetVertexArrayName());
			GlSystem::CheckErrors("Can't bind attribute binding");
		}
	}

	// use vertex buffers if possible
	if(device->GetInternalCaps() & GlDevice::InternalCaps::vertexAttribBinding)
	{
		// vertex buffers
		for(int i = 0; i < vertexBuffersCount; ++i)
			if(!cellVertexBuffers[i].IsActual())
			{
				LetVertexBuffer* let = (LetVertexBuffer*)cellVertexBuffers[i].top;
				VertexBuffer* abstractVertexBuffer = let->vertexBuffer;
				if(abstractVertexBuffer)
				{
					GlVertexBuffer* vertexBuffer = fast_cast<GlVertexBuffer*>(abstractVertexBuffer);
					glBindVertexBuffer((GLuint)i, vertexBuffer->GetName(), 0, vertexBuffer->GetLayout()->GetStride());
				}
				else
					glBindVertexBuffer((GLuint)i, 0, 0, 0);
				GlSystem::CheckErrors("Can't bind vertex buffer");

				cellVertexBuffers[i].Actual();
			}
	}
	// otherwise do manual binding
	else
	{
		// attribute binding
		THROW_ASSERT(cellAttributeBinding.top);
		LetAttributeBinding* letAB = (LetAttributeBinding*)cellAttributeBinding.top;
		AttributeBinding* abstractAttributeBinding = letAB ? letAB->attributeBinding : nullptr;
		GlAttributeBinding* attributeBinding = fast_cast<GlAttributeBinding*>(abstractAttributeBinding);

		// if attribute binding has changed, rebind all slots
		// else we may rebind changed slots only
		bool rebindAllSlots = !cellAttributeBinding.IsActual();

		const GlAttributeBinding::Slots& slots = attributeBinding->GetSlots();

		int maxUsedElement = 0;

		const bool supportAttributeInstancing = !!(device->GetCaps().flags & Device::Caps::attributeInstancing);

		for(int i = 0; i < vertexBuffersCount; ++i)
			if(rebindAllSlots || !cellVertexBuffers[i].IsActual())
			{
				if(i < (int)slots.size())
				{
					LetVertexBuffer* letVB = (LetVertexBuffer*)cellVertexBuffers[i].top;
					VertexBuffer* abstractVertexBuffer = letVB ? letVB->vertexBuffer : nullptr;
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

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
							if(element.integer)
								glVertexAttribIPointer(element.index, element.size, element.type, stride, element.pointer);
							else
#endif
								glVertexAttribPointer(element.index, element.size, element.type, element.normalized, stride, element.pointer);

							if(supportAttributeInstancing)
								glVertexAttribDivisor(element.index, slots[i].divisor);

							if((int)element.index > maxUsedElement)
								maxUsedElement = element.index;
						}

						GlSystem::CheckErrors("Can't bind vertex buffer");
					}
				}

				cellVertexBuffers[i].Actual();
			}

		// if binding has changed, disable unused attributes
		if(rebindAllSlots)
		{
			for(int i = maxUsedElement + 1; i < boundAttributesCount; ++i)
				glDisableVertexAttribArray(i);
			boundAttributesCount = maxUsedElement + 1;
			GlSystem::CheckErrors("Can't disable unused attributes");
		}
	}

	// make attribute binding cell actual in any case
	cellAttributeBinding.Actual();

	// index buffer
	if(!cellIndexBuffer.IsActual())
	{
		LetIndexBuffer* let = (LetIndexBuffer*)cellIndexBuffer.top;
		IndexBuffer* abstractIndexBuffer = let->indexBuffer;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, abstractIndexBuffer ? fast_cast<GlIndexBuffer*>(abstractIndexBuffer)->GetName() : 0);
		GlSystem::CheckErrors("Can't bind index buffer");

		cellIndexBuffer.Actual();
	}

#ifndef ___INANITY_PLATFORM_EMSCRIPTEN
	if(!cellFillMode.IsActual())
	{
		LetFillMode* let = (LetFillMode*)cellFillMode.top;
		FillMode fillMode = let->fillMode;
		switch(fillMode)
		{
		case fillModeWireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case fillModeSolid:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			THROW("Unknown fill mode");
		}
		GlSystem::CheckErrors("Can't bind fill mode");

		cellFillMode.Actual();
	}
#endif

	if(!cellCullMode.IsActual())
	{
		LetCullMode* let = (LetCullMode*)cellCullMode.top;
		CullMode cullMode = let->cullMode;
		switch(cullMode)
		{
		case cullModeNone:
			glDisable(GL_CULL_FACE);
			break;
		case cullModeBack:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case cullModeFront:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		default:
			THROW("Unknown cull mode");
		}
		GlSystem::CheckErrors("Can't bind cull mode");

		cellCullMode.Actual();
	}

	// viewport
	THROW_ASSERT(cellViewport.top);
	if(!cellViewport.IsActual())
	{
		LetViewport* let = (LetViewport*)cellViewport.top;
		glViewport(0, 0, let->viewportWidth, let->viewportHeight);
		GlSystem::CheckErrors("Can't bind viewport");

		cellViewport.Actual();
	}

	// depth-stencil state
	if(!cellDepthStencilState.IsActual())
	{
		LetDepthStencilState* let = (LetDepthStencilState*)cellDepthStencilState.top;
		DepthStencilState* abstractDepthStencilState = let->depthStencilState;
		if(abstractDepthStencilState)
			fast_cast<GlDepthStencilState*>(abstractDepthStencilState)->Apply();
		else
			GlDepthStencilState::ApplyDefault();

		GlSystem::CheckErrors("Can't bind depth-stencil state");

		cellDepthStencilState.Actual();
	}

	// blend state
	if(!cellBlendState.IsActual())
	{
		LetBlendState* let = (LetBlendState*)cellBlendState.top;
		BlendState* abstractBlendState = let->blendState;
		if(abstractBlendState)
			fast_cast<GlBlendState*>(abstractBlendState)->Apply();
		else
			GlBlendState::ApplyDefault();

		GlSystem::CheckErrors("Can't bind blend state");

		cellBlendState.Actual();
	}

	GlSystem::CheckErrors("Can't update context");
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

void GlContext::ClearColor(int colorBufferIndex, const vec4& color)
{
	UpdateFramebuffer();
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT);
#else
	glClearBufferfv(GL_COLOR, (GLint)colorBufferIndex, color.t);
#endif
	GlSystem::CheckErrors("Can't clear color buffer");
}

void GlContext::ClearDepth(float depth)
{
	UpdateFramebuffer();
	// nesessary for depth clear
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_TRUE);
	cellDepthStencilState.Reset();

#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	glClearDepth(depth);
	glClear(GL_DEPTH_BUFFER_BIT);
#else
	glClearBufferfv(GL_DEPTH, 0, &depth);
#endif
	GlSystem::CheckErrors("Can't clear depth");
}

void GlContext::ClearStencil(unsigned stencil)
{
	UpdateFramebuffer();
	GLint s = stencil;
#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	glClearStencil((GLint)s);
	glClear(GL_STENCIL_BUFFER_BIT);
#else
	glClearBufferiv(GL_STENCIL, 0, &s);
#endif
	GlSystem::CheckErrors("Can't clear stencil");
}

void GlContext::ClearDepthStencil(float depth, unsigned stencil)
{
	UpdateFramebuffer();
	// nesessary for depth clear
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_TRUE);
	cellDepthStencilState.Reset();

#ifdef ___INANITY_PLATFORM_EMSCRIPTEN
	glClearDepth(depth);
	glClearStencil(stencil);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#else
	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
#endif
	GlSystem::CheckErrors("Can't clear depth and stencil");
}

void GlContext::UploadBufferData(GLenum target, GLuint bufferName, const void* data, int size, int bufferSize)
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

void GlContext::UploadUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, int size)
{
	GlUniformBuffer* uniformBuffer = fast_cast<GlUniformBuffer*>(&*abstractUniformBuffer);

	THROW_ASSERT(!uniformBuffer->GetName() == !(device->GetInternalCaps() & GlDevice::InternalCaps::uniformBufferObject));

	// if uniform buffer is real, do upload
	GLuint name = uniformBuffer->GetName();
	if(name)
		UploadBufferData(GL_UNIFORM_BUFFER, name, data, size, uniformBuffer->GetSize());
	else
	{
		// else data is stored in memory buffer
		// actual uploading is deferred until drawing
		THROW_ASSERT(uniformBuffer->GetFile()->GetSize() >= (size_t)size);
		memcpy(uniformBuffer->GetFile()->GetData(), data, size);
		uniformBuffer->SetDirty(true);
	}
}

void GlContext::UploadVertexBufferData(VertexBuffer* abstractVertexBuffer, const void* data, int size)
{
	GlVertexBuffer* vertexBuffer = fast_cast<GlVertexBuffer*>(&*abstractVertexBuffer);

	UploadBufferData(GL_ARRAY_BUFFER, vertexBuffer->GetName(), data, size, vertexBuffer->GetSize());
}

void GlContext::UploadIndexBufferData(IndexBuffer* abstractIndexBuffer, const void* data, int size)
{
	GlIndexBuffer* indexBuffer = fast_cast<GlIndexBuffer*>(&*abstractIndexBuffer);

	UploadBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetName(), data, size, indexBuffer->GetIndicesCount() * indexBuffer->GetIndexSize());
	cellIndexBuffer.Reset();
}

void GlContext::Draw(int count)
{
	Update();

	LetIndexBuffer* let = (LetIndexBuffer*)cellIndexBuffer.current;
	IndexBuffer* abstractIndexBuffer = let ? let->indexBuffer : nullptr;
	if(abstractIndexBuffer)
	{
		GlIndexBuffer* indexBuffer = fast_cast<GlIndexBuffer*>(abstractIndexBuffer);
		glDrawElements(GL_TRIANGLES, count >= 0 ? count : indexBuffer->GetIndicesCount(), indexBuffer->GetIndicesType(), (void*)0);
	}
	else
	{
		THROW_ASSERT(cellVertexBuffers[0].current);
		VertexBuffer* abstractVertexBuffer = ((LetVertexBuffer*)cellVertexBuffers[0].current)->vertexBuffer;
		THROW_ASSERT(abstractVertexBuffer);
		glDrawArrays(GL_TRIANGLES, 0, count >= 0 ? count : fast_cast<GlVertexBuffer*>(abstractVertexBuffer)->GetVerticesCount());
	}
	GlSystem::CheckErrors("Can't draw");
}

void GlContext::DrawInstanced(int instancesCount, int count)
{
	Update();

	LetIndexBuffer* let = (LetIndexBuffer*)cellIndexBuffer.current;
	IndexBuffer* abstractIndexBuffer = let ? let->indexBuffer : nullptr;
	if(abstractIndexBuffer)
	{
		GlIndexBuffer* indexBuffer = fast_cast<GlIndexBuffer*>(abstractIndexBuffer);
		glDrawElementsInstanced(GL_TRIANGLES, count >= 0 ? count : indexBuffer->GetIndicesCount(), indexBuffer->GetIndicesType(), (void*)0, instancesCount);
	}
	else
	{
		THROW_ASSERT(cellVertexBuffers[0].current);
		VertexBuffer* abstractVertexBuffer = ((LetVertexBuffer*)cellVertexBuffers[0].current)->vertexBuffer;
		THROW_ASSERT(abstractVertexBuffer);
		glDrawArraysInstanced(GL_TRIANGLES, 0, count >= 0 ? count : fast_cast<GlVertexBuffer*>(abstractVertexBuffer)->GetVerticesCount(), instancesCount);
	}
	GlSystem::CheckErrors("Can't draw instanced");
}

END_INANITY_GRAPHICS
