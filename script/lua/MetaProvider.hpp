#ifndef ___INANITY_SCRIPT_LUA_META_PROVIDER_HPP___
#define ___INANITY_SCRIPT_LUA_META_PROVIDER_HPP___

#include "lua.hpp"
#include "../../meta/ClassBase.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

class MetaProvider
{
private:
	class CallableBase
	{
	public:
		virtual void PushThunk(lua_State* luaState) = 0;
	};

	class NamedCallableBase : public CallableBase
	{
	private:
		const char* name;

	public:
		NamedCallableBase(const char* name);

		const char* GetName() const;
	};

public:
	typedef CallableBase ConstructorBase;
	typedef NamedCallableBase FunctionBase;
	typedef NamedCallableBase MethodBase;

	template <typename ClassType, typename... Args>
	class Constructor : public ConstructorBase
	{
	public:
		void PushThunk(lua_State* luaState);
	};

	template <typename FunctionType, FunctionType function>
	class Function : public FunctionBase
	{
	public:
		Function(const char* name);

		void PushThunk(lua_State* luaState);
	};

	template <typename MethodType, MethodType method>
	class Method : public MethodBase
	{
	public:
		Method(const char* name);

		void PushThunk(lua_State* luaState);
	};

	struct Traits
	{
		typedef MetaProvider::ConstructorBase ConstructorBase;
		typedef MetaProvider::FunctionBase FunctionBase;
		typedef MetaProvider::MethodBase MethodBase;
		typedef Meta::ClassBase<Traits> ClassBase;
	};

	typedef Meta::ClassBase<Traits> ClassBase;

	template <typename ClassType>
	class Class : public ClassBase
	{
	public:
		Class(const char* name, const char* fullName);
	};
};

END_INANITY_LUA

#endif
