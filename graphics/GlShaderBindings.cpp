#include "GlShaderBindings.hpp"
#include "../StreamWriter.hpp"
#include "../StreamReader.hpp"

BEGIN_INANITY_GRAPHICS

GlShaderBindings::GlShaderBindings(
	const Bindings& uniformBlockBindings,
	const Bindings& samplerBindings,
	const Bindings& attributeBindings
) :
	uniformBlockBindings(uniformBlockBindings),
	samplerBindings(samplerBindings),
	attributeBindings(attributeBindings)
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

void GlShaderBindings::Serialize(StreamWriter& writer)
{
	// записать привязки uniform-блоков
	WriteBindings(writer, uniformBlockBindings);
	// записать привязки семплеров
	WriteBindings(writer, samplerBindings);
	// записать привязки
	WriteBindings(writer, attributeBindings);
}

ptr<GlShaderBindings> GlShaderBindings::Deserialize(StreamReader& reader)
{
	Bindings uniformBlockBindings, samplerBindings, attributeBindings;
	ReadBindings(reader, uniformBlockBindings);
	ReadBindings(reader, samplerBindings);
	ReadBindings(reader, attributeBindings);

	return NEW(GlShaderBindings(uniformBlockBindings, samplerBindings, attributeBindings));
}

END_INANITY_GRAPHICS
