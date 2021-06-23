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

	//トランプデータ
	Point SetTrumpPos[MAX_TRUMP];
	bool setposflag = false;
	for (int i = INITIALIZE; i < MAX_TRUMP; i++)
	{
		SetTrumpPos[i] = { 99,99 };
	}
	Trump* All_trump[MAX_TRUMP];
	for (int i = INITIALIZE; i < SUIT; i++)
	{
		for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
		{
			All_trump[(i * TRUMP_NUMBER) + j] = new Trump();
			All_trump[(i * TRUMP_NUMBER) + j]->line_card.num = j;
			All_trump[(i * TRUMP_NUMBER) + j]->line_card.suit = i;
		}
	}
	//各トランプの座標決定
	for (int i = INITIALIZE; i < SUIT; i++)
	{
		for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
		{
			do
			{
				setposflag = false;
				All_trump[(i * TRUMP_NUMBER) + j]->line_card.x = GetRand(12);
				All_trump[(i * TRUMP_NUMBER) + j]->line_card.y = GetRand(3);
				for (int k = INITIALIZE; k < (i * TRUMP_NUMBER) + j; k++)
				{
					if (SetTrumpPos[k].x == All_trump[(i * TRUMP_NUMBER) + j]->line_card.x &&
						SetTrumpPos[k].y == All_trump[(i * TRUMP_NUMBER) + j]->line_card.y)
					{
						setposflag = true;
					}
				}
			} while ((i * TRUMP_NUMBER) + j != 0 && setposflag == true);
			SetTrumpPos[(i * TRUMP_NUMBER) + j].x = All_trump[(i * TRUMP_NUMBER) + j]->line_card.x;
			SetTrumpPos[(i * TRUMP_NUMBER) + j].y = All_trump[(i * TRUMP_NUMBER) + j]->line_card.y;
		}
	}

	//送信用データ
	SendData* Send_Data = new SendData();
	for (int i = INITIALIZE; i < MAX_TRUMP; i++)
	{
		Send_Data->trump[i] = *All_trump[i];
	}

	//受信用データ
	RecvData* Recv_Data[MAX];
	for (int i = 0; i < MAX; i++)
	{
		Recv_Data[i] = new RecvData();
	}

	//ネットワーク関係
	IPDATA IP;
	int Port = 26;
	int NetHandle[4] = { 0 };

	//トランプチェック用変数
	int Check_count = 0;
	int Save_Trump[2] = { 0,0 };

	//ゲーム開始用フラグ
	bool GameStart_flag = false;
	bool player_check = false;
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
						memcpy_s(Recv_Data[0], sizeof(RecvData), StrBuf, sizeof(RecvData));
						p_data[0]->flag[0] = Recv_Data[0]->turn_flag;
						p_data[0]->flag[2] = Recv_Data[0]->Ready_flag;
						//クリック判定
					/*	for (int i = 0; i < 52; i++)
						{

						}*/
						if (p_data[0]->flag[0] == true)
						{
							for (int i = INITIALIZE; i < SUIT; i++)
							{
								for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
								{
									if (50 + (j * 140) < Recv_Data[0]->pos.x &&
										50 + (j * 140) + 120 > Recv_Data[0]->pos.x &&
										100 + (i * 200) < Recv_Data[0]->pos.y &&
										100 + (i * 200) + 170 > Recv_Data[0]->pos.y)
									{
										for (int k = INITIALIZE; k < MAX_TRUMP; k++)
										{
											if (All_trump[k]->line_card.x == j && All_trump[k]->line_card.y == i && All_trump[k]->ID == 10&& All_trump[k]->FandB_flag==false)
											{
												All_trump[k]->FandB_flag = true;
												Save_Trump[Check_count] = k;
												Check_count += 1;
											}
										}
									}
								}
							}
						}
						//送信データの更新
						for (int i = INITIALIZE; i < MAX_TRUMP; i++)
						{
							Send_Data->trump[i] = *All_trump[i];
						}


						if (Check_count == 2)
						{
							if (All_trump[Save_Trump[0]]->line_card.num == All_trump[Save_Trump[1]]->line_card.num)
							{
								All_trump[Save_Trump[0]]->ID = 99;
								All_trump[Save_Trump[1]]->ID = 99;
								Send_Data->data[0].count += 2;
							}
							else
							{
								All_trump[Save_Trump[0]]->FandB_flag = false;
								All_trump[Save_Trump[1]]->FandB_flag = false;
								p_data[0]->flag[0] = false;

							}
							Check_count = 0;
						}

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

						Send_Data->data[0].ip = p_data[0]->ip;//IP
						Send_Data->data[0].ID = p_data[0]->ID;
						Send_Data->data[0].flag[0] = p_data[0]->flag[0];



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
			//	IPDATA ip{ 0,0,0,0 };//IPアドレス
			//	int DataLength = -1;//受信データの大きさ取得用
			//	int p2_NetHandle = -1;//ネットワークハンドル
			//	char StrBuf[256]{ "null" };//送受信データ用

			//	//初回接続処理
			//	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			//	{
			//		if (NetHandle[0] != 0)
			//		{
			//			p2_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
			//			if (p2_NetHandle != -1)
			//			{
			//				NetHandle[1] = p2_NetHandle;
			//				break;
			//			}
			//		}
			//	}

			//	//サブスレッドのメインループ
			//	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			//	{
			//		DataLength = GetNetWorkDataLength(p2_NetHandle);
			//		if (DataLength != 0)
			//		{
			//			//受信データをStrBufに取得
			//			NetWorkRecv(p2_NetHandle, StrBuf, DataLength);
			//			//接続してきたマシンのIpアドレスを取得
			//			GetNetWorkIP(p2_NetHandle, &ip);

			//			//IPアドレスから初回の接続か確認
			//			if (p_data[1]->ip.d1 == ip.d1 &&
			//				p_data[1]->ip.d2 == ip.d2 &&
			//				p_data[1]->ip.d3 == ip.d3 &&
			//				p_data[1]->ip.d4 == ip.d4)
			//			{
			//				//2回目以降の接続
			//			
			//				//受信データを変換
			//			
			//				//移動処理
			//			
			//				//送信データの更新

			//			}
			//			else
			//			{
			//				//初回の接続
			//				//IPと名前を登録
			//				p_data[1]->ip = ip;
			//				p_data[1]->ID = 0;
			//				memcpy_s(p_data[1]->name, sizeof(p_data[1]->name), StrBuf, sizeof(p_data[1]->name));
			//				//送信データの更新
			//				strcpy_s(Send_Data->data[1].name, sizeof(p_data[1]->name), p_data[1]->name);

			//				Send_Data->data[1].ip = p_data[1]->ip;//IP
			//				Send_Data->data[1].ID = p_data[1]->ID;

			//				//データを送信
			//				NetWorkSend(p2_NetHandle, Send_Data, sizeof(SendData));
			//			}
			//		}
			//	}
		}
	);

	//p_data[2]
	thread* p3 = new thread([&]()
		{
			//IPDATA ip{ 0,0,0,0 };//IPアドレス
			//int DataLength = -1;//受信データの大きさ取得用
			//int p3_NetHandle = -1;//ネットワークハンドル
			//char StrBuf[256]{ "null" };//送受信データ用

			////初回接続処理
			//while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			//{
			//	if (NetHandle[1] != 0)
			//	{
			//		p3_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
			//		if (p3_NetHandle != -1)
			//		{
			//			NetHandle[2] = p3_NetHandle;
			//			break;
			//		}
			//	}
			//}

			////サブスレッドのメインループ
			//while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			//{
			//	DataLength = GetNetWorkDataLength(p3_NetHandle);
			//	if (DataLength != 0)
			//	{
			//		//受信データをStrBufに取得
			//		NetWorkRecv(p3_NetHandle, StrBuf, DataLength);
			//		//接続してきたマシンのIpアドレスを取得
			//		GetNetWorkIP(p3_NetHandle, &ip);

			//		//IPアドレスから初回の接続か確認
			//		if (p_data[2]->ip.d1 == ip.d1 &&
			//			p_data[2]->ip.d2 == ip.d2 &&
			//			p_data[2]->ip.d3 == ip.d3 &&
			//			p_data[2]->ip.d4 == ip.d4)
			//		{
			//			//2回目以降の接続
			//		
			//			//受信データを変換
			//		
			//			//移動処理
			//			
			//			//送信データの更新

			//		}
			//		else
			//		{
			//			//初回の接続
			//			//IPと名前を登録
			//			p_data[2]->ip = ip;
			//			p_data[2]->ID = 0;
			//			memcpy_s(p_data[2]->name, sizeof(p_data[2]->name), StrBuf, sizeof(p_data[2]->name));
			//			//送信データの更新
			//			strcpy_s(Send_Data->data[2].name, sizeof(p_data[2]->name), p_data[2]->name);

			//			Send_Data->data[2].ip = p_data[2]->ip;//IP
			//			Send_Data->data[2].ID = p_data[2]->ID;

			//			//データを送信
			//			NetWorkSend(p3_NetHandle, Send_Data, sizeof(SendData));
			//		}
			//	}
			//}
		}
	);

	//p_data[3]
	thread* p4 = new thread([&]()
		{
			//IPDATA ip{ 0,0,0,0 };//IPアドレス
			//int DataLength = -1;//受信データの大きさ取得用
			//int p4_NetHandle = -1;//ネットワークハンドル
			//char StrBuf[256]{ "null" };//送受信データ用

			////初回接続処理
			//while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			//{
			//	if (NetHandle[2] != 0)
			//	{
			//		p4_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
			//		if (p4_NetHandle != -1)
			//		{
			//			NetHandle[3] = p4_NetHandle;
			//			break;
			//		}
			//	}
			//}

			////サブスレッドのメインループ
			//while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			//{
			//	DataLength = GetNetWorkDataLength(p4_NetHandle);
			//	if (DataLength != 0)
			//	{
			//		//受信データをStrBufに取得
			//		NetWorkRecv(p4_NetHandle, StrBuf, DataLength);
			//		//接続してきたマシンのIpアドレスを取得
			//		GetNetWorkIP(p4_NetHandle, &ip);

			//		//IPアドレスから初回の接続か確認
			//		if (p_data[3]->ip.d1 == ip.d1 &&
			//			p_data[3]->ip.d2 == ip.d2 &&
			//			p_data[3]->ip.d3 == ip.d3 &&
			//			p_data[3]->ip.d4 == ip.d4)
			//		{
			//			//2回目以降の接続
			//			
			//			//受信データを変換
			//		
			//			//移動処理
			//		
			//			//送信データの更新

			//		}
			//		else
			//		{
			//			//初回の接続
			//			//IPと名前を登録
			//			p_data[3]->ip = ip;
			//			p_data[3]->ID = 0;
			//			memcpy_s(p_data[3]->name, sizeof(p_data[3]->name), StrBuf, sizeof(p_data[3]->name));
			//			//送信データの更新
			//			strcpy_s(Send_Data->data[3].name, sizeof(p_data[3]->name), p_data[3]->name);
			//			Send_Data->data[3].ip = p_data[3]->ip;//IP
			//			Send_Data->data[3].ID = p_data[3]->ID;

			//			//データを送信
			//			NetWorkSend(p4_NetHandle, Send_Data, sizeof(SendData));
			//		}
			//	}
			//}
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




		int Mouse = GetMouseInput();

		if (Mouse & MOUSE_INPUT_RIGHT)
		{
			p_data[0]->flag[0] = true;
		}

		if (GameStart_flag == false)
		{
			for (int i = INITIALIZE; i < MAX; i++) {
				//準備確認
				if (NetHandle[i] != 0) {
					if (p_data[i]->flag[2] == false)
					{
						break;
					}
					player_check = true;
				}
				if (i == MAX - 1&&player_check==true)
				{
					//プレイヤーの順番を決める処理
					/*

					*/

					GameStart_flag = true;
					p_data[0]->flag[0] = true;
				}
			}
		}
		if (GameStart_flag == true)
		{
			//ターン移行処理
			/*if ()
			{

			}
			*/
			//クリックフラグをfalseにする
			if (Send_Data->data[0].flag[1] == true && Check_count != 2)
			{
				Send_Data->data[0].flag[1] = false;
			}

			for (int i = INITIALIZE; i < MAX; i++)
			{
				if (NetHandle[i] != -1) {
					Send_Data->data[i].flag[0] = p_data[i]->flag[0];
				}
			}

		}

		for (int i = 0; i < MAX; i++) {
			//データを送信する
			if (NetHandle[i] != -1) {
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
			DrawFormatString(0, i * 16 + 32, GetColor(255, 255, 255),
				"スレッド1 IP:%d.%d.%d.%d name=%8s 枚数=%d:y=%f",
				p_data[i]->ip.d1,
				p_data[i]->ip.d2,
				p_data[i]->ip.d3,
				p_data[i]->ip.d4,
				p_data[i]->name,
				p_data[i]->count,
				p_data[i]->flag
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