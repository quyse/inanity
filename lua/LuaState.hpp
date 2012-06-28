#ifndef ___INANITY_LUA_STATE_HPP___
#define ___INANITY_LUA_STATE_HPP___

/* Файл содержит общие определения, используемые для поддержки Lua.
 */

#include "../Object.hpp"
#include "../String.hpp"
#include "../Exception.hpp"
#include "lua.hpp"
#include <vector>

BEGIN_INANITY

class File;

/// Класс, инкапсулирующий состояние Lua.
class LuaState : public Object
{
private:
	lua_State* state;

public:
	/// Вспомогательный класс, с которым происходит взаимодействие при выполнении Inanity-метода, вызванного из Lua.
	class Call
	{
		friend class LuaState;
	private:
		LuaState* state;
		int argumentsCount;
		bool returned;

		/// Создать объект.
		Call(LuaState* state);

		/// Проверить, не возвращалось ли уже значение.
		void CheckReturn();

		/// Завершить выполнение вызова.
		/** Возвращает количество возвращённых аргументов (0 или 1). */
		int Finish();

	public:
		/// Получить количество входящих аргументов.
		int GetArgumentsCount() const;
		/// Проверить, что аргументов ровно столько, сколько должно. (Метод для удобства)
		/** Если не столько, выбрасывается исключение. */
		void EnsureArgumentsCount(int count);

		bool GetBoolean(int i) const;
		int GetInteger(int i) const;
		float GetFloat(int i) const;
		double GetDouble(int i) const;
		String GetString(int i) const;
		template <typename T>
		ptr<T> GetPointer(int i) const
		{
			if(lua_isuserdata(state->state, i))
			{
				UserData* userData = (UserData*)lua_touserdata(state->state, i);
				// проверить, что это действительно объект
				if(userData->type == UserData::typeObject)
				{
					ObjectUserData* objectUserData = (ObjectUserData*)userData;
					// проверить, что это действительно объект нужного типа
					if(objectUserData->cls == &T::scriptClass)
						return (T*)objectUserData->object;
				}
				THROW_PRIMARY_EXCEPTION("Userdata argument is not an object pointer");
			}
			THROW_PRIMARY_EXCEPTION("Argument is not an userdata");
		}

		void Return(bool value);
		void Return(int value);
		void Return(float value);
		void Return(double value);
		void Return(const char* value);
		void Return(const String& value);

		template <typename T>
		void Return(ptr<T> value)
		{
			CheckReturn();
			state->PushObject(static_cast<Object*>((T*)value), &T::scriptClass);
		}

		template <typename T>
		void Return(ptr<T> value, const char* methodName)
		{
			CheckReturn();
			state->PushDelegate(static_cast<Object*>((T*)value), &T::scriptClass, methodName);
		}
	};

	/// Тип метода.
	typedef void (Object::*Method)(Call&);

	/// Класс, хранящий, каким образом Inanity-класс представляется в Lua.
	/** То есть, содержит свойства и методы, видимые в Lua. */
	class Class
	{
		friend class LuaState;
	private:
		/// Список читающих свойств.
		std::vector<std::pair<const char*, Method> > getProperties;
		/// Список записывающих свойств.
		std::vector<std::pair<const char*, Method> > setProperties;
		/// Список методов.
		std::vector<std::pair<const char*, Method> > methods;

	public:
		void AddProperty(const char* name, Method readMethod, Method writeMethod);
		void AddMethod(const char* name, Method method);
	};

	/// Класс скрипта Lua.
	/** Lua-функция скрипта хранится в глобальной таблице scripts, индексированной адресом объекта Script. */
	class Script : public Object
	{
	private:
		ptr<LuaState> state;

	public:
		/// Создать скрипт Lua.
		/** Предполагается, что при вызове в стеке Lua лежит функция скрипта. Конструктор забирает её из стека. */
		Script(ptr<LuaState> luaState);
		~Script();

		void Run();
	};

private:
	/// Каждая открытая userdata является этой структурой.
	struct UserData
	{
		/// Тип userdata.
		enum Type
		{
			typeObject
		} type;
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
	LuaState();
	~LuaState();

	/// Загрузить скрипт из файла.
	ptr<Script> LoadScript(ptr<File> file);
	/// Указать глобальный объект.
	template <typename T>
	void SetGlobal(const char* name, ptr<T> object)
	{
		SetGlobal(name, static_cast<Object*>((T*)object), &T::scriptClass);
	}
};

typedef LuaState::Call LuaCall;
typedef LuaState::Method LuaMethod;
typedef LuaState::Class LuaClass;
typedef LuaState::Script LuaScript;

END_INANITY

#endif
