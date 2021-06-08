#pragma once
#include "DxLib.h"

#include <string.h>
#include <string>
#include <sstream>

#define MAX 4 //接続数
#define WIDTH 800 //windowサイズ　横
#define HEIGHT 600 //windowサイズ　縦

IPDATA IP_set(); //IP入力

//スレッドナンバー
enum {
	T_A,
	T_B,
	T_C,
	T_D,
};

//位置情報
struct Point {
	float x, y;
};

//カード用構造体
struct Card {
	float x = 0, y = 0;
	int suit = 0;
	int num = 0;
};

//プレイヤークラス
class Data {
private:
public:
	char name[8]{ "null" };//名前
	Point pos{ 0.0f, 0.0f };//位置
	int ID{ -1 };//オブジェクト識別用
	IPDATA ip{ 0,0,0,0 };//IPアドレス保存用
	//コンストラクタ
	Data();
	Data(float _x, float _y, char* _name);
	//初期化メソッド
	void Data_Init();
};

//カードクラス
class Trump {
private:
public:
	Card line_card ;
	int ID{ -1 }; //オブジェクト識別用
	bool FandB_flag = false;

	//コンストラクタ
	Trump();
	Trump(Card _line_card, bool _FandB);
	//初期化メソッド
	void Card_Init();
};

//送受信データ用のクラス
class SendData {
private:
public:
	Data data[MAX];
};