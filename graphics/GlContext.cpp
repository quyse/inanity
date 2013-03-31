#include "GlContext.hpp"
#include "GlSystem.hpp"
#include "GlInternalProgramCache.hpp"
#include "GlInternalProgram.hpp"
#include "GlRenderBuffer.hpp"
#include "GlDepthStencilBuffer.hpp"
#include "GlTexture.hpp"
#include "GlSamplerState.hpp"
#include "GlUniformBuffer.hpp"
#include "GlVertexShader.hpp"
#include "GlPixelShader.hpp"
#include "GlVertexBuffer.hpp"
#include "GlIndexBuffer.hpp"
#include "GlGeometry.hpp"
#include "GlBlendState.hpp"
#include "GlInternalTexture.hpp"
#include "Layout.hpp"
#include "../Exception.hpp"

GlContext::GlContext() :
	targetsFramebuffer(0), serviceFramebuffer(0),
	currentFramebuffer(0), dirtyCurrentFramebuffer(true)
{
	programCache = NEW(GlInternalProgramCache());
}

GlContext::~GlContext()
{
	if(targetsFramebuffer)
		glDeleteFramebuffers(1, &targetsFramebuffer);
	if(serviceFramebuffer)
		glDeleteFramebuffers(1, &serviceFramebuffer);
}

void GlContext::BindTargetsFramebuffer()
{
	if(!targetsFramebuffer)
		glGenFramebuffers(1, &targetsFramebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetsFramebuffer);
	GlSystem::CheckErrors("Can't bind targets framebuffer");

	currentFramebuffer = targetsFramebuffer;
	dirtyCurrentFramebuffer = false;
}

void GlContext::BindServiceFramebuffer()
{
	if(!serviceFramebuffer)
		glGenFramebuffers(1, &serviceFramebuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, serviceFramebuffer);
	GlSystem::CheckErrors("Can't bind service framebuffer");

	dirtyCurrentFramebuffer = true;
}

