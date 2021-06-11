//data.cpp
#include "main.h"

//コンストラクタ
Data::Data( char* _name,bool* _flag){

	for (int i = 0; i < FLAG_MAX; i++)
	{
		flag[i] = _flag[i];
	}
	strcpy_s(name, _name);
}

Data::Data(){}

//初期化メソッド
void Data::Data_Init()
{
	strcpy_s(name, "null");

	ip.d1 = 0; ip.d2 = 0; ip.d3 = 0; ip.d4 = 0;

	ID = -1;

	count = 0;

	for (int i = 0; i <FLAG_MAX; i++)
	{
		flag[i] == false;
	}

}

Rank_Data::Rank_Data(ALL_Name _name, int* _count, int* _ranking){
	for (int i = 0; i < MAX; i++)
	{
		allrank.count[i] =_count[i];
		strcpy_s(allrank.name[i], _name.p_name[i]);
		allrank.ranking[i] = _ranking[i];
	}
}

Rank_Data::Rank_Data() {};

void Rank_Data::Rank_Data_Init() {
	for (int i = 0; i < MAX; i++)
	{
		allrank.count[i] = 0;
		strcpy_s(allrank.name[i], "null");
		allrank.ranking[i] = 0;
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
	line_card.x = 0;
	line_card.y = 0;
	line_card.num = 0;
	line_card.suit = 0;

	FandB_flag = false;
}