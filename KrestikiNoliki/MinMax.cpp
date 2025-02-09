#include "pch.h"
#include "MinMax.h"


#define DRAW 1000
inline int FindWinner(vector<int>& data, int zeros)
{
	if ((data[0] == data[1]) && (data[0] == data[2]) && (data[0] != 0))return data[0];
	if ((data[3] == data[4]) && (data[3] == data[5]) && (data[3] != 0))return data[3];
	if ((data[6] == data[7]) && (data[6] == data[8]) && (data[6] != 0))return data[6];

	if ((data[0] == data[3]) && (data[0] == data[6]) && (data[0] != 0))return data[0];
	if ((data[1] == data[4]) && (data[1] == data[7]) && (data[1] != 0))return data[1];
	if ((data[2] == data[5]) && (data[2] == data[8]) && (data[2] != 0))return data[2];

	if ((data[0] == data[4]) && (data[0] == data[8]) && (data[0] != 0))return data[0];
	if ((data[2] == data[4]) && (data[2] == data[6]) && (data[2] != 0))return data[2];

	if (zeros == 0)return DRAW;
	return 0;
}

int Min(vector<int>& p)
{
	int res = 0;
	bool move = true;
	if (p[0] != INT_MAX)move = false;

	for (int i = 1; i < p.size(); i++)
	{
		if (p[i] == INT_MAX)
		{
			if (move)res++;
			continue;
		}
		else
		{
			if (move)
			{
				res++;
				move = !move;
			}
		}
		if (p[res] > p[i])res = i;
	}
	return res;
}
int Max(vector<int>& p)
{
	int res = 0;
	bool move = true;
	if (p[0] != INT_MAX)move = false;
	for (int i = 1; i < p.size(); i++)
	{
		if (p[i] == INT_MAX)
		{
			if (move)res++;
			continue;
		}
		else
		{
			if (move)
			{
				res++;
				move = !move;
			}
		}
		if (p[res] < p[i])res = i;
	}
	return res;
}
int g_id = 0;
int minmax(std::vector<int> data, int zeroes, int turn, bool bot, int deep)
{
	int winner = FindWinner(data, zeroes);
	if (bot)
	{
		if (winner == 1)return 10 - deep;
		else
			if (winner == -1)return -10 + deep;
	}
	else
	{
		if (winner == -1)return 10 - deep;
		else
			if (winner == 1)return -10 + deep;
	}
	if (winner == DRAW)return deep;


	vector<int> properties(9, INT_MAX);
	for (int i = 0; i < 9; i++)
	{
		if (data[i] == 0)
		{
			vector<int> temp = data;
			temp[i] = turn;
			int minmaxres = minmax(temp, zeroes - 1, -turn, bot, deep + 1);
			properties[i] = minmaxres;
		}
	}

	if (((turn == 1) && (bot)) || ((turn == -1) && (bot == false)))
	{
		/*int min = Min(properties);
		if (properties[min] == -8)g_id = min;
		else g_id = Max(properties);*/
		g_id = Max(properties);
	}
	else
		if (((turn == -1) && (bot)) || ((turn == 1) && (bot == false)))
		{
			/*int max = Max(properties);
			if (properties[max] == 8)g_id = max;
			else g_id = Min(properties);*/
			g_id = Min(properties);
		}

	return properties[g_id];
}

int minmaxres()
{
	return g_id;
}
