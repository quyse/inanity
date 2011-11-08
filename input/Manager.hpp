#ifndef ___INANITY_INPUT_MANAGER_HPP___
#define ___INANITY_INPUT_MANAGER_HPP___

#include "input.hpp"
#include "../CriticalSection.hpp"

BEGIN_INANITY_INPUT

/// Абстрактный класс менеджера ввода.
/** Менеджер ввода позволяет получать ввод, непосредственно (с помощью методов
получения текущих координат мыши, к примеру), или буферизованно (получая список
произошедших событий ввода). Менеджер ввода работает покадрово, переходя к следующему
кадру с помощью метода Update. В течение кадра списки событий и непосредственные
значения не меняются.
Насчет потоковой безопасности. Допускается, чтобы во внутренний кадр данные добавлял
другой поток (с помощью метода AddEvent). Но все остальные операции
(обновление, работа с текущим кадром) должны выполняться только основным потоком.
*/
class Frame;
struct Event;
class Manager : public Object
{
protected:
	/// Окно, к которому привязан менеджер.
	HWND hWnd;
	/// Критическая секция.
	/** Предназначена для синхронизации, в случае если подкласс будет получать
	данные из другого потока. */
	CriticalSection criticalSection;

	/// Текущий кадр.
	/** Данный кадр - последний завершенный кадр. */
	ptr<Frame> currentFrame;
	/// Текущий внутренний кадр.
	/** Этот кадр не завершен, он в процессе. Содержит самые последние данные,
	но пользователю он не доступен. */
	ptr<Frame> internalFrame;

	Manager(HWND hWnd);

	/// Добавить событие.
	void AddEvent(const Event& e);

public:
	//обработать оконное сообщение.
	virtual bool ProcessWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	//перейти к следующему кадру
	virtual void Update();
	/// Получить текущий кадр.
	ptr<Frame> GetCurrentFrame();

	/// Выполнить обработку активации окна.
	virtual void AcquireDevices();
	/// Выполнить обработку деактивации окна.
	virtual void UnacquireDevices();
};

END_INANITY_INPUT

#endif
