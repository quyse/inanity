#include "GlShaderBindings.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"

BEGIN_INANITY_GRAPHICS

GlShaderBindings::GlShaderBindings(
	const Bindings& uniformBlockBindings,
	const Bindings& samplerBindings,
	const Bindings& attributeBindings,
	const Bindings& targetBindings
) :
	uniformBlockBindings(uniformBlockBindings),
	samplerBindings(samplerBindings),
	attributeBindings(attributeBindings),
	targetBindings(targetBindings)
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
		bindings[i].second = reader.ReadShortly();
	}
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

void GlShaderBindings::Serialize(StreamWriter& writer)
{
	WriteBindings(writer, uniformBlockBindings);
	WriteBindings(writer, samplerBindings);
	WriteBindings(writer, attributeBindings);
	WriteBindings(writer, targetBindings);
}

ptr<GlShaderBindings> GlShaderBindings::Deserialize(StreamReader& reader)
{
	Bindings uniformBlockBindings, samplerBindings, attributeBindings, targetBindings;
	ReadBindings(reader, uniformBlockBindings);
	ReadBindings(reader, samplerBindings);
	ReadBindings(reader, attributeBindings);
	ReadBindings(reader, targetBindings);

	return NEW(GlShaderBindings(uniformBlockBindings, samplerBindings, attributeBindings, targetBindings));
}

END_INANITY_GRAPHICS
