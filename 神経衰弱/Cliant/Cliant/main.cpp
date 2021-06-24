#pragma once
#include "DxLib.h"
#include "main.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

	//window���[�h�؂�ւ�
	ChangeWindowMode(TRUE);
	//window�T�C�Y
	SetGraphMode(WIDTH, HEIGHT, 32);
	//�o�b�N�O���E���h�����̋���
	SetAlwaysRunFlag(TRUE);
	//���d�N���̋���
	SetDoubleStartValidFlag(TRUE);
	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1) return -1; //���s������v���O�����I��

	//�o�b�N�o�b�t�@���g���ݒ�
	SetDrawScreen(DX_SCREEN_BACK);
	//window�̖��O
	SetWindowText("�_�o����");

	//�摜�ǂݍ���
	int img[SUIT][TRUMP_NUMBER] = {
		{
		   LoadGraph("image\\�g�����v�f��\\clover\\clover1.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover2.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover3.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover4.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover5.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover6.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover7.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover8.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover9.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover10.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover11.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover12.png"),
		   LoadGraph("image\\�g�����v�f��\\clover\\clover13.png")
		},
		{
		   LoadGraph("image\\�g�����v�f��\\spade\\spade1.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade2.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade3.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade4.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade5.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade6.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade7.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade8.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade9.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade10.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade11.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade12.png"),
		   LoadGraph("image\\�g�����v�f��\\spade\\spade13.png")
		},
		{
		   LoadGraph("image\\�g�����v�f��\\dia\\dia1.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia2.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia3.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia4.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia5.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia6.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia7.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia8.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia9.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia10.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia11.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia12.png"),
		   LoadGraph("image\\�g�����v�f��\\dia\\dia13.png")
		},
		{
		   LoadGraph("image\\�g�����v�f��\\heart\\heart1.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart2.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart3.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart4.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart5.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart6.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart7.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart8.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart9.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart10.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart11.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart12.png"),
		   LoadGraph("image\\�g�����v�f��\\heart\\heart13.png")
		}

	};

	int backimg = LoadGraph("image\\�g�����v�f��\\RedBlue.png");



	
	//����M�f�[�^�����p
	char StrBuf[1400] = { "null" };//256�o�C�g�܂�

	//�S�Ẵv���C���[�f�[�^
	RecvData* Player_ALL = new RecvData();

	//���M�f�[�^�p�̃N���X
	SendData* Send_Data = new SendData();

	//SendTrump* Trump_ALL = new SendTrump();

	//�ʐM�֌W
	IPDATA IP;
	//�ʐM�p�̃n���h��
	int NetHandel;
	//�|�[�g
	int Port = 26;
	
	//�ʐM���IP�A�h���X�ݒ�
	IP = IP_set();

	//���O�̓���
	char name[NAME_INPUT_MAX] = " null ";
	ClearDrawScreen();//��ʃN���A
	DrawString(NAME_INPUT_POS_X, NAME_INPUT_POS_Y, "���O����́@������8����/�S�p4�����܂�",
		GetColor(WHITE));
	KeyInputString(NAME_INPUT_ANSWER_POS_X, NAME_INPUT_ANSWER_POS_Y, NAME_INPUT_MAX, name, FALSE);

	bool init_flag[FLAG_MAX] = { false,false,false };

	//���񑗐M�f�[�^�̍쐬
	Data* my_Data = new Data(name,init_flag);

	//����ڑ�����
	NetHandel = ConnectNetWork(IP, Port);//���͂���IP�Ɛݒ肵���|�[�g���g�p

	//�ڑ�����܂őҋ@
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen(); //��ʃN���A

		if (NetHandel != -1) {
			//�ڑ�����
			//�T�[�o�[�Ƀf�[�^�𑗐M
			NetWorkSend(NetHandel, &my_Data->name, sizeof(my_Data->name));
			//�T�[�o�[����̉����ҋ@
			while (!ProcessMessage()) {
				//����M�f�[�^�𒲂ׂ�
				if (GetNetWorkDataLength(NetHandel) != 0) break;
			}
			//��M�����f�[�^��ϊ�
			//��M�f�[�^������������StrBuf�ɃR�s�[���āASendData�ɕϊ�
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvData)); //�R�s�[���
			memcpy_s(Player_ALL, sizeof(RecvData), StrBuf, sizeof(RecvData));//�ϊ�

			DrawString(CONNECTION_CMP_POS_X, CONNECTION_CMP_POS_Y, "�ڑ������B�����L�[�������Ă��������B", GetColor(WHITE));

			ScreenFlip();
			WaitKey();
			break;
		}
		else {
			//�ڑ���
			DrawString(CONNECTION_POS_X, CONNECTION_POS_Y, "�ڑ��m�����E�E�E", GetColor(WHITE));
			
		}
		ScreenFlip();
	}

	//���C�����[�v
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();//��ʂ��N���A

		if (GetNetWorkDataLength(NetHandel) != 0) {
			//�f�[�^����M���Ă��ꍇ
			NetWorkRecv(NetHandel, StrBuf, sizeof(RecvData));
			//�v���C���[�S�̃f�[�^�̍X�V
			memcpy_s(Player_ALL, sizeof(RecvData), StrBuf, sizeof(RecvData));
			my_Data->flag[0] = Player_ALL->data[0].flag[0];
			
			int Mouse2 = GetMouseInput();
			if ((MOUSE_INPUT_LEFT &Mouse2)== 0)
			{
				my_Data->flag[1] = Player_ALL->data[0].flag[1];
			}
		}
		else if (my_Data->flag[2] ==false)
		{
			if (CheckHitKey(KEY_INPUT_S))
			{
				my_Data->flag[2] = true;
				Send_Data->Ready_flag = my_Data->flag[2];
				NetWorkSend(NetHandel, Send_Data, sizeof(SendData));
			}
		}

		else {
			//�f�[�^��M���ĂȂ��ꍇ
			//�}�E�X���N���b�N���Ă��邩����
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

		//�`��
		if (my_Data->flag[2] == false)
		{
			DrawStringF(0, 0, "�ҋ@��", GetColor(WHITE));
		}
		else
		{
			for (int i = INITIALIZE; i < TRUMP_MAX; i++)
			{
				if (Player_ALL->trump[i].ID == 10)
				{
					//�g�����v�J�[�h�`��
					if (Player_ALL->trump[i].FandB_flag == true)
					{
						DrawGraphF(OFFSET_X + (Player_ALL->trump[i].line_card.x * HORIZONTAL_SPACING),
							OFFSET_Y + (Player_ALL->trump[i].line_card.y * VERTICAL_SPACING),
							img[Player_ALL->trump[i].line_card.suit][Player_ALL->trump[i].line_card.num],
							true
						);
					}
					else if (Player_ALL->trump[i].FandB_flag == false)
					{
						DrawGraphF(OFFSET_X + (Player_ALL->trump[i].line_card.x * HORIZONTAL_SPACING),
							OFFSET_Y + (Player_ALL->trump[i].line_card.y * VERTICAL_SPACING),
							backimg, true
						);
					}
				}
			}

			for (int i = INITIALIZE; i < MAX; i++)
			{
				if (Player_ALL->data[i].ID != -1) {
					if (Player_ALL->data[i].ip.d1 == my_Data->ip.d1&&
						Player_ALL->data[i].ip.d2 == my_Data->ip.d2&&
						Player_ALL->data[i].ip.d3 == my_Data->ip.d3&&
						Player_ALL->data[i].ip.d4 == my_Data->ip.d4) {
						DrawStringF(950, 900, Player_ALL->data[i].name, GetColor(WHITE));
					}
					else {
						DrawStringF(0, 0, Player_ALL->data[i].name, GetColor(WHITE));

					}
				}
			}
		}
		

		
		Point pd{ INITIALIZE,INITIALIZE };
		int Mouse = GetMouseInput();
		GetMousePoint(&pd.x, &pd.y);

		DrawFormatString(0, 0, GetColor(WHITE), "x=%d : y=%d", pd.x, pd.y);



		ScreenFlip(); //��ʍX�V

		//��O���������Ń��[�v�𔲂���
		if (ProcessMessage() == -1)break;
	}

	DxLib_End(); //Dx���C�u�����̊J��
	return 0;
}