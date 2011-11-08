#include "GeometryFormat.hpp"
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
