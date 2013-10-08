#ifndef ___INANITY_INPUT_WIN32_MANAGER_HPP___
#define ___INANITY_INPUT_WIN32_MANAGER_HPP___

#include "Manager.hpp"
#include "Key.hpp"
#include "../platform/windows.hpp"

BEGIN_INANITY_INPUT

/// Общий класс для менеджеров ввода в Windows.
class Win32Manager : public Manager
{
protected:
	/// Окно, к которому привязан менеджер.
	HWND hWnd;

protected:
	Win32Manager(HWND hWnd);

	static Key ConvertKey(USHORT key);

public:
	/// Обработать оконное сообщение.
	virtual bool ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	/// Выполнить обработку активации окна.
	virtual void AcquireDevices();
	/// Выполнить обработку деактивации окна.
	virtual void UnacquireDevices();
};

END_INANITY_INPUT

#endif
