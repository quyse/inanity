#ifndef ___INANITY_INPUT_WIN32_RAW_MANAGER_HPP___
#define ___INANITY_INPUT_WIN32_RAW_MANAGER_HPP___

#include "Win32Manager.hpp"

BEGIN_INANITY_INPUT

/// Класс менеджера ввода, использующий для получения ввода Windows Raw Input.
class Win32RawManager : public Win32Manager
{
private:
	/// Bound window.
	HWND hWnd;
	/// Размер буфера для буферизованного ввода.
	static const int deviceBufferSize = 32;

public:
	Win32RawManager(HWND hWnd);

	bool ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

END_INANITY_INPUT

#endif
