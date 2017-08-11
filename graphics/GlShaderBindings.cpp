#include "GlShaderBindings.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"

BEGIN_INANITY_GRAPHICS

GlShaderBindings::GlShaderBindings(
	const UniformBindings& uniformBindings,
	const Bindings& uniformBlockBindings,
	const Bindings& samplerBindings,
	const Bindings& attributeBindings,
	const Bindings& targetBindings,
	bool dualFragmentTarget
) :
	uniformBindings(uniformBindings),
	uniformBlockBindings(uniformBlockBindings),
	samplerBindings(samplerBindings),
	attributeBindings(attributeBindings),
	targetBindings(targetBindings),
	dualFragmentTarget(dualFragmentTarget)
{
}

void GlShaderBindings::WriteBindings(StreamWriter& writer, const Bindings& bindings)
{
	writer.WriteShortly(bindings.size());
	for(size_t i = 0; i < bindings.size(); ++i)
	{
		writer.WriteString(bindings[i].first);
		writer.WriteShortly(bindings[i].second);
	}
}

void GlShaderBindings::ReadBindings(StreamReader& reader, Bindings& bindings)
{
	bindings.resize(reader.ReadShortly());
	for(size_t i = 0; i < bindings.size(); ++i)
	{
		bindings[i].first = reader.ReadString();
		bindings[i].second = (int)reader.ReadShortly();
	}
}

const GlShaderBindings::UniformBindings& GlShaderBindings::GetUniformBindings() const
{
	return uniformBindings;
}

const GlShaderBindings::Bindings& GlShaderBindings::GetUniformBlockBindings() const
{
	return uniformBlockBindings;
}

const GlShaderBindings::Bindings& GlShaderBindings::GetSamplerBindings() const
{
	return samplerBindings;
}

const GlShaderBindings::Bindings& GlShaderBindings::GetAttributeBindings() const
{
	return attributeBindings;
}

const GlShaderBindings::Bindings& GlShaderBindings::GetTargetBindings() const
{
	return targetBindings;
}

bool GlShaderBindings::GetDualFragmentTarget() const
{
	return dualFragmentTarget;
}

void GlShaderBindings::Serialize(StreamWriter& writer)
{
	writer.WriteShortly(uniformBindings.size());
	for(size_t i = 0; i < uniformBindings.size(); ++i)
	{
		const UniformBinding& ub = uniformBindings[i];
		writer.WriteShortly(ub.dataType);
		writer.WriteShortly(ub.count);
		writer.WriteShortly(ub.slot);
		writer.WriteShortly(ub.offset);
		writer.WriteString(ub.name);
	}
	WriteBindings(writer, uniformBlockBindings);
	WriteBindings(writer, samplerBindings);
	WriteBindings(writer, attributeBindings);
	WriteBindings(writer, targetBindings);
	writer.WriteShortly((size_t)dualFragmentTarget);
}

ptr<GlShaderBindings> GlShaderBindings::Deserialize(StreamReader& reader)
{
	UniformBindings uniformBindings;
	uniformBindings.resize(reader.ReadShortly());
	for(size_t i = 0; i < uniformBindings.size(); ++i)
	{
		UniformBinding& ub = uniformBindings[i];
		ub.dataType = (DataType)reader.ReadShortly();
		ub.count = (int)reader.ReadShortly();
		ub.slot = (int)reader.ReadShortly();
		ub.offset = (int)reader.ReadShortly();
		ub.name = reader.ReadString();
	}

	Bindings uniformBlockBindings, samplerBindings, attributeBindings, targetBindings;
	ReadBindings(reader, uniformBlockBindings);
	ReadBindings(reader, samplerBindings);
	ReadBindings(reader, attributeBindings);
	ReadBindings(reader, targetBindings);
	bool dualFragmentTarget = !!reader.ReadShortly();

	return NEW(GlShaderBindings(uniformBindings, uniformBlockBindings, samplerBindings, attributeBindings, targetBindings, dualFragmentTarget));
}

END_INANITY_GRAPHICS
