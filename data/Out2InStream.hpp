#ifndef ___INANITY_DATA_OUT2IN_STREAM_HPP___
#define ___INANITY_DATA_OUT2IN_STREAM_HPP___

#include "data.hpp"
#include "../OutputStream.hpp"
#include "../InputStream.hpp"
#include "../CriticalSection.hpp"
#include "../Semaphore.hpp"
#include <queue>
#include <list>

BEGIN_INANITY_DATA

/// Поток выход-типа-вход.
/** Специальный класс, который преобразует поток активных
 * данных в поток активного считывания.
 * Является выходным потоком (OutputStream), то есть в него можно свободно
 * записывать данные. Для того, чтобы считывать данные, нужно получить
 * один или несколько InputStream, которые независимо друг от друга
 * могут считывать данные.
 * Класс потоковобезопасный. */
class Out2InStream : public OutputStream
{
private:
	/// Считыватель данных.
	/** Считыватель может быть создан в другом потоке, но
	 * затем более одного обращения к Read одновременно
	 * происходить не должно. */
	class Reader : public InputStream
	{
	private:
		/// Ссылка на поток.
		ptr<Out2InStream> stream;
		/// Итератор в списке считывателей.
		std::list<Reader*>::iterator iterator;
		/// Очередь файлов на выход.
		std::queue<ptr<File> > files;
		/// Семафор на очередь файлов.
		Semaphore semaphore;
		/// Критическая секция на очередь файлов.
		CriticalSection criticalSection;
		/// Смещение в первом файле, то есть сколько байт в нём уже пройдено.
		size_t firstOffset;

	public:
		Reader(ptr<Out2InStream> stream);
		~Reader();
		size_t Read(void* data, size_t size);
		/// Запихать данные в очередь.
		/** Если file = 0, это означает, что данные закончились. */
		void Push(ptr<File> file);
	};

	/// Критическая секция для внутренних переменных.
	CriticalSection criticalSection;
	/// Список выданных считывателей.
	std::list<Reader*> readers;
	/// Закончен ли поток.
	bool flushed;

public:
	/// Создать поток.
	Out2InStream();

	/// Записать файл с данными.
	void Write(ptr<File> file);
	/// Отметить конец данных.
	void End();

	/// Создать считыватель данных.
	/** Считыватель данных считывает поток данных, который начинается
	 * с тех данных, которые будут записаны после этой точки.
	 * Считыватель данных работает по обычным правилам InputStream,
	 * но нужно учитывать, что данные могут возвращаться долго
	 * (это зависит от того, когда их запишут в Out2InStream). */
	ptr<InputStream> CreateInputStream();
};

END_INANITY_DATA

#endif
