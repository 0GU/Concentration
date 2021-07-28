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
	int img[SUIT][TRUMP_NUMBER] = {
		{
		   LoadGraph("image\\トランプ素材\\clover\\clover1.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover2.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover3.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover4.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover5.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover6.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover7.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover8.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover9.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover10.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover11.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover12.png"),
		   LoadGraph("image\\トランプ素材\\clover\\clover13.png")
		},
		{
		   LoadGraph("image\\トランプ素材\\spade\\spade1.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade2.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade3.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade4.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade5.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade6.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade7.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade8.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade9.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade10.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade11.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade12.png"),
		   LoadGraph("image\\トランプ素材\\spade\\spade13.png")
		},
		{
		   LoadGraph("image\\トランプ素材\\dia\\dia1.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia2.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia3.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia4.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia5.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia6.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia7.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia8.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia9.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia10.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia11.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia12.png"),
		   LoadGraph("image\\トランプ素材\\dia\\dia13.png")
		},
		{
		   LoadGraph("image\\トランプ素材\\heart\\heart1.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart2.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart3.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart4.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart5.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart6.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart7.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart8.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart9.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart10.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart11.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart12.png"),
		   LoadGraph("image\\トランプ素材\\heart\\heart13.png")
		}

	};

	int backimg = LoadGraph("image\\トランプ素材\\RedBlue.png");



	
	//送受信データ処理用
	char StrBuf[2000] = { "null" };//256バイトまで

	//全てのプレイヤーデータ
	RecvSetData* Player_Set_ALL = new RecvSetData();

	RecvMainData* Player_Main_ALL = new RecvMainData();

	RecvRankData* Player_Rank_ALL = new RecvRankData();

	Data* p_data[MAX];

	bool End_flag = false;

	for (int i = INITIALIZE; i < MAX; i++)p_data[i] = new Data();
	Trump* All_trump[TRUMP_MAX];//トランプ用配列
	//送信データ用のクラス
	SendData* Send_Data = new SendData();

	RecvRankData* Rank_Data = new RecvRankData();
	
	//マイナンバー
	int Mynumber=99;

	//通信関係
	IPDATA IP;
	//通信用のハンドル
	int NetHandel;
	//ポート
	int Port = 26;

	//通信用のフラグ
	bool Conect_comp_flag = false;
	
	//通信先のIPアドレス設定
	IP = IP_set();

	//名前の入力
	char name[NAME_INPUT_MAX] = " null ";
	ClearDrawScreen();//画面クリア
	DrawString(NAME_INPUT_POS_X, NAME_INPUT_POS_Y, "名前を入力　小文字8文字/全角4文字まで",
		GetColor(WHITE));
	KeyInputString(NAME_INPUT_ANSWER_POS_X, NAME_INPUT_ANSWER_POS_Y, NAME_INPUT_MAX, name, FALSE);

	bool init_flag[FLAG_MAX] = { false,false,false };

	//初回送信データの作成
	Data* my_Data = new Data(name,init_flag);

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
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvSetData)); //コピー作業
			memcpy_s(Player_Set_ALL, sizeof(RecvSetData), StrBuf, sizeof(RecvSetData));//変換

			DrawString(CONNECTION_CMP_POS_X, CONNECTION_CMP_POS_Y, "接続完了。何かキーを押してください。", GetColor(WHITE));

			ScreenFlip();
			WaitKey();
			///接続用（バグってる）
		
			do
			{
				if (GetNetWorkDataLength(NetHandel) != 0) {
					//データを受信してた場合
					NetWorkRecv(NetHandel, StrBuf, sizeof(RecvSetData)); //コピー作業
					memcpy_s(Player_Set_ALL, sizeof(RecvSetData), StrBuf, sizeof(RecvSetData));//変換


					for (int i = INITIALIZE; i < MAX; i++)
					{
						if (strcmp(Player_Set_ALL->data[i].name, my_Data->name) == 0)
						{
							Mynumber = i;

							if (Player_Set_ALL->data[Mynumber].flag[2] == true)
							{
								Conect_comp_flag = true;
							}
						}
					}
				} 
				else {
					//データ受信してない場合
					my_Data->flag[2] = true;
					Send_Data->Ready_flag = my_Data->flag[2];
					NetWorkSend(NetHandel, Send_Data, sizeof(SendData));
				}
			} while (Conect_comp_flag==false);
				
			//データの登録
			for (int i = INITIALIZE; i < TRUMP_MAX; i++)
			{
				*All_trump[i]=Player_Set_ALL->trump[i];
			}
			for (int j = INITIALIZE; j < MAX; j++)
			{
				*p_data[j] = Player_Set_ALL->data[j];
			}			


			break;
		}
		else {
			//接続中
			DrawString(CONNECTION_POS_X, CONNECTION_POS_Y, "接続確立中・・・", GetColor(WHITE));
			
		}
		ScreenFlip();
	}

	//メインループ
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0&&End_flag==false) {
		ClearDrawScreen();//画面をクリア

		if (GetNetWorkDataLength(NetHandel) != 0) {
			//データを受信してた場合
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvMainData));
			//プレイヤー全体データの更新
			memcpy_s(Player_Main_ALL, sizeof(RecvMainData), StrBuf, sizeof(RecvMainData));

			my_Data->flag[0] = Player_Main_ALL->data[Mynumber].flag[0];

			//トランプデータの更新
			if (Player_Main_ALL->Reverse_flag == true)
			{
				for (int k = INITIALIZE; k < TRUMP_MAX; k++)
				{
					if (All_trump[k]->line_card.x == Player_Main_ALL->Reverse.x && All_trump[k]->line_card.y == Player_Main_ALL->Reverse.y)
					{
						All_trump[k]->FandB_flag = true;
					}
				}
			}
			else if(Player_Main_ALL->back_flag==true)
			{
				for (int k = INITIALIZE; k < TRUMP_MAX; k++)
				{
					if (All_trump[k]->FandB_flag==true)
					{
						All_trump[k]->FandB_flag = false;
					}
				}
			}
			else if (Player_Main_ALL->erase_flag==true)
			{
				for (int k = INITIALIZE; k < TRUMP_MAX; k++)
				{
					if (All_trump[k]->FandB_flag == true)
					{
						All_trump[k]->ID = 99;
					}
				}
			}

			/*int Mouse2 = GetMouseInput();
			if ((MOUSE_INPUT_LEFT &Mouse2)== 0)
			{
				my_Data->flag[1] = Player_Main_ALL->data[Mynumber].flag[1];
			}*/
		}
		

		else {
			//データ受信してない場合
			//マウスをクリックしているか判定
			Point p{ INITIALIZE,INITIALIZE };
			int Mouse = GetMouseInput();
			if (my_Data->flag[1] == false && Mouse & MOUSE_INPUT_LEFT)
			{
				my_Data->flag[1] = true;
				Send_Data->turn_flag = my_Data->flag[0];
				
				GetMousePoint(&p.x,&p.y);
				Send_Data->pos = p;
				NetWorkSend(NetHandel, Send_Data, sizeof(SendData));
			}
		}

		//描画
		if (my_Data->flag[2] == false)
		{
			DrawStringF(0, 0, "待機中", GetColor(WHITE));
		}
		else
		{
			for (int i = INITIALIZE; i < TRUMP_MAX; i++)
			{
				if (All_trump[i]->ID == 10)
				{
					//トランプカード描画
					if (All_trump[i]->FandB_flag == true)
					{
						DrawGraphF(OFFSET_X + (All_trump[i]->line_card.x * HORIZONTAL_SPACING),
							OFFSET_Y + (All_trump[i]->line_card.y * VERTICAL_SPACING),
							img[All_trump[i]->line_card.suit][All_trump[i]->line_card.num],
							true
						);
					}
					else if (All_trump[i]->FandB_flag == false)
					{
						DrawGraphF(OFFSET_X + (All_trump[i]->line_card.x * HORIZONTAL_SPACING),
							OFFSET_Y + (All_trump[i]->line_card.y * VERTICAL_SPACING),
							backimg, true
						);
					}
				}
			}

			int playernamenum = 0;

			for (int i = INITIALIZE; i < MAX; i++)
			{
				if (Player_Main_ALL->data[i].ID != -1) {
					if (Mynumber==i) {
						DrawStringF(950, 900, Player_Main_ALL->data[i].name, GetColor(WHITE));
						DrawFormatStringF(1000, 900, GetColor(WHITE), "%d枚" , Player_Main_ALL->data[i].count);
					}

					else {
						switch (playernamenum)
						{
						case 0:
							DrawStringF(300, 35, Player_Main_ALL->data[i].name, GetColor(RED));
							DrawFormatStringF(400, 35, GetColor(WHITE), "%d枚", Player_Main_ALL->data[i].count);
							break;

						case 1:
							DrawStringF(950, 35, Player_Main_ALL->data[i].name, GetColor(BLUE));
							DrawFormatStringF(1000, 35, GetColor(WHITE), "%d枚", Player_Main_ALL->data[i].count);
							break;

						case 2:
							DrawStringF(1600, 35, Player_Main_ALL->data[i].name, GetColor(GREEN));
							DrawFormatStringF(1700, 35, GetColor(WHITE), "%d枚", Player_Main_ALL->data[i].count);
							break;
							
						default:
							break;
						}
					}
				}
			}
		}
		
		Point pd{ INITIALIZE,INITIALIZE };

		int Mouse = GetMouseInput();
		GetMousePoint(&pd.x, &pd.y);

		DrawFormatString(0, 0, GetColor(WHITE), "x=%d : y=%d", pd.x, pd.y);

		ScreenFlip(); //画面更新

		//例外処理発生でループを抜ける
		if (ProcessMessage() == -1)break;
	}

	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 ) {

		ClearDrawScreen();//画面をクリア

		if (GetNetWorkDataLength(NetHandel) != 0) {
			//データを受信してた場合
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvRankData));
			//プレイヤー全体データの更新
			memcpy_s(Rank_Data, sizeof(RecvRankData), StrBuf, sizeof(RecvRankData));

		}

		for (int  i = 0; i < 4; i++)
		{
			switch (Rank_Data->Rdata.allrank.ranking[i])
			{
			case 1:
				SetFontSize(128);
				DrawStringF(300, 35, Player_Rank_ALL->Rdata.allrank.name[i], GetColor(RED));
				DrawFormatStringF(200, 100, GetColor(WHITE), "%d位	%8s		獲得枚数：%d枚",
					Rank_Data->Rdata.allrank.ranking[i], Rank_Data->Rdata.allrank.name[i],
					Rank_Data->Rdata.allrank.count[i]);
				break;

			case 2:
				SetFontSize(64);
				DrawStringF(950, 35, Player_Rank_ALL->Rdata.allrank.name[i], GetColor(BLUE));
				DrawFormatStringF(264,300 , GetColor(WHITE), "%d位	%8s		獲得枚数：%d枚",
					Rank_Data->Rdata.allrank.ranking[i], Rank_Data->Rdata.allrank.name[i],
					Rank_Data->Rdata.allrank.count[i]);
				break;

			case 3:
				SetFontSize(32);
				DrawStringF(1600, 35, Player_Rank_ALL->Rdata.allrank.name[i], GetColor(GREEN));
				DrawFormatStringF(296, 450, GetColor(WHITE), "%d位	%8s		獲得枚数：%d枚",
					Rank_Data->Rdata.allrank.ranking[i], Rank_Data->Rdata.allrank.name[i],
					Rank_Data->Rdata.allrank.count[i]);
				break;
			case 4:
				SetFontSize(16);
				DrawStringF(1600, 35, Player_Rank_ALL->Rdata.allrank.name[i], GetColor(GREEN));
				DrawFormatStringF(312, 550, GetColor(WHITE), "%d位	%8s		獲得枚数：%d枚",
					Rank_Data->Rdata.allrank.ranking[i], Rank_Data->Rdata.allrank.name[i],
					Rank_Data->Rdata.allrank.count[i]);
				break;

			default:
				break;
			}
		}



		ScreenFlip(); //画面更新

	//例外処理発生でループを抜ける
		if (ProcessMessage() == -1)break;
	}


	DxLib_End(); //Dxライブラリの開放
	return 0;
}