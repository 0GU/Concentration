//data.cpp
#include "main.h"

//コンストラクタ
Data::Data(char* _name, bool* _flag) {

	for (int i = INITIALIZE; i < FLAG_MAX; i++)
	{
		flag[i] = _flag[i];
	}
	strcpy_s(name, _name);
}

Data::Data() {}

//初期化メソッド
void Data::Data_Init()
{
	strcpy_s(name, "null");

	ip.d1 = INITIALIZE; ip.d2 = INITIALIZE; ip.d3 = INITIALIZE; ip.d4 = INITIALIZE;

	ID = -1;

	count = INITIALIZE;

	for (int i = INITIALIZE; i < FLAG_MAX; i++)
	{
		flag[i] = false;
	}

}

Rank_Data::Rank_Data(ALL_Name _name, int* _count, int* _ranking) {
	for (int i = INITIALIZE; i < MAX; i++)
	{
		allrank.count[i] = _count[i];
		strcpy_s(allrank.name[i], _name.p_name[i]);
		allrank.ranking[i] = _ranking[i];
	}
}

Rank_Data::Rank_Data() {};

void Rank_Data::Rank_Data_Init() {
	for (int i = INITIALIZE; i < MAX; i++)
	{
		allrank.count[i] = INITIALIZE;
		strcpy_s(allrank.name[i], "null");
		allrank.ranking[i] = INITIALIZE;
	}
}

Trump::Trump(Card _line_card, bool _FandB)
{
	line_card.x = _line_card.x;
	line_card.y = _line_card.y;
	line_card.num = _line_card.num;
	line_card.suit = _line_card.suit;

	FandB_flag = _FandB;
}

Trump::Trump() {};

void Trump::Card_Init()
{
	line_card.x = INITIALIZE;
	line_card.y = INITIALIZE;
	line_card.num = INITIALIZE;
	line_card.suit = INITIALIZE;

	FandB_flag = false;
}