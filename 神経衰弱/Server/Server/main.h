#pragma once
#include "DxLib.h"

#include <string.h>
#include <string>
#include <sstream>

#define INITIALIZE 0//初期化

#define MAX 4//接続数
#define FLAG_MAX 3//フラグの数
#define WIDTH 800//windowsサイズ　横
#define HEIGHT 600//windowaサイズ　縦
#define OFFSET_X 50//画像表示位置補正　横
#define OFFSET_Y 100//画像表示位置補正　縦
#define TRUMP_WIDTH 120//トランプの画像サイズ　横
#define TRUMP_HEIGHT 170//トランプの画像サイズ　縦
#define HORIZONTAL_SPACING (20+TRUMP_WIDTH)//トランプの横の間隔
#define VERTICAL_SPACING (30+TRUMP_HEIGHT)//トランプの縦の間隔 

#define SUIT 4//トランプのスートの数
#define TRUMP_NUMBER 13//トランプの数字の数
#define MAX_TRUMP (SUIT*TRUMP_NUMBER)//トランプの最大枚数


	
//IPDATA IP_set();//IP入力

//スレッドナンバー
enum{
	T_A,
	T_B,
	T_C,
	T_D,
};

//カード用構造体
struct Card {
	float x = 0, y = 0;
	int suit = 0;
	int num = 0;
};

//位置情報
struct Point{
	int x, y;
};

//順位確認
struct Rank{
	char name[MAX][8] = { { "null" },{ "null" },{ "null" },{ "null" } };//名前
	int count[MAX] = {0,0,0,0};
	int ranking[MAX] = {0,0,0,0};
};

//全員の名前
struct ALL_Name{
	char p_name[MAX][8] = { { "null" },{ "null" },{ "null" },{ "null" } };//名前
};

//プレイヤークラス
class Data{
private:
public:
	char name[8] = { "null" };//名前
	int count=0;
	int ID{ -1 };//オブジェクト識別用
	IPDATA ip{ 0,0,0,0 };//IPアドレス保存用
	bool flag[FLAG_MAX] = { false,false,false };//0・・・ターン識別用　1・・・クリック用　2・・・準備確認用
	
	//コンストラクタ
	Data();
	Data( char* _name,bool* _flag);
	//初期化用メソッド
	void Data_Init();
};

//カードクラス
class Trump {
private:
public:
	Card line_card;
	int ID{ 10 }; //オブジェクト識別用
	bool FandB_flag = false;

	//コンストラクタ
	Trump();
	Trump(Card _line_card, bool _FandB);
	//初期化メソッド
	void Card_Init();
};

//ランキングクラス
class Rank_Data {
private:
public:
	Rank allrank;
	//コンストラクタ
	Rank_Data();
	Rank_Data(ALL_Name _name, int* _count,int* _ranking);
	//初期化用メソッド
	void Rank_Data_Init();
};

//送信データ用のクラス
class SendData {
private:
public:
	Data data[MAX];
	Trump trump[52];
};

//受信データ用のクラス
class RecvData {
private:
public:
	Point pos;
	bool Ready_flag;
	bool turn_flag;
};

////送受信トランプ用のクラス
//class SendTrump {
//private:
//public:
//	
//};

//送受信ランキングデータ用のクラス
class SendRankData {
private:
public:
	Rank_Data Rdata;
};