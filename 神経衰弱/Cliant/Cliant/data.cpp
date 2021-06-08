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