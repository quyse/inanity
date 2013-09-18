#include "Reporter.hpp"

#ifdef ___INANITY_PROFILING

#include "../Time.hpp"
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <stack>

BEGIN_INANITY_PROFILE

double Ticks2Time(long long ticks)
{
	static double coef = 1000.0 / Time::GetTicksPerSecond();
	return double(ticks) * coef;
}

Reporter::Reporter(Profiler::ThreadChunkIterator threadChunkIterator, std::ostream& stream)
: threadChunkIterator(threadChunkIterator), stream(stream)
{}

void Reporter::RawReport()
{
	int threadNumber = 0;
	for(Profiler::ThreadChunkIterator i = threadChunkIterator; i; ++i)
	{
		stream << "Thread " << (++threadNumber) << '\n';
		for(Profiler::RecordIterator j = Profiler::RecordIterator(*i); j; ++j)
		{
			Profiler::Record* record = *j;
			stream << "  ";
#define T(t) case recordType##t: stream << #t; break
			switch(record->type)
			{
			T(Start);
			T(Stop);
			T(Point);
			T(ScopeEnter);
			T(ScopeLeave);
			}
#undef T
			stream << ' ' << record->time << ' ' << record->note << '\n';
		}
	}
}

void Reporter::Report()
{
	// print header
	stream <<
		"<!doctype html>\n"
		"<html>\n"
		"\t<head>\n"
		"\t\t<title>Inanity Profile Report</title>\n"
		"\t</head>\n"
		"\t<body>\n"
		"\t\t<h1>Inanity Profile Report</h1>\n";

	stream << "\t\t<h2>Points Rating</h2>\n";
	ReportPoints();

	stream << "\t\t<h2>Scopes Rating</h2>\n";
	ReportScopes();

	// print footer
	stream <<
		"\t</body>\n"
		"</html>\n";
}

void Reporter::ReportPoints()
{
	std::vector<const char*> points;
	for(Profiler::ThreadChunkIterator i = threadChunkIterator; i; ++i)
	{
		for(Profiler::RecordIterator j = Profiler::RecordIterator(*i); j; ++j)
		{
			Profiler::Record* record = *j;
			if(record->type == recordTypePoint)
				points.push_back(record->note);
		}
	}

	std::sort(points.begin(), points.end());

	std::vector<std::pair<int, const char*> > pointsRating;
	for(size_t i = 0; i < points.size(); )
	{
		size_t j;
		for(j = i + 1; j < points.size() && points[i] == points[j]; ++j);
		pointsRating.push_back(std::pair<int, const char*>(j - i, points[i]));
		i = j;
	}
	std::sort(pointsRating.begin(), pointsRating.end(), std::greater<std::pair<int, const char*> >());

	// printing
	stream <<
		"\t\t<table>\n"
		"\t\t\t<tr>\n"
		"\t\t\t\t<th>Rating</th>\n"
		"\t\t\t\t<th>Hits</th>\n"
		"\t\t\t\t<th>Note</th>\n"
		"\t\t\t</tr>\n";
	for(size_t i = 0; i < pointsRating.size(); ++i)
		stream <<
			"\t\t\t<tr>\n"
			"\t\t\t\t<td>" << (i + 1) << "</td>\n"
			"\t\t\t\t<td>" << pointsRating[i].first << "</td>\n"
			"\t\t\t\t<td>" << pointsRating[i].second << "</td>\n"
			"\t\t\t</tr>\n";
	stream <<
		"\t\t</table>\n";
}

void Reporter::ReportScopes()
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
				stream <<
					"\t\t\t<tr>\n"
					"\t\t\t\t<td>";
				for(int i = 0; i < level; ++i)
					stream << "*&nbsp;&nbsp;&nbsp;";
				stream <<
					states[u].position << "</td>\n"
					"\t\t\t\t<td>" << states[u].hitCount << "</td>\n"
					"\t\t\t\t<td>" << Ticks2Time(states[u].totalTime) << "</td>\n"
					"\t\t\t\t<td>" << Ticks2Time(states[u].ownTime) << "</td>\n"
					"\t\t\t\t<td>" << (Ticks2Time(states[u].ownTime) / states[u].hitCount) << "</td>\n"
					"\t\t\t</tr>\n";
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
	for(Profiler::ThreadChunkIterator i = threadChunkIterator; i; ++i)
		for(Profiler::RecordIterator j = Profiler::RecordIterator(*i); j; ++j)
		{
			Profiler::Record* record = *j;

			switch(record->type)
			{
			case recordTypeScopeEnter:
				// запомнить, когда начался scope
				currentStackEnterTimes.push(record->time);
				// добавить уровень в стеке
				currentStackState = trie.Go(currentStackState, record->note);
				break;
			case recordTypeScopeLeave:
				{
					long long time = record->time - currentStackEnterTimes.top();
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
	stream.precision(3);

	stream <<
		"\t\t<table>\n"
		"\t\t\t<tr>\n"
		"\t\t\t\t<th>Note</th>\n"
		"\t\t\t\t<th>HitCount</th>\n"
		"\t\t\t\t<th>Total, ms</th>\n"
		"\t\t\t\t<th>Own, ms</th>\n"
		"\t\t\t\t<th>AvgOwn, ms</th>\n"
		"\t\t\t</tr>\n";
	trie.Print(stream, 0);
	stream <<
		"\t\t</table>\n";
}

END_INANITY_PROFILE

#endif
