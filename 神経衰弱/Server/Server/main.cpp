//マルチスレッドTCP通信
//サーバー
//Dxライブラリ使用
#pragma once
#include "main.h"

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
	for (int i = INITIALIZE; i < MAX; i++)p_data[i] = new Data();

	//トランプデータ
	Point SetTrumpPos[MAX_TRUMP];////セットされている座標確認用配列
	bool setposflag = false;//再設定用フラグ
	//初期化
	for (int i = INITIALIZE; i < MAX_TRUMP; i++)
	{
		SetTrumpPos[i] = { TRUMP_INIT,TRUMP_INIT };
	}

	Trump* All_trump[MAX_TRUMP];//トランプ用配列
	//数字とスートの設定
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
				setposflag = false;//再設定用フラグをfalseにする
				All_trump[(i * TRUMP_NUMBER) + j]->line_card.x = GetRand(12);//x座標の決定
				All_trump[(i * TRUMP_NUMBER) + j]->line_card.y = GetRand(3);//y座標の決定
				for (int k = INITIALIZE; k < (i * TRUMP_NUMBER) + j; k++)
				{
					//決定した座標に他のカードが既にセットされている場合、再設定用フラグをtrueにする
					if (SetTrumpPos[k].x == All_trump[(i * TRUMP_NUMBER) + j]->line_card.x &&
						SetTrumpPos[k].y == All_trump[(i * TRUMP_NUMBER) + j]->line_card.y)
					{
						setposflag = true; //再設定用フラグをtrueにする
					}
				}
			} while (/*(i * TRUMP_NUMBER) + j != 0 &&*/ setposflag == true);
			SetTrumpPos[(i * TRUMP_NUMBER) + j].x = All_trump[(i * TRUMP_NUMBER) + j]->line_card.x;//セットされている座標確認用配列にx座標を保存する
			SetTrumpPos[(i * TRUMP_NUMBER) + j].y = All_trump[(i * TRUMP_NUMBER) + j]->line_card.y;//セットされている座標確認用配列にy座標を保存する
		}
	}

	//登録送信用データ
	SendSetData* Send_Set_Data = new SendSetData();
	for (int i = INITIALIZE; i < MAX_TRUMP; i++)
	{
		Send_Set_Data->trump[i] = *All_trump[i];
	}
	//メイン送信用データ
	SendMainData* Send_Main_Data = new SendMainData();
	for (int i = 0; i < MAX; i++)
	{
		Send_Main_Data->data[i].Data_Init();
	}
	

	//ランキング送信用データ
	SendRankData* Send_RankData = new SendRankData();
	Send_RankData->Rdata.Rank_Data_Init();
	//順位ソート用の配列
	int RankSort[4];
	//初期化
	for (int i = INITIALIZE; i < 4; i++)
	{
			RankSort[i] = -1;
	}


	//受信用データ
	RecvData* Recv_Data[MAX];
	for (int i = INITIALIZE; i < MAX; i++)
	{
		Recv_Data[i] = new RecvData();
	}

	//ネットワーク関係
	IPDATA IP;
	int Port = 26;
	int NetHandle[MAX] = { INITIALIZE };

	//トランプチェック用変数
	int Check_count = INITIALIZE;
	int Save_Trump[2] = { INITIALIZE,INITIALIZE };
	int GetCord_num = 0;

	//ゲーム開始/終了用フラグ
	bool GameStart_flag = false;
	bool GameSet_flag = false;
	bool player_check = false;

	//ターン処理用変数
	int Turn_Player_num = INITIALIZE;//ターンプレイヤー識別用
	int Join_Player_num = INITIALIZE;//参加人数保存用
	///////////

	//接続待機状態にする
	PreparationListenNetWork(Port);

	//サブスレッド
	//p_data[0]
	thread* p1 = new thread([&]()
		{
			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p1_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				p1_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
				if (p1_NetHandle != -1)
				{
					NetHandle[Player1] = p1_NetHandle;
					break;
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p1_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p1_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p1_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[Player1]->ip.d1 == ip.d1 &&
						p_data[Player1]->ip.d2 == ip.d2 &&
						p_data[Player1]->ip.d3 == ip.d3 &&
						p_data[Player1]->ip.d4 == ip.d4)
					{
						//2回目以降の接続

						//受信データを変換
						memcpy_s(Recv_Data[Player1], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player1]->flag[2] = Recv_Data[Player1]->Ready_flag;
						if (p_data[Player1]->flag[2] == true)
						{
							Send_Main_Data->data[Player1].flag[2] = true;
						}

						//クリック判定

						if (p_data[Player1]->flag[0] == true&& Send_Main_Data->Reverse_flag == false)
						{
							for (int i = INITIALIZE; i < SUIT; i++)
							{
								for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
								{
									if (OFFSET_X + (j * HORIZONTAL_SPACING) < Recv_Data[Player1]->pos.x &&
										OFFSET_X + (j * HORIZONTAL_SPACING) + TRUMP_WIDTH > Recv_Data[Player1]->pos.x &&
										OFFSET_Y + (i * VERTICAL_SPACING) < Recv_Data[Player1]->pos.y &&
										OFFSET_Y + (i * VERTICAL_SPACING) + TRUMP_HEIGHT > Recv_Data[Player1]->pos.y)
									{
										for (int k = INITIALIZE; k < MAX_TRUMP; k++)
										{
											if (All_trump[k]->line_card.x == j && All_trump[k]->line_card.y == i && All_trump[k]->ID == 10 && All_trump[k]->FandB_flag == false)
											{
												All_trump[k]->FandB_flag = true;
												Send_Main_Data->Reverse.x = j;
												Send_Main_Data->Reverse.y = i;
												Send_Main_Data->Reverse_flag = true;
												Save_Trump[Check_count] = k;
												Check_count += 1;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[Player1]->ip = ip;
						p_data[Player1]->ID = INITIALIZE;
						memcpy_s(p_data[Player1]->name, sizeof(p_data[Player1]->name), StrBuf, sizeof(p_data[Player1]->name));
						//初回の接続用送信データの更新
						strcpy_s(Send_Set_Data->data[Player1].name, sizeof(p_data[Player1]->name), p_data[Player1]->name);

						Send_Set_Data->data[Player1].ip = p_data[Player1]->ip;//IP
						Send_Set_Data->data[Player1].ID = p_data[Player1]->ID;
						Send_Set_Data->data[Player1].flag[0] = p_data[Player1]->flag[0];
						//二回目以降の送信データの更新
						strcpy_s(Send_Set_Data->data[Player1].name, sizeof(p_data[Player1]->name), p_data[Player1]->name);

						Send_Main_Data->data[Player1].ip = p_data[Player1]->ip;//IP
						Send_Main_Data->data[Player1].ID = p_data[Player1]->ID;
						Send_Main_Data->data[Player1].flag[0] = p_data[Player1]->flag[0];
						//データを送信
						NetWorkSend(p1_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
					}
				}
			}
		}
	);

	//p_data[1]
	thread* p2 = new thread([&]()
		{

			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p2_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				if (NetHandle[Player1] != 0)
				{
					p2_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
					if (p2_NetHandle != -1)
					{
						NetHandle[Player2] = p2_NetHandle;
						break;
					}
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p2_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p2_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p2_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[Player2]->ip.d1 == ip.d1 &&
						p_data[Player2]->ip.d2 == ip.d2 &&
						p_data[Player2]->ip.d3 == ip.d3 &&
						p_data[Player2]->ip.d4 == ip.d4)
					{
						//2回目以降の接続

						//受信データを変換
						memcpy_s(Recv_Data[Player2], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player2]->flag[2] = Recv_Data[Player2]->Ready_flag;
						if (p_data[Player2]->flag[2] == true)
						{
							Send_Main_Data->data[Player2].flag[2] = true;
						}
						//クリック判定

						if (p_data[Player2]->flag[0] == true&&Send_Main_Data->Reverse_flag == false)
						{
							for (int i = INITIALIZE; i < SUIT; i++)
							{
								for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
								{
									if (OFFSET_X + (j * HORIZONTAL_SPACING) < Recv_Data[Player2]->pos.x &&
										OFFSET_X + (j * HORIZONTAL_SPACING) + TRUMP_WIDTH > Recv_Data[Player2]->pos.x &&
										OFFSET_Y + (i * VERTICAL_SPACING) < Recv_Data[Player2]->pos.y &&
										OFFSET_Y + (i * VERTICAL_SPACING) + TRUMP_HEIGHT > Recv_Data[Player2]->pos.y)
									{
										for (int k = INITIALIZE; k < MAX_TRUMP; k++)
										{
											if (All_trump[k]->line_card.x == j && All_trump[k]->line_card.y == i && All_trump[k]->ID == 10 && All_trump[k]->FandB_flag == false)
											{
												All_trump[k]->FandB_flag = true;
												Send_Main_Data->Reverse.x= j;
												Send_Main_Data->Reverse.y = i;
												Send_Main_Data->Reverse_flag = true;
												Save_Trump[Check_count] = k;
												Check_count += 1;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[Player2]->ip = ip;
						p_data[Player2]->ID = INITIALIZE;
						memcpy_s(p_data[Player2]->name, sizeof(p_data[Player2]->name), StrBuf, sizeof(p_data[Player2]->name));
						//初回の接続用送信データの更新
						strcpy_s(Send_Set_Data->data[Player2].name, sizeof(p_data[Player2]->name), p_data[Player2]->name);

						Send_Set_Data->data[Player2].ip = p_data[Player2]->ip;//IP
						Send_Set_Data->data[Player2].ID = p_data[Player2]->ID;
						Send_Set_Data->data[Player2].flag[0] = p_data[Player2]->flag[0];

						//二回目以降の送信データの更新
						strcpy_s(Send_Main_Data->data[Player2].name, sizeof(p_data[Player2]->name), p_data[Player2]->name);

						Send_Main_Data->data[Player2].ip = p_data[Player2]->ip;//IP
						Send_Main_Data->data[Player2].ID = p_data[Player2]->ID;
						Send_Main_Data->data[Player2].flag[0] = p_data[Player2]->flag[0];

						//データを送信
						NetWorkSend(p2_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
					}
				}
			}
		}
	);

	//p_data[2]
	thread* p3 = new thread([&]()
		{


			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p3_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				if (NetHandle[Player2] != 0)
				{
					p3_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
					if (p3_NetHandle != -1)
					{
						NetHandle[Player3] = p3_NetHandle;
						break;
					}
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p3_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p3_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p3_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[Player3]->ip.d1 == ip.d1 &&
						p_data[Player3]->ip.d2 == ip.d2 &&
						p_data[Player3]->ip.d3 == ip.d3 &&
						p_data[Player3]->ip.d4 == ip.d4)
					{
						//2回目以降の接続

						//受信データを変換
						memcpy_s(Recv_Data[Player3], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player3]->flag[2] = Recv_Data[Player3]->Ready_flag;
						if (p_data[Player3]->flag[2] == true)
						{
							Send_Main_Data->data[Player3].flag[2] = true;
						}
						//クリック判定

						if (p_data[Player3]->flag[0] == true&& Send_Main_Data->Reverse_flag == false)
						{
							for (int i = INITIALIZE; i < SUIT; i++)
							{
								for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
								{
									if (OFFSET_X + (j * HORIZONTAL_SPACING) < Recv_Data[Player3]->pos.x &&
										OFFSET_X + (j * HORIZONTAL_SPACING) + TRUMP_WIDTH > Recv_Data[Player3]->pos.x &&
										OFFSET_Y + (i * VERTICAL_SPACING) < Recv_Data[Player3]->pos.y &&
										OFFSET_Y + (i * VERTICAL_SPACING) + TRUMP_HEIGHT > Recv_Data[Player3]->pos.y)
									{
										for (int k = INITIALIZE; k < MAX_TRUMP; k++)
										{
											if (All_trump[k]->line_card.x == j && All_trump[k]->line_card.y == i && All_trump[k]->ID == 10 && All_trump[k]->FandB_flag == false)
											{
												All_trump[k]->FandB_flag = true;
												Send_Main_Data->Reverse.x = j;
												Send_Main_Data->Reverse.y = i;
												Send_Main_Data->Reverse_flag = true;
												Save_Trump[Check_count] = k;
												Check_count += 1;
											}
										}
									}
								}
							}
						}
					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[Player3]->ip = ip;
						p_data[Player3]->ID = INITIALIZE;
						memcpy_s(p_data[Player3]->name, sizeof(p_data[Player3]->name), StrBuf, sizeof(p_data[Player3]->name));
						//初回の接続用送信データの更新
						strcpy_s(Send_Set_Data->data[Player3].name, sizeof(p_data[Player3]->name), p_data[Player3]->name);

						Send_Set_Data->data[Player3].ip = p_data[Player3]->ip;//IP
						Send_Set_Data->data[Player3].ID = p_data[Player3]->ID;
						Send_Set_Data->data[Player3].flag[0] = p_data[Player3]->flag[0];

						//二回目以降の送信データの更新
						strcpy_s(Send_Main_Data->data[Player3].name, sizeof(p_data[Player3]->name), p_data[Player3]->name);

						Send_Main_Data->data[Player3].ip = p_data[Player3]->ip;//IP
						Send_Main_Data->data[Player3].ID = p_data[Player3]->ID;
						Send_Main_Data->data[Player3].flag[0] = p_data[Player3]->flag[0];

						//データを送信
						NetWorkSend(p3_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
					}
				}
			}
		}
	);

	//p_data[3]
	thread* p4 = new thread([&]()
		{

			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IPアドレス
			int DataLength = -1;//受信データの大きさ取得用
			int p4_NetHandle = -1;//ネットワークハンドル
			char StrBuf[256]{ "null" };//送受信データ用

			//初回接続処理
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				if (NetHandle[Player3] != 0)
				{
					p4_NetHandle = GetNewAcceptNetWork();//ネットワークハンドル取得
					if (p4_NetHandle != -1)
					{
						NetHandle[Player4] = p4_NetHandle;
						break;
					}
				}
			}

			//サブスレッドのメインループ
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p4_NetHandle);
				if (DataLength != 0)
				{
					//受信データをStrBufに取得
					NetWorkRecv(p4_NetHandle, StrBuf, DataLength);
					//接続してきたマシンのIpアドレスを取得
					GetNetWorkIP(p4_NetHandle, &ip);

					//IPアドレスから初回の接続か確認
					if (p_data[Player4]->ip.d1 == ip.d1 &&
						p_data[Player4]->ip.d2 == ip.d2 &&
						p_data[Player4]->ip.d3 == ip.d3 &&
						p_data[Player4]->ip.d4 == ip.d4)
					{
						//2回目以降の接続

						//受信データを変換
						memcpy_s(Recv_Data[Player4], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player4]->flag[2] = Recv_Data[Player4]->Ready_flag;
						if (p_data[Player4]->flag[2] == true)
						{
							Send_Main_Data->data[Player4].flag[2] = true;
						}
						//クリック判定

						if (p_data[Player4]->flag[0] == true && Send_Main_Data->Reverse_flag == false)
						{
							for (int i = INITIALIZE; i < SUIT; i++)
							{
								for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
								{
									if (OFFSET_X + (j * HORIZONTAL_SPACING) < Recv_Data[Player4]->pos.x &&
										OFFSET_X + (j * HORIZONTAL_SPACING) + TRUMP_WIDTH > Recv_Data[Player4]->pos.x &&
										OFFSET_Y + (i * VERTICAL_SPACING) < Recv_Data[Player4]->pos.y &&
										OFFSET_Y + (i * VERTICAL_SPACING) + TRUMP_HEIGHT > Recv_Data[Player4]->pos.y)
									{
										for (int k = INITIALIZE; k < MAX_TRUMP; k++)
										{
											if (All_trump[k]->line_card.x == j && All_trump[k]->line_card.y == i && All_trump[k]->ID == 10 && All_trump[k]->FandB_flag == false)
											{
												All_trump[k]->FandB_flag = true;
												Send_Main_Data->Reverse.x = j;
												Send_Main_Data->Reverse.y = i;
												Send_Main_Data->Reverse_flag = true;
												Save_Trump[Check_count] = k;
												Check_count += 1;
											}
										}
									}
								}
							}
						}

					}
					else
					{
						//初回の接続
						//IPと名前を登録
						p_data[Player4]->ip = ip;
						p_data[Player4]->ID = INITIALIZE;
						memcpy_s(p_data[Player4]->name, sizeof(p_data[Player4]->name), StrBuf, sizeof(p_data[Player4]->name));
						//初回の接続用送信データの更新
						strcpy_s(Send_Set_Data->data[Player4].name, sizeof(p_data[Player4]->name), p_data[Player4]->name);

						Send_Set_Data->data[Player4].ip = p_data[Player4]->ip;//IP
						Send_Set_Data->data[Player4].ID = p_data[Player4]->ID;
						Send_Set_Data->data[Player4].flag[0] = p_data[Player4]->flag[0];

						//二回目以降の送信データの更新
						strcpy_s(Send_Main_Data->data[Player4].name, sizeof(p_data[Player4]->name), p_data[Player4]->name);

						Send_Main_Data->data[Player4].ip = p_data[Player4]->ip;//IP
						Send_Main_Data->data[Player4].ID = p_data[Player4]->ID;
						Send_Main_Data->data[Player4].flag[0] = p_data[Player4]->flag[0];

						//データを送信
						NetWorkSend(p4_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
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



		//テスト用
		/*int Mouse = GetMouseInput();

		if (Mouse & MOUSE_INPUT_RIGHT)
		{
			p_data[0]->flag[0] = true;
		}*/
		//ゲーム開始用のデータのセット
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
				if (i == MAX - 1 && player_check == true)
				{
					//プレイヤーの順番を決める処理
					for (int j = INITIALIZE; j < MAX; j++) {
						if (NetHandle[j] != 0) {

							Join_Player_num += 1;
						}
					}
					GameStart_flag = true;
					Turn_Player_num = Player1;
					p_data[Turn_Player_num]->flag[0] = true;

				}
			}
		}
		//ゲーム中の処理
		if (GameSet_flag == false)
		{
			if (GameStart_flag == true)
			{
				//ターン移行処理
				if (p_data[Turn_Player_num]->flag[0] == false)
				{
					Check_count = INITIALIZE;
					Turn_Player_num += 1;
					if (Turn_Player_num == Join_Player_num)
					{
						Turn_Player_num = Player1;
					}
					p_data[Turn_Player_num]->flag[0] = true;
				}
				//ターンフラグの更新
				for (int i = INITIALIZE; i < MAX; i++)
				{
					if (NetHandle[i] != -1) {
						Send_Main_Data->data[i].flag[0] = p_data[i]->flag[0];
					}
				}

				//クリックフラグをfalseにする
				if (Send_Main_Data->data[Turn_Player_num].flag[1] == true && Check_count != 2)
				{
					Send_Main_Data->data[Turn_Player_num].flag[1] = false;
					//クリックフラグを更新したらデータを送る
					for (int i = INITIALIZE; i < MAX; i++) {
						//データを送信する
						if (NetHandle[i] != -1) {
							NetWorkSend(NetHandle[i], Send_RankData, sizeof(SendRankData));
						}
					}

				}

			



				////送信データの更新
				//for (int i = INITIALIZE; i < MAX_TRUMP; i++)
				//{
				//	Send_Main_Data->trump[i] = *All_trump[i];
				//}
			}

			
			//状況表示
			DrawFormatString(IP_POS_X, IP_POS_Y, GetColor(WHITE),
				"PCのIPアドレス:%d.%d.%d.%d 接続ポート:%d",
				IP.d1,
				IP.d2,
				IP.d3,
				IP.d4,
				Port
			);
			for (int i = INITIALIZE; i < MAX; i++)
			{

				DrawFormatString(IP_DIS_POS_X, i * IP_DIS_POS_Y + 32, GetColor(WHITE),
					"スレッド1 IP:%d.%d.%d.%d name=%8s 枚数=%d",
					p_data[i]->ip.d1,
					p_data[i]->ip.d2,
					p_data[i]->ip.d3,
					p_data[i]->ip.d4,
					p_data[i]->name,
					p_data[i]->count

					);
			}

			if (Check_count == 2)
			{
				for (int i = INITIALIZE; i < MAX; i++) {
					//データを送信する
					if (NetHandle[i] != -1) {
						NetWorkSend(NetHandle[i], Send_Main_Data, sizeof(Send_Main_Data));
					}
				}

				if (All_trump[Save_Trump[0]]->line_card.num == All_trump[Save_Trump[1]]->line_card.num)
				{
					All_trump[Save_Trump[0]]->ID = TRUMP_ERASURE;
					All_trump[Save_Trump[1]]->ID = TRUMP_ERASURE;
					Send_Main_Data->data[Turn_Player_num].count += 2;
					GetCord_num += 2;
					Check_count = INITIALIZE;
					Send_Main_Data->erase_flag = true;
					
				}
				else
				{
					All_trump[Save_Trump[0]]->FandB_flag = false;
					All_trump[Save_Trump[1]]->FandB_flag = false;
					p_data[Turn_Player_num]->flag[0] = false;
					Send_Main_Data->back_flag = true;

				}

				Send_Main_Data->Reverse_flag = false;

				if (GetCord_num == 52)
				{
					GameSet_flag == true;
					Send_Main_Data->end_flag = true;
					for (int i = INITIALIZE; i < MAX; i++) {
						//終了処理
						if (NetHandle[i] != 0) {
							NetWorkSend(NetHandle[i], Send_Main_Data, sizeof(Send_Main_Data));

						}
					}

				}

				WaitTimer(1000);
				for (int i = INITIALIZE; i < MAX; i++) {
					//データを送信する
					if (NetHandle[i] != -1) {
						NetWorkSend(NetHandle[i], Send_Main_Data, sizeof(Send_Main_Data));
					}
				}

				////送信データの更新
				//for (int i = INITIALIZE; i < MAX_TRUMP; i++)
				//{
				//	Send_Main_Data->trump[i] = *All_trump[i];
				//}
						//切断したプレイヤーを初期化
				for (int i = 0; i < MAX; i++)
				{
					if (LostHandle == NetHandle[i]) {
						NetHandle[i] = -1;
						//データの初期化
						p_data[i]->Data_Init();
					}
				}
			}
		}
		if (GameSet_flag == true)
		{
			//データを更新する
			for (int i = INITIALIZE; i < MAX; i++) {
				if (NetHandle[i] != -1) {
					Send_RankData->Rdata.allrank.count[i] = p_data[i]->count;
					RankSort[i]= p_data[i]->count;
					strcpy_s(Send_RankData->Rdata.allrank.name[i], sizeof(Send_RankData->Rdata.allrank.name[i]), p_data[i]->name);
					
				}
			}
			Shaker_Sort(RankSort);

			for (int i = INITIALIZE; i < MAX; i++) {
				if (NetHandle[i] != -1) {
					for (int j = 0; j < 4; j++)
					{
						if (Send_RankData->Rdata.allrank.count[i]==RankSort[j])
						{
							Send_RankData->Rdata.allrank.ranking[i] = j+1;
						}
					}
				}
			}
		

			for (int i = INITIALIZE; i < MAX; i++) {
				//データを送信する
				if (NetHandle[i] != -1) {
					NetWorkSend(NetHandle[i], Send_RankData, sizeof(SendRankData));
				}
			}
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