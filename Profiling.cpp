#include "Profiling.hpp"

#ifdef ___INANITY_PROFILING

#include "Time.hpp"
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <stack>

BEGIN_INANITY

namespace Profiling
{

// Профилирование по умолчанию выключено.
bool profiling = false;

long long recordTimes[PROFILE_MAXIMUM_RECORDS_COUNT];
RecordType recordTypes[PROFILE_MAXIMUM_RECORDS_COUNT];
const char* recordPositions[PROFILE_MAXIMUM_RECORDS_COUNT];
size_t recordsCount = 0;

void Start()
{
	if(!profiling)
	{
		profiling = true;
		Record(recordTypeStart, "Manual start");
	}
}

void Stop()
{
	if(profiling)
	{
		Record(recordTypeStop, "Manual stop");
		profiling = false;
	}
}

void Reset()
{
	recordsCount = 0;
}

double Ticks2Time(long long ticks)
{
	static double coef = 1.0 / Time::GetTicksPerSecond();
	return double(ticks) * coef;
}

void ReportScopes(std::ostream& stream);

void Report(std::ostream& stream)
{
	// красивый заголовочек
	stream << "INANITY PROFILING REPORT\n";
	stream << "Collected " << recordsCount << " profiling records of possible " << PROFILE_MAXIMUM_RECORDS_COUNT <<
		" (" << (recordsCount * 100 / PROFILE_MAXIMUM_RECORDS_COUNT) << " %)\n";

	// сделать рейтинг точек профайлинга
	{
		std::vector<const char*> points;
		for(size_t i = 0; i < recordsCount; ++i)
			if(recordTypes[i] == recordTypePoint)
				points.push_back(recordPositions[i]);
		std::sort(points.begin(), points.end());
		std::vector<std::pair<int, const char*> > pointsRating;
		for(size_t i = 0; i < points.size();)
		{
			size_t j;
			for(j = i + 1; j < points.size() && points[i] == points[j]; ++j);
			pointsRating.push_back(std::pair<int, const char*>(j - i, points[i]));
			i = j;
		}
		std::sort(pointsRating.begin(), pointsRating.end(), std::greater<std::pair<int, const char*> >());
		stream << "Rating of profile points:\n";
		for(size_t i = 0; i < pointsRating.size(); ++i)
			stream << pointsRating[i].second << ": " << pointsRating[i].first << "\n";
	}

	// сделать рейтинг областей видимости
	ReportScopes(stream);
}

void ReportScopes(std::ostream& stream)
{
	/// Состояние стека.
	struct State
	{
		/// Индекс родителя.
		int parent;
		/// Индекс на состояние - суффикс этого состояния (суффиксная ссылка).
		int suffix;
		/// Индекс следующего состояния среди потомков родителя.
		int nextSibling;
		/// Индекс первого дочернего состояния.
		int firstChild;

		/// Строка, соответствующая последнему scope в стеке.
		const char* position;
		/// Общее время вызова.
		long long totalTime;
		/// Собственное время.
		long long ownTime;
		/// Количество вызовов.
		int hitCount;

		State() : parent(-1), suffix(-1), nextSibling(-1), firstChild(-1), position(0), totalTime(0), ownTime(0), hitCount(0) {}
	};

	/// Дерево состояний.
	struct StateTrie
	{
		std::vector<State> states;

		struct AverageOwnTimeSorter
		{
			StateTrie* trie;
			bool operator()(int a, int b) const
			{
				return trie->states[a].ownTime * trie->states[b].hitCount < trie->states[b].ownTime * trie->states[a].hitCount;
			}
		};
		AverageOwnTimeSorter averageOwnTimeSorter;

		StateTrie() : states(1)
		{
			averageOwnTimeSorter.trie = this;
		}

		int Go(int u, const char* position)
		{
			for(int i = states[u].firstChild; i >= 0; i = states[i].nextSibling)
				if(states[i].position == position)
					return i;
			int z = (int)states.size();
			states.push_back(State());
			states[z].parent = u;
			states[z].nextSibling = states[u].firstChild;
			states[u].firstChild = z;
			states[z].suffix = u > 0 ? Go(GetSuffix(u), position) : 0;
			states[z].position = position;
			return z;
		}

		/// Получить суффикс состояния.
		/** \param u Не может быть 0. */
		int GetSuffix(int u)
		{
			if(states[u].suffix < 0 && u > 0)
				states[u].suffix = states[u].parent > 0 ? Go(GetSuffix(states[u].parent), states[u].position) : 0;
			return states[u].suffix;
		}

		/// Расставить время, занимаемое вложенными вызовами.
		void CalculateOwnTimes(int u)
		{
			states[u].ownTime = states[u].totalTime;
			for(int i = states[u].firstChild; i >= 0; i = states[i].nextSibling)
			{
				states[u].ownTime -= states[i].totalTime;
				CalculateOwnTimes(i);
			}
		}

		/// Отсортировать потомков состояния по собственному времени.
		void SortByAverageOwnTime(int u)
		{
			static std::vector<int> tmp;
			tmp.clear();
			for(int i = states[u].firstChild; i >= 0; i = states[i].nextSibling)
				tmp.push_back(i);
			std::sort(tmp.begin(), tmp.end(), averageOwnTimeSorter);
			states[u].firstChild = -1;
			for(int i = 0; i < (int)tmp.size(); ++i)
			{
				int t = tmp[i];
				states[t].nextSibling = states[u].firstChild;
				states[u].firstChild = t;
			}
			for(int i = states[u].firstChild; i >= 0; i = states[i].nextSibling)
				SortByAverageOwnTime(i);
		}

		void Print(std::ostream& stream, int u, int level = 0)
		{
			// root печатать не надо
			if(u > 0)
			{
				for(int i = 0; i < level; ++i)
					stream << "  ";
				stream << states[u].position << " hitcount=" << states[u].hitCount << " total=" << Ticks2Time(states[u].totalTime) << " own=" << Ticks2Time(states[u].ownTime) << " avgown=" << (Ticks2Time(states[u].ownTime) / states[u].hitCount) << '\n';
			}

			if(u > 0)
				++level;
			for(int i = states[u].firstChild; i >= 0; i = states[i].nextSibling)
				Print(stream, i, level);
		}
	};

	// бор состояний
	StateTrie trie;
	// текущий стек
	std::stack<long long> currentStackEnterTimes;
	// текущее состояние стека
	size_t currentStackState = 0;
	// цикл по записям
	for(size_t i = 0; i < recordsCount; ++i)
	{
		const char* recordPosition = recordPositions[i];
		long long recordTime = recordTimes[i];
		switch(recordTypes[i])
		{
		case recordTypeScopeEnter:
			// запомнить, когда начался scope
			currentStackEnterTimes.push(recordTime);
			// добавить уровень в стеке
			currentStackState = trie.Go(currentStackState, recordPosition);
			break;
		case recordTypeScopeLeave:
			{
				long long time = recordTime - currentStackEnterTimes.top();
				currentStackEnterTimes.pop();
				// добавить время и вызов ко всем scope
				for(int i = currentStackState; i >= 0; i = trie.GetSuffix(i))
				{
					trie.states[i].totalTime += time;
					trie.states[i].hitCount++;
				}
				// перейти назад
				currentStackState = trie.states[currentStackState].parent;
			}
			break;
		default:
			break;
		}
	}

	// расставить собственные времена
	trie.CalculateOwnTimes(0);
	// отсортировать по среднему собственному времени
	trie.SortByAverageOwnTime(0);
	// вывести всё
	stream << std::fixed;
	stream.precision(6);
	trie.Print(stream, 0);
}

}

END_INANITY

#endif
