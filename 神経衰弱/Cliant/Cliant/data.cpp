//data.cpp
#include "main.h"

//コンストラクタ
Data::Data(float _x, float _y, char* _name) {
	pos.x = _x;
	pos.y = _y;

	strcpy_s(name, _name);
}

Data::Data() {}

//初期化メソッド
void Data::Data_Init()
{
	strcpy_s(name, "null");

	ip.d1 = 0; ip.d2 = 0; ip.d3 = 0; ip.d4 = 0;

	pos.x = 0.0f;
	pos.y = 0.0f;

	ID = -1;
}

Trump::Trump() {};

Trump::Trump(Card _line_card, bool _FandB)
{
	line_card.x = _line_card.x;
	line_card.y = _line_card.y;
	line_card.num = _line_card.num;
	line_card.suit = _line_card.suit;

	FandB_flag = _FandB;
}

void Trump::Card_Init()
{
	line_card.x = 0;
	line_card.y = 0;
	line_card.num = 0;
	line_card.suit = 0;

	FandB_flag = false;
}