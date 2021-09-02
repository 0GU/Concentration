#include "sort.h"
using namespace std;
void Shaker_Sort(int* data)
{
	int save = 0;
	int rank_data[4];
	int change = 0;
	//ƒf[ƒ^‚Ì“o˜^
	for (int i = 0; i < 4; i++)
	{
		rank_data[i] = data[i];
	}
	do
	{
		change = 0;
		for (int i = 0; i < 3; i++)
		{
			if (rank_data[i] < rank_data[i + 1])
			{
				save = rank_data[i];
				rank_data[i] = rank_data[i + 1];
				rank_data[i + 1] = save;
				change += 1;
			}
		}
		for (int j = 3; j > 0; j--)
		{
			if (rank_data[j] > rank_data[j - 1])
			{
				save = rank_data[j];
				rank_data[j] = rank_data[j - 1];
				rank_data[j - 1] = save;
				change += 1;
			}
		}
	} while (change > 0);


	for (int i = 0; i < 4; i++)
	{
		data[i] = rank_data[i];
	}
}