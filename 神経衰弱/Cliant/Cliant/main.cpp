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