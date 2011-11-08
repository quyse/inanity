#ifndef ___INANITY_INPUT_RAW_MANAGER_HPP___
#define ___INANITY_INPUT_RAW_MANAGER_HPP___

#include "Manager.hpp"

BEGIN_INANITY_INPUT

/// Класс менеджера ввода, использующий для получения ввода Windows Raw Input.
class RawManager : public Manager
{
private:
	/// Размер буфера для буферизованного ввода.
	static const int deviceBufferSize = 32;

public:
	RawManager(HWND hWnd);

	bool ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);
};

END_INANITY_INPUT

#endif
