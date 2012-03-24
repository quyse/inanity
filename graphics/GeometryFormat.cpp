#include "GeometryFormat.hpp"
#include "../FileInputStream.hpp"
#include "../File.hpp"
#include "../StreamReader.hpp"
#include "../StreamWriter.hpp"
#include "../Exception.hpp"

GeometryFormat::GeometryFormat(const std::vector<std::pair<String, GeometrySemantic> >& semantics) :
	semantics(semantics)
{
}

const GeometrySemantic& GeometryFormat::GetSemantic(const String& name) const
{
	//найти нужную семантику перебором
	for(unsigned i = 0; i < semantics.size(); ++i)
		if(semantics[i].first == name)
			return semantics[i].second;
	THROW_PRIMARY_EXCEPTION("Can't get semantic " + name);
}

ptr<GeometryFormat> GeometryFormat::Deserialize(ptr<File> file, ResourceLoader* resourceLoader)
{
	ptr<StreamReader> reader = NEW(StreamReader(NEW(FileInputStream(file))));

	std::vector<std::pair<String, GeometrySemantic> > semantics;
	for(;;)
	{
		std::pair<String, GeometrySemantic> semantic;
		semantic.first = reader->ReadString();
		if(semantic.first.empty())
			break;
		reader->Read(&semantic.second, sizeof(semantic.second));
		semantics.push_back(semantic);
	}

	return NEW(GeometryFormat(semantics));
}

void GeometryFormat::Serialize(ptr<OutputStream> outputStream)
{
	ptr<StreamWriter> writer = NEW(StreamWriter(outputStream));

	for(size_t i = 0; i < semantics.size(); ++i)
	{
		writer->WriteString(semantics[i].first);
		writer->Write(&semantics[i].second, sizeof(semantics[i].second));
	}
	writer->WriteString(String());
}