void GlContext::Update()
{
	// установить фреймбуфер в то, чем он был
	// далее его могут поменять, но здесь мы гарантируем, что он
	// соответствует тому, что в currentFramebuffer
	if(dirtyCurrentFramebuffer)
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer);
		GlSystem::CheckErrors("Can't bind current framebuffer");
		dirtyCurrentFramebuffer = false;
	}

	// проверить, если первый рендербуфер - по умолчанию (а непервым он быть не может),
	// то он должен быть единственным, и depth-stencil буфера не может быть
	if(targetState.renderBuffers[0] && fast_cast<GlRenderBuffer*>(&*targetState.renderBuffers[0])->GetName() == 0)
	{
		if(targetState.depthStencilBuffer)
			THROW_PRIMARY_EXCEPTION("Default renderbuffer can be bound only without depth-stencil buffer");
		for(int i = 1; i < ContextState::renderTargetSlotsCount; ++i)
			if(targetState.renderBuffers[i])
				THROW_PRIMARY_EXCEPTION("Default renderbuffer can be bound only without other buffers");

		// привязать фреймбуфер по умолчанию
		if(currentFramebuffer != 0)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer = 0);

		// больше ничего привязывать не надо

		// установить, что всё привязано
		for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
			boundState.renderBuffers[i] = targetState.renderBuffers[i];
		boundState.depthStencilBuffer = targetState.depthStencilBuffer;

		GlSystem::CheckErrors("qqq1");
	}
	// иначе все рендербуферы - не по умолчанию
	// определяем, что поменялось, и меняем
	else
	{
		// привязать нужный фреймбуфер
		BindTargetsFramebuffer();
		GlSystem::CheckErrors("qqq1_0");

		// привязать изменившиеся рендербуферы
		for(int i = 0; i < ContextState::renderTargetSlotsCount; ++i)
			if(targetState.renderBuffers[i] != boundState.renderBuffers[i])
			{
				RenderBuffer* abstractRenderBuffer = targetState.renderBuffers[i];
				if(abstractRenderBuffer)
				{
					GLuint name = fast_cast<GlRenderBuffer*>(abstractRenderBuffer)->GetName();
					if(name == 0)
						THROW_PRIMARY_EXCEPTION("Default renderbuffer can't be bound with other buffers");
					glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, name, 0);
				}
				else
					glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, 0, 0);

				boundState.renderBuffers[i] = targetState.renderBuffers[i];
			}
		GlSystem::CheckErrors("qqq1_1");

		// привязать изменившийся depth-stencil буфер
		if(targetState.depthStencilBuffer != boundState.depthStencilBuffer)
		{
			DepthStencilBuffer* abstractDepthStencilBuffer = targetState.depthStencilBuffer;
			glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
				abstractDepthStencilBuffer ? fast_cast<GlDepthStencilBuffer*>(abstractDepthStencilBuffer)->GetName() : 0, 0);

			boundState.depthStencilBuffer = targetState.depthStencilBuffer;
		}

		GlSystem::CheckErrors("qqq2");
	}

	GlSystem::CheckErrors("Can't bind render buffers and depth stencil buffers");

	// текстуры
	for(int i = 0; i < ContextState::textureSlotsCount; ++i)
		if(targetState.textures[i] != boundState.textures[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			Texture* abstractTexture = targetState.textures[i];
			glBindTexture(GL_TEXTURE_2D, abstractTexture ? fast_cast<GlTexture*>(abstractTexture)->GetName() : 0);

			boundState.textures[i] = targetState.textures[i];
		}
	// семплеры
	for(int i = 0; i < ContextState::textureSlotsCount; ++i)
		if(targetState.samplerStates[i] != boundState.samplerStates[i])
		{
			SamplerState* abstractSamplerState = targetState.samplerStates[i];
			glBindSampler(i, abstractSamplerState ? fast_cast<GlSamplerState*>(abstractSamplerState)->GetName() : 0);

			boundState.samplerStates[i] = targetState.samplerStates[i];
		}
	GlSystem::CheckErrors("Can't bind textures and samplers");

	// uniform-буферы
	for(int i = 0; i < ContextState::uniformBufferSlotsCount; ++i)
		if(targetState.uniformBuffers[i] != boundState.uniformBuffers[i])
		{
			UniformBuffer* abstractUniformBuffer = targetState.uniformBuffers[i];
			glBindBufferBase(GL_UNIFORM_BUFFER, i, abstractUniformBuffer ? fast_cast<GlUniformBuffer*>(abstractUniformBuffer)->GetName() : 0);

			boundState.uniformBuffers[i] = targetState.uniformBuffers[i];
		}
	GlSystem::CheckErrors("Can't bind uniform buffers");

	// вершинный и пиксельный шейдеры
	if(targetState.vertexShader != boundState.vertexShader || targetState.pixelShader != boundState.pixelShader)
	{
		ptr<GlInternalProgram> program = programCache->GetProgram(fast_cast<GlVertexShader*>(&*targetState.vertexShader), fast_cast<GlPixelShader*>(&*targetState.pixelShader));
		if(boundProgram != program)
		{
			glUseProgram(program->GetName());
			GlSystem::CheckErrors("Can't bind program");
			boundProgram = program;
		}

		boundState.vertexShader = targetState.vertexShader;
		boundState.pixelShader = targetState.pixelShader;
	}

	// геометрия
	if(targetState.geometry != boundState.geometry)
	{
		glBindVertexArray(fast_cast<GlGeometry*>(&*targetState.geometry)->GetVertexArrayName());
		GlSystem::CheckErrors("Can't bind geometry");

		boundState.geometry = targetState.geometry;
	}

	if(targetState.fillMode != boundState.fillMode)
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
			THROW_PRIMARY_EXCEPTION("Unknown fill mode");
		}
		GlSystem::CheckErrors("Can't bind fill mode");

		boundState.fillMode = targetState.fillMode;
	}

	if(targetState.cullMode != boundState.cullMode)
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
			THROW_PRIMARY_EXCEPTION("Unknown cull mode");
		}
		GlSystem::CheckErrors("Can't bind cull mode");

		boundState.cullMode = targetState.cullMode;
	}

	if(targetState.viewportWidth != boundState.viewportWidth || targetState.viewportHeight != boundState.viewportHeight)
	{
		glViewport(0, 0, targetState.viewportWidth, targetState.viewportHeight);
		GlSystem::CheckErrors("Can't bind viewport");

		boundState.viewportWidth = targetState.viewportWidth;
		boundState.viewportHeight = targetState.viewportHeight;
	}

	if(targetState.depthTestFunc != boundState.depthTestFunc)
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
			THROW_PRIMARY_EXCEPTION("Unknown depth test func");
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(func);

		GlSystem::CheckErrors("Can't bind depth test func");

		boundState.depthTestFunc = targetState.depthTestFunc;
	}

	if(targetState.depthWrite != boundState.depthWrite)
	{
		glDepthMask(targetState.depthWrite ? GL_TRUE : GL_FALSE);

		GlSystem::CheckErrors("Can't bind depth write");

		boundState.depthWrite = targetState.depthWrite;
	}

	if(targetState.blendState != boundState.blendState)
	{
		if(targetState.blendState)
			fast_cast<GlBlendState*>(&*targetState.blendState)->Apply();

		GlSystem::CheckErrors("Can't bind blend state");

		boundState.blendState = targetState.blendState;
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
		ptr<GlInternalTexture> internalTexture = NEW(GlInternalTexture(textureName));
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
		THROW_SECONDARY_EXCEPTION("Can't get dummy render buffer", exception);
	}
}

