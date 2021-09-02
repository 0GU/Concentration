#pragma once
#include "DxLib.h"

#include <string.h>
#include <string>
#include <sstream>
#include <Windows.h>

#define INITIALIZE 0                      //初期化
#define MAX 4                             //接続数
#define FLAG_MAX 3                        //フラグの数
#define WIDTH 1900                        //windowsサイズ　横
#define HEIGHT 1000                       //windowaサイズ　縦
#define WHITE 255,255,255                 //文字の色：白
#define RED 255,0,0                       //文字の色；赤
#define GREEN 0,255,0                     //文字の色：緑
#define BLUE 0,0,255                      //文字の色：青
#define CONNECTION_POS_X 0                //接続確立文字X軸
#define CONNECTION_POS_Y 0                //接続確立文字Y軸
#define CONNECTION_CMP_POS_X 0            //接続完了文字X軸
#define CONNECTION_CMP_POS_Y 16           //接続完了文字Y軸
#define TRUMP_WIDTH 120                   //カードの縦の長さ
#define TRUMP_HEIGHT 170                  //カードの横の長さ
#define HORIZONTAL_SPACING (20 + TRUMP_WIDTH) //トランプの横の間隔
#define VERTICAL_SPACING (30 + TRUMP_HEIGHT) //トランプの縦の間隔
#define OFFSET_X 50                       //画面表示位置補正X軸
#define OFFSET_Y 100                      //画面表示位置補正Y軸
#define NAME_INPUT_POS_X 0                //名前表示位置X軸
#define NAME_INPUT_POS_Y 0                //名前表示位置Y軸
#define NAME_INPUT_ANSWER_POS_X 0         //名前解答文字表示位置X軸
#define NAME_INPUT_ANSWER_POS_Y 16        //名前解答文字表示位置Y軸
#define NAME_INPUT_MAX 8                  //名前入力最大文字数
#define IP_INPUT_POS_X 0                  //IP入力文字表示位置X軸
#define IP_INPUT_POS_Y 0                  //IP入力文字表示位置Y軸
#define IP_DIS_POS_X 0                    //IP表示位置X軸
#define IP_DIS_POS_Y 16                   //IP表示位置Y軸
#define IP_CONFIRMATION_POS_X 0           //IP確認文字表示位置X軸
#define IP_CONFIRMATION_POS_Y 32          //IP確認文字表示位置Y軸
#define IP_ANSWER_POS_X 0                 //IP解答文字表示位置X軸
#define IP_ANSWER_POS_Y 48                //IP解答文字表示位置Y軸
#define IP_KEY_INPUT_POS_X 0              //IP入力文字表示位置X軸
#define IP_KEY_INPUT_POS_Y 16             //IP入力文字表示位置Y軸
#define IP_KEY_INPUT_MAX_NUM 255          //IP入力文字最大文字数
#define IP_KEY_INPUT_MIN_NUM 0            //IP入力文字最小文字数
#define IP_KEY_INPUT_ANSWER_POS_X 16      //IP入力文字確認文字表示位置X軸
#define IP_KEY_INPUT_ANSWER_POS_Y 64      //IP入力文字確認文字表示位置Y軸
#define RANK_DIS_POS_X 1000 //ランキング表示X座標
#define RANK_DIS_POS_Y 100 //ランキング表示Y座標

#define SUIT 4                            //トランプのスートの数
#define TRUMP_NUMBER 13                   //トランプの数字の数
#define TRUMP_MAX (SUIT*TRUMP_NUMBER)     //トランプの合計枚数

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
	int x, y;
};

//カード用構造体
struct Card {
	float x = INITIALIZE, y = INITIALIZE;
	int suit = INITIALIZE;
	int num = INITIALIZE;
};

//順位確認
struct Rank {
	char name[MAX][NAME_INPUT_MAX] = { { "null" },{ "null" },{ "null" },{ "null" } };//名前
	int count[MAX] = { 0,0,0,0 };
	int ranking[MAX] = { 0,0,0,0 };
};

//全員の名前
struct ALL_Name {
	char p_name[MAX][NAME_INPUT_MAX] = { { "null" },{ "null" },{ "null" },{ "null" } };//名前
};

//プレイヤークラス
class Data {
private:
public:
	char name[NAME_INPUT_MAX] = { "null" };//名前
	int count = INITIALIZE;
	int ID{ -1 };//オブジェクト識別用
	IPDATA ip{ 0,0,0,0 };//IPアドレス保存用
	bool flag[FLAG_MAX] = { false,false,false };//0・・・ターン識別用　1・・・クリック用　2・・・準備確認用

	//コンストラクタ
	Data();
	Data(char* _name, bool* _flag);
	//初期化用メソッド
	void Data_Init();
};

//カードクラス
class Trump {
private:
public:
	Card line_card ;
	int ID{ 10 }; //オブジェクト識別用
	bool FandB_flag = false; //裏表フラグ

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
	Rank_Data(ALL_Name _name, int* _count, int* _ranking);
	//初期化用メソッド
	void Rank_Data_Init();
};

//送受信データ用のクラス
class RecvData {
private:
public:
	Data data[MAX];
	Trump trump[TRUMP_MAX];
	bool End_flag;
};

class SendData {
private:
public:
	Point pos;
	bool Ready_flag;
	bool turn_flag;
};

class SendMyData {
private:
public:
	Data data;
};

////送受信トランプ用のクラス
//class SendTrump {
//private:
//public:
//	Trump trump[52];
//};


//送受信ランキングデータ用のクラス
class RecvRankData {
private:
public:
	Rank_Data Rdata;
};