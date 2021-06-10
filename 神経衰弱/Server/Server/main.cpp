//マルチスレッドTCP通信
//サーバー
//Dxライブラリ使用
#pragma once
#include "main.h"
#include <thread>
#include <iostream>
using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	//windowモードの切り替え
	ChangeWindowMode(TRUE);
	//windowサイズ
	SetGraphMode(WIDTH, HEIGHT, 32);
	//バックグラウンド処理の許可
	SetAlwaysRunFlag(TRUE);
	//多重起動の許可
	SetDoubleStartValidFlag(TRUE);

	//Dxライブラリの初期化
	if (DxLib_Init() == -1)return -1;//失敗したらプログラム終了

	//windowの名前
	SetWindowText("サーバー");

	//バックバッファを使用
	SetDrawScreen(DX_SCREEN_BACK);

	//送受信データ処理用
	char StrBuf[256] = { "NULL" };

	//プレイヤーデータ
	Data* p_data[MAX];
	for (int i = 0; i < MAX; i++)p_data[i] = new Data();

	//送信用データ
	SendData* Send_Data = new SendData();

	//ネットワーク関係
	IPDATA IP;
	int Port = 26;
	int NetHandle[4] = { 0 };

	///////////

	//接続待機状態にする
	PreparationListenNetWork(Port);

	//サブスレッド
	//p_data[0]
	thread* p1 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p1_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p1_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
				if (p1_NetHandle != -1)
				{
					NetHandle[0] = p1_NetHandle;
					break;
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p1_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p1_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p1_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[0]->ip.d1 == ip.d1 &&
						p_data[0]->ip.d2 == ip.d2 &&
						p_data[0]->ip.d3 == ip.d3 &&
						p_data[0]->ip.d4 == ip.d4)
					{
						//2回目以降の接続
						
						//受信データを変換
					
						//送信データの更新
						Send_Data->data[0].pos.x = p_data[0]->pos.x;
						Send_Data->data[0].pos.y = p_data[0]->pos.y;
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[0]->ip = ip;
						p_data[0]->ID = 0;
						memcpy_s(p_data[0]->name, sizeof(p_data[0]->name), StrBuf, sizeof(p_data[0]->name));
						//送信データの更新
						strcpy_s(Send_Data->data[0].name, sizeof(p_data[0]->name), p_data[0]->name);
						Send_Data->data[0].pos = p_data[0]->pos;//位置
						Send_Data->data[0].ip = p_data[0]->ip;//IP
						Send_Data->data[0].ID = p_data[0]->ID;

						//データを送信
						NetWorkSend(p1_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//p_data[1]
	thread* p2 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p2_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p2_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
				if (p2_NetHandle != -1)
				{
					NetHandle[1] = p2_NetHandle;
					break;
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p2_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p2_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p2_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[1]->ip.d1 == ip.d1 &&
						p_data[1]->ip.d2 == ip.d2 &&
						p_data[1]->ip.d3 == ip.d3 &&
						p_data[1]->ip.d4 == ip.d4)
					{
						//2回目以降の接続
					
						//受信データを変換
					
						//移動処理
					
						//送信データの更新
						Send_Data->data[1].pos.x = p_data[1]->pos.x;
						Send_Data->data[1].pos.y = p_data[1]->pos.y;
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[1]->ip = ip;
						p_data[1]->ID = 0;
						memcpy_s(p_data[1]->name, sizeof(p_data[1]->name), StrBuf, sizeof(p_data[1]->name));
						//送信データの更新
						strcpy_s(Send_Data->data[1].name, sizeof(p_data[1]->name), p_data[1]->name);
						Send_Data->data[1].pos = p_data[1]->pos;//位置
						Send_Data->data[1].ip = p_data[1]->ip;//IP
						Send_Data->data[1].ID = p_data[1]->ID;

						//データを送信
						NetWorkSend(p2_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//p_data[2]
	thread* p3 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p3_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p3_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
				if (p3_NetHandle != -1)
				{
					NetHandle[2] = p3_NetHandle;
					break;
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p3_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p3_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p3_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[2]->ip.d1 == ip.d1 &&
						p_data[2]->ip.d2 == ip.d2 &&
						p_data[2]->ip.d3 == ip.d3 &&
						p_data[2]->ip.d4 == ip.d4)
					{
						//2回目以降の接続
					
						//受信データを変換
					
						//移動処理
						
						//送信データの更新
						Send_Data->data[2].pos.x = p_data[2]->pos.x;
						Send_Data->data[2].pos.y = p_data[2]->pos.y;
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[2]->ip = ip;
						p_data[2]->ID = 0;
						memcpy_s(p_data[2]->name, sizeof(p_data[2]->name), StrBuf, sizeof(p_data[2]->name));
						//送信データの更新
						strcpy_s(Send_Data->data[2].name, sizeof(p_data[2]->name), p_data[2]->name);
						Send_Data->data[2].pos = p_data[2]->pos;//位置
						Send_Data->data[2].ip = p_data[2]->ip;//IP
						Send_Data->data[2].ID = p_data[2]->ID;

						//データを送信
						NetWorkSend(p3_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//p_data[3]
	thread* p4 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p4_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p4_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
				if (p4_NetHandle != -1)
				{
					NetHandle[3] = p4_NetHandle;
					break;
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p4_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p4_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p4_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[3]->ip.d1 == ip.d1 &&
						p_data[3]->ip.d2 == ip.d2 &&
						p_data[3]->ip.d3 == ip.d3 &&
						p_data[3]->ip.d4 == ip.d4)
					{
						//2回目以降の接続
						
						//受信データを変換
					
						//移動処理
					
						//送信データの更新
						Send_Data->data[3].pos.x = p_data[3]->pos.x;
						Send_Data->data[3].pos.y = p_data[3]->pos.y;
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[3]->ip = ip;
						p_data[3]->ID = 0;
						memcpy_s(p_data[3]->name, sizeof(p_data[3]->name), StrBuf, sizeof(p_data[3]->name));
						//送信データの更新
						strcpy_s(Send_Data->data[3].name, sizeof(p_data[3]->name), p_data[3]->name);
						Send_Data->data[3].pos = p_data[3]->pos;//位置
						Send_Data->data[3].ip = p_data[3]->ip;//IP
						Send_Data->data[3].ID = p_data[3]->ID;

						//データを送信
						NetWorkSend(p4_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//メインループ(サーバー管理画面)
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();//画面クリア
		//起動端末のIPアドレス
		GetMyIPAddress(&IP, 1, NULL);

		//切断状況をチェック
		int LostHandle = GetLostNetWork();


		for (int i = 0; i < MAX; i++) {
			//データを送信する
			if (NetHandle[i]!=-1){
				NetWorkSend(NetHandle[i], Send_Data, sizeof(SendData));
			}

			//切断したプレイヤーを初期化
			if (LostHandle == NetHandle[i]) {
				NetHandle[i] = -1;
				//データの初期化
				p_data[i]->Data_Init();
			}
		}
		//状況表示
		DrawFormatString(0, 0, GetColor(255, 255, 255),
			"PCのIPアドレス:%d.%d.%d.%d 接続ポート:%d",
			IP.d1,
			IP.d2,
			IP.d3,
			IP.d4,
			Port
		);
		for (int i = 0; i < MAX; i++)
		{
			DrawFormatString(0, i*16+32, GetColor(255, 255, 255),
				"スレッド1 IP:%d.%d.%d.%d name=%8s x=%f:y=%f",
				p_data[i]->ip.d1,
				p_data[i]->ip.d2,
				p_data[i]->ip.d3,
				p_data[i]->ip.d4,
				p_data[i]->name,
				p_data[i]->pos.x,
				p_data[i]->pos.y
			);
		}
		
		ScreenFlip();//画面更新
		if ((ProcessMessage() == -1))break;
	}
	p1->join();
	p2->join();
	p3->join();
	p4->join();
	delete p1;
	delete p2;
	delete p3;
	delete p4;

	DxLib_End();
	return 0;
}