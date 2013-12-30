#ifndef ___INANITY_SCRIPT_NP_NAMESPACE_HPP___
#define ___INANITY_SCRIPT_NP_NAMESPACE_HPP___

#include "np.hpp"
#include "MetaProvider.hpp"
#include "../../platform/npapi.hpp"
#include "../../String.hpp"
#include <unordered_map>

BEGIN_INANITY_NP

struct NPObjectNamespace;

/// Object represents namespaces and (or) constructors.
/** One class works for namespaces and constructors,
because of possibility to use nested names, i.e.:

var rootClass = new SomeNamespace.RootClass();
var rootNestedClass = new SomeNamespace.RootClass.NestedClass();

Class used for constructor (optional) and static methods.

*/
class Namespace : public Object
{
public:
	typedef std::unordered_map<NPIdentifier, ptr<Namespace> > Namespaces;

private:
	/// Name of the namespace/class.
	String name;

	/// Wrapper for NPObject.
	NPObjectNamespace* wrapper;

	/// Map of sub-namespaces by indentifiers.
	Namespaces namespaces;

	/// Class of the namespace.
	/** Can be null. */
	MetaProvider::ClassBase* classMeta;

public:
	Namespace(const String& name);
	~Namespace();

	NPObjectNamespace* GetWrapper() const;

	/// Get existing or create new subnamespace.
	ptr<Namespace> OpenNamespace(const String& name);
	/// Get namespaces.
	const Namespaces& GetNamespaces() const;

	void SetClassMeta(MetaProvider::ClassBase* classMeta);
	MetaProvider::ClassBase* GetClassMeta() const;
};

END_INANITY_NP

#endif
