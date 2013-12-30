#include "Namespace.hpp"
#include "wrappers.hpp"
#include "State.hpp"
#include "../../platform/NpapiPlugin.hpp"
#include "../../platform/NpapiPluginInstance.hpp"

BEGIN_INANITY_NP

Namespace::Namespace(const String& name)
: name(name), classMeta(nullptr)
{
	wrapper = static_cast<NPObjectNamespace*>(
		Platform::NpapiPlugin::browserFuncs.createobject(
			State::GetCurrent()->GetPluginInstance()->GetNpp(),
			&NPClassNamespace::instance));
	wrapper->object = this;
}

Namespace::~Namespace()
{
	Platform::NpapiPlugin::browserFuncs.releaseobject(wrapper);
}

NPObjectNamespace* Namespace::GetWrapper() const
{
	return wrapper;
}

ptr<Namespace> Namespace::OpenNamespace(const String& name)
{
	NPIdentifier identifier = Platform::NpapiPlugin::browserFuncs.getstringidentifier(name.c_str());
	Namespaces::const_iterator i = namespaces.find(identifier);
	if(i != namespaces.end())
		return i->second;

	ptr<Namespace> object = NEW(Namespace(name));
	namespaces.insert(std::make_pair(identifier, object));
	return object;
}

const Namespace::Namespaces& Namespace::GetNamespaces() const
{
	return namespaces;
}

void Namespace::SetClassMeta(MetaProvider::ClassBase* classMeta)
{
	this->classMeta = classMeta;
}

MetaProvider::ClassBase* Namespace::GetClassMeta() const
{
	return classMeta;
}

END_INANITY_NP
