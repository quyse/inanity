#include "Profiling.hpp"

#ifdef ___INANITY_PROFILING

#include "Time.hpp"
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <vector>

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
			pointsRating.push_back(std::pair<int, const char*>((int)(j - i), points[i]));
			i = j;
		}
		std::sort(pointsRating.begin(), pointsRating.end(), std::greater<std::pair<int, const char*> >());
		stream << "Rating of profile points:\n";
		for(size_t i = 0; i < pointsRating.size(); ++i)
			stream << pointsRating[i].second << ": " << pointsRating[i].first << "\n";
	}

	// сделать рейтинг областей видимости - пока простой

	// общее время каждого scope
	std::unordered_map<const char*, long long> scopeTotalTime;
	// последнее начало каждого scope
	std::unordered_map<const char*, long long> scopeLastEnterTime;
	for(size_t i = 0; i < recordsCount; ++i)
		switch(recordTypes[i])
		{
		case recordTypeScopeEnter:
			scopeLastEnterTime[recordPositions[i]] = recordTimes[i];
			break;
		case recordTypeScopeLeave:
			scopeTotalTime[recordPositions[i]] += recordTimes[i] - scopeLastEnterTime[recordPositions[i]];
			break;
		default:
			break;
		}
	std::vector<std::pair<long long, const char*> > scopes;
	scopes.reserve(scopeTotalTime.size());
	for(std::unordered_map<const char*, long long>::const_iterator i = scopeTotalTime.begin(); i != scopeTotalTime.end(); ++i)
		scopes.push_back(std::pair<long long, const char*>(i->second, i->first));
	std::sort(scopes.begin(), scopes.end(), std::greater<std::pair<long long, const char*> >());

	double coef = 1.0 / Time::GetTicksPerSecond();
	stream << "Top-10 of most timing scopes:\n";
	stream << std::fixed;
	stream.precision(6);
	for(size_t i = 0; i < 10 && i < scopes.size(); ++i)
		stream << scopes[i].second << " === " << (double(scopes[i].first) * coef) << " sec\n";
}

}

END_INANITY

#endif
