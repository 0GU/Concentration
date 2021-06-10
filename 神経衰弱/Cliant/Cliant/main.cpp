#pragma once
#include "DxLib.h"
#include "main.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

	//windowモード切り替え
	ChangeWindowMode(TRUE);
	//windowサイズ
	SetGraphMode(WIDTH, HEIGHT, 32);
	//バックグラウンド処理の許可
	SetAlwaysRunFlag(TRUE);
	//多重起動の許可
	SetDoubleStartValidFlag(TRUE);
	//Dxライブラリの初期化
	if (DxLib_Init() == -1) return -1; //失敗したらプログラム終了

	//バックバッファを使う設定
	SetDrawScreen(DX_SCREEN_BACK);
	//windowの名前
	SetWindowText("神経衰弱");

	//画像読み込み
	
	//送受信データ処理用
	char StrBuf[256] = { "null" };//256バイトまで

	//全てのプレイヤーデータ
	SendData* Player_ALL = new SendData();

	//通信関係
	IPDATA IP;
	//通信用のハンドル
	int NetHandel;
	//ポート
	int Port = 26;

	//通信先のIPアドレス設定
	IP = IP_set();

	//名前の入力
	char name[8] = " null ";
	ClearDrawScreen();//画面クリア
	DrawString(0, 0, "名前を入力　小文字8文字/全角4文字まで",
		GetColor(255, 255, 255));
	KeyInputString(0, 16, 8, name, FALSE);

	//初回送信データの作成
	Data* my_Data = new Data(0.0f, 0.0f, name,false);

	//初回接続処理
	NetHandel = ConnectNetWork(IP, Port);//入力したIPと設定したポートを使用

	//接続するまで待機
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen(); //画面クリア

		if (NetHandel != -1) {
			//接続完了
			//サーバーにデータを送信
			NetWorkSend(NetHandel, &my_Data->name, sizeof(my_Data->name));
			//サーバーからの応答待機
			while (!ProcessMessage()) {
				//送受信データを調べる
				if (GetNetWorkDataLength(NetHandel) != 0) break;
			}
			//受信したデータを変換
			//受信データをメモリからStrBufにコピーして、SendDataに変換
			NetWorkRecv(NetHandel, StrBuf, sizeof(SendData)); //コピー作業
			memcpy_s(Player_ALL, sizeof(SendData), StrBuf, sizeof(SendData));//変換

			DrawString(0, 16, "接続完了。何かキーを押してください。", GetColor(255, 255, 255));

			ScreenFlip();
			WaitKey();
			break;
		}
		else {
			//接続中
			DrawString(0, 0, "接続確立中・・・", GetColor(255, 255, 255));
		}
		ScreenFlip();
	}

	//メインループ
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();//画面をクリア

		if (GetNetWorkDataLength(NetHandel) != 0) {
			//データを受信してた場合
			NetWorkRecv(NetHandel, StrBuf, sizeof(SendData));
			//プレイヤー全体データの更新
			memcpy_s(Player_ALL, sizeof(SendData), StrBuf, sizeof(SendData));
		}
		else {
			//データ受信してない場合
		}

		ScreenFlip(); //画面更新

		//例外処理発生でループを抜ける
		if (ProcessMessage() == -1)break;
	}

	DxLib_End(); //Dxライブラリの開放
	return 0;
}