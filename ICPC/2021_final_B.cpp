#include <stdio.h>
#include <stdint.h>

#include "2021_final_B.h"

#define MAX_COUNT_ROOM (2001)

using namespace std;

int32_t n; // NOTE: # of rooms
int32_t q; // NOTE: # of scenarios to consider

int32_t s, k, t; // NOTE: start, key, trap
int32_t t_parent_from_s;

int64_t flag_corridors[MAX_COUNT_ROOM][MAX_COUNT_ROOM];

int rec_search_from_s(int num_parent_room, int num_current_room, int64_t* sum_current_weight)
{
	if (num_current_room == t)
	{
		t_parent_from_s = num_parent_room;
		return 1;
	}

	int flag_kt_from_child = 0;
	int flag_kt = (num_current_room == k) << 1;
	int64_t sum_child = 0;

	for (int i = 1; i <= n; ++i)
	{
		if (i == num_parent_room || flag_corridors[num_current_room][i] == 0)
			continue;

		sum_child = 0;

		*sum_current_weight += flag_corridors[num_current_room][i];
		flag_kt_from_child = rec_search_from_s(num_current_room, i, &sum_child);

		if (!(flag_kt_from_child & 1))
			*sum_current_weight += flag_corridors[num_current_room][i];

		flag_kt |= flag_kt_from_child;
		*sum_current_weight += sum_child;
	}

	return flag_kt;
}

int64_t rec_search_from_t(int num_parent_room, int num_current_room)
{
	int64_t sum_weight = 0;

	for (int i = 1; i <= n; ++i)
	{
		if (i == num_parent_room ||
			flag_corridors[num_current_room][i] == 0
			)
		{
			continue;
		}

		sum_weight += flag_corridors[num_current_room][i];
		sum_weight += rec_search_from_t(num_current_room, i);
		sum_weight += flag_corridors[num_current_room][i];
	}

	return sum_weight;
}

int64_t search_from_t()
{
	int64_t sum_weight = 0;
	int64_t max_weight = 0;
	int64_t child_weight = 0;

	for (int i = 1; i <= n; ++i)
	{
		if (i == t ||
			i == t_parent_from_s ||
			flag_corridors[t][i] == 0
		)
		{
			continue;
		}

		child_weight = 0;
		child_weight += flag_corridors[t][i];
		child_weight += rec_search_from_t(t, i);
		child_weight += flag_corridors[t][i];

		if (max_weight == 0 || child_weight >= max_weight)
		{
			sum_weight += max_weight;
			max_weight = child_weight;
		}
		else
		{
			sum_weight += child_weight;
		}
	}

	sum_weight = max_weight / 2;
	return sum_weight;
}

int64_t search()
{
	int64_t sum_weight = 0;

	if (rec_search_from_s(s, s, &sum_weight) == 3)
	{
		sum_weight += search_from_t();
		return sum_weight;
	}
	else
	{
		return -1;
	}
}

int problem_main(int argv, char* argc[])
{
	int u, v;
	int64_t w; // NOTE: roomA, roomB, cost to move between A and B
	
	(void)scanf("%d %d", &n, &q);

	for (int i = 0; i < n - 1; ++i)
	{
		(void)scanf("%d %d %lld", &u, &v, &w);
		flag_corridors[u][v] = w;
		flag_corridors[v][u] = w;
	}

	for (int i = 0; i < q; ++i)
	{
		(void)scanf("%d %d %d", &s, &k, &t);

		int64_t sum_weight = search();

		if (sum_weight < 0)
			printf("impossible\n");
		else
			printf("%lld\n", sum_weight);
	}

	return 0;
}