void GlContext::ClearRenderBuffer(RenderBuffer* renderBuffer, const float* color)
{
	GLuint name = fast_cast<GlRenderBuffer*>(renderBuffer)->GetName();
	if(name)
	{
		BindServiceFramebuffer();
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, name, 0);
		glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, 0, 0);
	}
	else
	{
		if(currentFramebuffer != 0)
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer = 0);
	}

	glClearBufferfv(GL_COLOR, 0, color);
	GlSystem::CheckErrors("Can't clear render buffer");
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* abstractDepthStencilBuffer, float depth)
{
	BindServiceFramebuffer();

	GlDepthStencilBuffer* depthStencilBuffer = fast_cast<GlDepthStencilBuffer*>(abstractDepthStencilBuffer);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GetDummyRenderBuffer(depthStencilBuffer->GetWidth(), depthStencilBuffer->GetHeight())->GetName(), 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilBuffer->GetName(), 0);
	GlSystem::CheckErrors("CCCC1");
	GLenum r = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	switch(r)
	{
#define P(a) case a: printf("Framebuffer status: " #a "\n"); break
		P(GL_FRAMEBUFFER_COMPLETE);
		P(GL_FRAMEBUFFER_UNDEFINED);
		P(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
		P(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
		P(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER);
		P(GL_FRAMEBUFFER_UNSUPPORTED);
		P(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
		P(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS);
#undef P
	default:
		printf("Framebuffer status: %d\n", (int)r);
		break;
	}
	glClearBufferfv(GL_DEPTH, 0, &depth);
	GlSystem::CheckErrors("CCCC2");//"Can't clear depth");
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* abstractDepthStencilBuffer, unsigned stencil)
{
	BindServiceFramebuffer();

	GlDepthStencilBuffer* depthStencilBuffer = fast_cast<GlDepthStencilBuffer*>(abstractDepthStencilBuffer);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GetDummyRenderBuffer(depthStencilBuffer->GetWidth(), depthStencilBuffer->GetHeight())->GetName(), 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, depthStencilBuffer->GetName(), 0);
	GLint s = stencil;
	glClearBufferiv(GL_STENCIL, 0, &s);
	GlSystem::CheckErrors("Can't clear stencil");
}

void GlContext::ClearDepthStencilBuffer(DepthStencilBuffer* abstractDepthStencilBuffer, float depth, unsigned stencil)
{
	BindServiceFramebuffer();

	GlDepthStencilBuffer* depthStencilBuffer = fast_cast<GlDepthStencilBuffer*>(abstractDepthStencilBuffer);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GetDummyRenderBuffer(depthStencilBuffer->GetWidth(), depthStencilBuffer->GetHeight())->GetName(), 0);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilBuffer->GetName(), 0);
	glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);
	GlSystem::CheckErrors("Can't clear depth and stencil");
}

void GlContext::SetUniformBufferData(UniformBuffer* abstractUniformBuffer, const void* data, size_t size)
{
	GlUniformBuffer* uniformBuffer = fast_cast<GlUniformBuffer*>(&*abstractUniformBuffer);

	// проверить, что размер правильный
	if(size > uniformBuffer->GetSize())
		THROW_PRIMARY_EXCEPTION("Uniform buffer data size too big");

	// http://www.opengl.org/wiki/GLAPI/glBindBuffer
	// Сказано, что такая привязка не снимает буферы, привязанные
	// через glBindBufferBase. Верим, но надо проверить.
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer->GetName());
	void* bufferData = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	GlSystem::CheckErrors("Can't map uniform buffer");

	memcpy(bufferData, data, size);

	glUnmapBuffer(GL_UNIFORM_BUFFER);
	GlSystem::CheckErrors("Can't unmap uniform buffer");
}

void GlContext::Draw()
{
	Update();

	fast_cast<GlGeometry*>(&*boundState.geometry)->IssueDraw();
	GlSystem::CheckErrors("Can't draw");
}

void GlContext::DrawInstanced(int instancesCount)
{
	Update();

	fast_cast<GlGeometry*>(&*boundState.geometry)->IssueDrawInstanced(instancesCount);
	GlSystem::CheckErrors("Can't draw instanced");
}
