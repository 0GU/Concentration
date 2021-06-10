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
	int img[4][13] = {
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

	//����M�f�[�^�����p
	char StrBuf[256] = { "null" };//256�o�C�g�܂�

	//�S�Ẵv���C���[�f�[�^
	SendData* Player_ALL = new SendData();

	//�ʐM�֌W
	IPDATA IP;
	//�ʐM�p�̃n���h��
	int NetHandel;
	//�|�[�g
	int Port = 26;

	//�ʐM���IP�A�h���X�ݒ�
	IP = IP_set();

	//���O�̓���
	char name[8] = " null ";
	ClearDrawScreen();//��ʃN���A
	DrawString(0, 0, "���O����́@������8����/�S�p4�����܂�",
		GetColor(255, 255, 255));
	KeyInputString(0, 16, 8, name, FALSE);

	//���񑗐M�f�[�^�̍쐬
	Data* my_Data = new Data(0.0f, 0.0f, name);

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
			NetWorkRecv(NetHandel, StrBuf, sizeof(SendData)); //�R�s�[���
			memcpy_s(Player_ALL, sizeof(SendData), StrBuf, sizeof(SendData));//�ϊ�

			DrawString(0, 16, "�ڑ������B�����L�[�������Ă��������B", GetColor(255, 255, 255));

			ScreenFlip();
			WaitKey();
			break;
		}
		else {
			//�ڑ���
			DrawString(0, 0, "�ڑ��m�����E�E�E", GetColor(255, 255, 255));
		}
		ScreenFlip();
	}

	//���C�����[�v
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		ClearDrawScreen();//��ʂ��N���A

		if (GetNetWorkDataLength(NetHandel) != 0) {
			//�f�[�^����M���Ă��ꍇ
			NetWorkRecv(NetHandel, StrBuf, sizeof(SendData));
			//�v���C���[�S�̃f�[�^�̍X�V
			memcpy_s(Player_ALL, sizeof(SendData), StrBuf, sizeof(SendData));
		}
		else {
			//�f�[�^��M���ĂȂ��ꍇ
		}

		ScreenFlip(); //��ʍX�V

		//��O���������Ń��[�v�𔲂���
		if (ProcessMessage() == -1)break;
	}

	DxLib_End(); //Dx���C�u�����̊J��
	return 0;
}