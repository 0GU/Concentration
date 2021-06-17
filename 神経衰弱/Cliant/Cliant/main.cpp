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
	int img[4][13] = {
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
	char StrBuf[1400] = { "null" };//256バイトまで

	//全てのプレイヤーデータ
	RecvData* Player_ALL = new RecvData();

	//SendTrump* Trump_ALL = new SendTrump();

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
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvData)); //コピー作業
			memcpy_s(Player_ALL, sizeof(RecvData), StrBuf, sizeof(RecvData));//変換

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
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvData));
			//プレイヤー全体データの更新
			memcpy_s(Player_ALL, sizeof(RecvData), StrBuf, sizeof(RecvData));
			int Mouse2 = GetMouseInput();
			if (MOUSE_INPUT_LEFT &Mouse2== 0)
			{
				my_Data->flag[2] = Player_ALL->data[0].flag[1];
			}
		}
		else {
			//データ受信してない場合
			//マウスをクリックしているか判定
			Point p{ 0,0 };
			int Mouse = GetMouseInput();
			if (my_Data->flag[2] == false && Mouse & MOUSE_INPUT_LEFT)
			{
				my_Data->flag[2] = true;
				
				GetMousePoint(&p.x,&p.y);
				NetWorkSend(NetHandel, &p, sizeof(Point));
			}
		}

		//描画
		for (int i = 0; i < 52; i++)
		{
			if (Player_ALL->trump[i].ID == 10)
			{
				//トランプカード描画
				if (Player_ALL->trump[i].FandB_flag==true)
				{
					DrawGraphF( 50 + (Player_ALL->trump[i].line_card.x * 140),
						100 + (Player_ALL->trump[i].line_card.y * 200),
						img[Player_ALL->trump[i].line_card.suit][Player_ALL->trump[i].line_card.num],
						true
					);
				}
				else if (Player_ALL->trump[i].FandB_flag == false)
				{
					DrawGraphF(50 + (Player_ALL->trump[i].line_card.x * 140),
						100 + (Player_ALL->trump[i].line_card.y * 200),
						backimg, true
					);
				}
			}
		}

		for (int i = 0; i < MAX; i++)
		{
			if (Player_ALL->data[i].ID != -1) {
				if (Player_ALL->data[i].name == my_Data->name) {
					DrawStringF(950, 900, Player_ALL->data[i].name, GetColor(255, 255, 255));
				}
				else {
					DrawStringF(0, 0, Player_ALL->data[i].name, GetColor(255, 255, 255));

				}
			}
		}
		Point pd{ 0,0 };
		int Mouse = GetMouseInput();
		GetMousePoint(&pd.x, &pd.y);

		DrawFormatString(0, 0, GetColor(255, 255, 255), "x=%d : y=%d", pd.x, pd.y);



		ScreenFlip(); //画面更新

		//例外処理発生でループを抜ける
		if (ProcessMessage() == -1)break;
	}

	DxLib_End(); //Dxライブラリの開放
	return 0;
}