//data.cpp
#include "main.h"

//コンストラクタ
Data::Data(float _x, float _y, char* _name,bool* _flag){
	pos.x = _x;
	pos.y = _y;
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
	
	pos.x = 0.0f;
	pos.y = 0.0f;

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