#ifndef ___INANITY_LUA_STATE_DECL_HPP___
#define ___INANITY_LUA_STATE_DECL_HPP___

#include "Function_decl.hpp"
#include "lualib.hpp"

BEGIN_INANITY_LUA

/// Класс, инкапсулирующий состояние Lua.
class State : public Object
{
private:
	lua_State* state;

private:
	/// Каждая открытая userdata является этой структурой.
	struct UserData
	{
		/// Тип userdata.
		enum Type
		{
			typeClass,
			typeObject
		} type;
	};

	/// Структура объекта, представляющего класс в Lua.
	struct ClassUserData : public UserData
	{
		Class* cls;
	};

	/// Структура объекта, представляющего управляемый указатель в Lua.
	struct ObjectUserData : public UserData
	{
		Object* object;
		Class* cls;
	};

	/// Структура объекта, представляющего метод.
	struct MethodUserData
	{
		Method method;
	};

	/// Обработчик выделения памяти Lua.
	static void* Alloc(void* self, void* ptr, size_t osize, size_t nsize);

	// Обработчики объектов.
	static int ObjectIndexed(lua_State* state);
	static int ObjectCollected(lua_State* state);

	// Обработчик делегата.
	static int DelegateCalled(lua_State* state);
	// Обработчик делегата-метода.
	static int MethodDelegateCalled(lua_State* state);

	/// Вызвать метод, применив его к объекту.
	/** В стеке Lua должны лежать аргументы. */
	int CallMethod(Object* object, Method method);

	/// Зарегистрировать класс.
	void Register(Class* cls);
	/// Запихать в стек объект.
	/** Относительно медленный метод, так как создаётся fulluserdata. */
	void PushObject(Object* object, Class* cls);
	/// Запихать в стек делегат.
	/** Предполагается, в стеке лежит объект и имя метода; они будут замены на делегат.
	 * \param method флаг, нужно ли удалять первый аргумент (self) из стека. */
	void PushDelegate(bool method = false);
	/// Запихать в стек делегат.
	/** Относительно медленный метод, так как объект запихивается PushObject. */
	void PushDelegate(Object* object, Class* cls, const char* methodName);
	/// Установить глобальный объект.
	void SetGlobal(const char* name, Object* object, Class* cls);

public:
	State();
	~State();

	/// Загрузить скрипт из файла.
	ptr<Function> LoadScript(ptr<File> file);
	/// Указать глобальный объект.
	template <typename T>
	void SetGlobal(const char* name, ptr<T> object)
	{
		SetGlobal(name, static_cast<Object*>((T*)object), &T::scriptClass);
	}
};

END_INANITY_LUA

#endif
