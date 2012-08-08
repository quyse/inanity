#include "../Profiling.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

const int k = 26;
struct SA
{
	struct State
	{
		int e[k];
		int len,link,leaf;
		State(int len):len(len),link(-1),leaf(0)
		{
			std::fill(e,e+k,-1);
		}
	};
	std::vector<State> s;
	int last;
	SA() : s(1, State(0)), last(0) {}

	void extend(int c)
	{
		PROFILE_SCOPE();

		int u = (int)s.size();
		s.push_back(State(s[last].len + 1));
		int i;
		for(i = last; i >= 0 && s[i].e[c] < 0; i = s[i].link)
			s[i].e[c] = u;
		if(i < 0)
			s[u].link = 0;
		else
		{
			int t = s[i].e[c];
			if(s[t].len == s[i].len + 1)
				s[u].link = t;
			else
			{
				int z = (int)s.size();
				s.push_back(s[t]);
				s[z].len = s[i].len + 1;
				for(; i >= 0 && s[i].e[c] == t; i = s[i].link)
					s[i].e[c] = z;
				s[u].link = s[t].link = z;
			}
		}
		last = u;
	}

	void markLeaves()
	{
		PROFILE_SCOPE();

		for(int i = last; i >= 0; i = s[i].link)
			s[i].leaf = 1;
	}

	void print(int u)
	{
		PROFILE_SCOPE();

		static char str[1000];
		static int len = 0;
		if(s[u].leaf)
		{
			str[len] = 0;
			printf("%s\n", str);
		}
		for(int i = 0; i < k; ++i)
			if(s[u].e[i] >= 0)
			{
				str[len++] = i + 'a';
				print(s[u].e[i]);
				--len;
			}
	}
};

void run()
{
	PROFILE_SCOPE();

	SA a;
	const char pat[] = "abacaba";
	const int cnt = 10;
	char str[cnt * (sizeof(pat) - 1) + 1];
	for(int i = 0; i < cnt; ++i)
		memcpy(str + i * (sizeof(pat) - 1), pat, sizeof(pat));
	str[cnt * (sizeof(pat) - 1)] = 0;
	for(int i = 0; str[i]; ++i)
		a.extend(str[i] - 'a');
	a.markLeaves();
	a.print(0);
}

int main()
{
	Profiling::Start();
	run();
	Profiling::Stop();
	Profiling::Report(std::cout);
	return 0;
}
