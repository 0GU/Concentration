//�}���`�X���b�hTCP�ʐM
//�T�[�o�[
//Dx���C�u�����g�p
#pragma once
#include "main.h"
#include <thread>
#include <iostream>
using namespace std;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	//window���[�h�̐؂�ւ�
	ChangeWindowMode(TRUE);
	//window�T�C�Y
	SetGraphMode(WIDTH, HEIGHT, 32);
	//�o�b�N�O���E���h�����̋���
	SetAlwaysRunFlag(TRUE);
	//���d�N���̋���
	SetDoubleStartValidFlag(TRUE);

	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1)return -1;//���s������v���O�����I��

	//window�̖��O
	SetWindowText("�T�[�o�[");

	//�o�b�N�o�b�t�@���g�p
	SetDrawScreen(DX_SCREEN_BACK);

	//����M�f�[�^�����p
	char StrBuf[256] = { "NULL" };

	//�v���C���[�f�[�^
	Data* p_data[MAX];
	for (int i = 0; i < MAX; i++)p_data[i] = new Data();

	//���M�p�f�[�^
	SendData* Send_Data = new SendData();

	//�l�b�g���[�N�֌W
	IPDATA IP;
	int Port = 26;
	int NetHandle[4] = { 0 };

	///////////

	//�ڑ��ҋ@��Ԃɂ���
	PreparationListenNetWork(Port);

	//�T�u�X���b�h
	//p_data[0]
	thread* p1 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p1_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p1_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
				if (p1_NetHandle != -1)
				{
					NetHandle[0] = p1_NetHandle;
					break;
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p1_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p1_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p1_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[0]->ip.d1 == ip.d1 &&
						p_data[0]->ip.d2 == ip.d2 &&
						p_data[0]->ip.d3 == ip.d3 &&
						p_data[0]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�
						
						//��M�f�[�^��ϊ�
					
						//���M�f�[�^�̍X�V
						Send_Data->data[0].pos.x = p_data[0]->pos.x;
						Send_Data->data[0].pos.y = p_data[0]->pos.y;
					}
					else
					{
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[0]->ip = ip;
						p_data[0]->ID = 0;
						memcpy_s(p_data[0]->name, sizeof(p_data[0]->name), StrBuf, sizeof(p_data[0]->name));
						//���M�f�[�^�̍X�V
						strcpy_s(Send_Data->data[0].name, sizeof(p_data[0]->name), p_data[0]->name);
						Send_Data->data[0].pos = p_data[0]->pos;//�ʒu
						Send_Data->data[0].ip = p_data[0]->ip;//IP
						Send_Data->data[0].ID = p_data[0]->ID;

						//�f�[�^�𑗐M
						NetWorkSend(p1_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//p_data[1]
	thread* p2 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p2_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p2_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
				if (p2_NetHandle != -1)
				{
					NetHandle[1] = p2_NetHandle;
					break;
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p2_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p2_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p2_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[1]->ip.d1 == ip.d1 &&
						p_data[1]->ip.d2 == ip.d2 &&
						p_data[1]->ip.d3 == ip.d3 &&
						p_data[1]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�
					
						//��M�f�[�^��ϊ�
					
						//�ړ�����
					
						//���M�f�[�^�̍X�V
						Send_Data->data[1].pos.x = p_data[1]->pos.x;
						Send_Data->data[1].pos.y = p_data[1]->pos.y;
					}
					else
					{
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[1]->ip = ip;
						p_data[1]->ID = 0;
						memcpy_s(p_data[1]->name, sizeof(p_data[1]->name), StrBuf, sizeof(p_data[1]->name));
						//���M�f�[�^�̍X�V
						strcpy_s(Send_Data->data[1].name, sizeof(p_data[1]->name), p_data[1]->name);
						Send_Data->data[1].pos = p_data[1]->pos;//�ʒu
						Send_Data->data[1].ip = p_data[1]->ip;//IP
						Send_Data->data[1].ID = p_data[1]->ID;

						//�f�[�^�𑗐M
						NetWorkSend(p2_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//p_data[2]
	thread* p3 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p3_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p3_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
				if (p3_NetHandle != -1)
				{
					NetHandle[2] = p3_NetHandle;
					break;
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p3_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p3_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p3_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[2]->ip.d1 == ip.d1 &&
						p_data[2]->ip.d2 == ip.d2 &&
						p_data[2]->ip.d3 == ip.d3 &&
						p_data[2]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�
					
						//��M�f�[�^��ϊ�
					
						//�ړ�����
						
						//���M�f�[�^�̍X�V
						Send_Data->data[2].pos.x = p_data[2]->pos.x;
						Send_Data->data[2].pos.y = p_data[2]->pos.y;
					}
					else
					{
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[2]->ip = ip;
						p_data[2]->ID = 0;
						memcpy_s(p_data[2]->name, sizeof(p_data[2]->name), StrBuf, sizeof(p_data[2]->name));
						//���M�f�[�^�̍X�V
						strcpy_s(Send_Data->data[2].name, sizeof(p_data[2]->name), p_data[2]->name);
						Send_Data->data[2].pos = p_data[2]->pos;//�ʒu
						Send_Data->data[2].ip = p_data[2]->ip;//IP
						Send_Data->data[2].ID = p_data[2]->ID;

						//�f�[�^�𑗐M
						NetWorkSend(p3_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//p_data[3]
	thread* p4 = new thread([&]()
		{
			IPDATA ip{ 0,0,0,0 };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p4_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				p4_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
				if (p4_NetHandle != -1)
				{
					NetHandle[3] = p4_NetHandle;
					break;
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				DataLength = GetNetWorkDataLength(p4_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p4_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p4_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[3]->ip.d1 == ip.d1 &&
						p_data[3]->ip.d2 == ip.d2 &&
						p_data[3]->ip.d3 == ip.d3 &&
						p_data[3]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�
						
						//��M�f�[�^��ϊ�
					
						//�ړ�����
					
						//���M�f�[�^�̍X�V
						Send_Data->data[3].pos.x = p_data[3]->pos.x;
						Send_Data->data[3].pos.y = p_data[3]->pos.y;
					}
					else
					{
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[3]->ip = ip;
						p_data[3]->ID = 0;
						memcpy_s(p_data[3]->name, sizeof(p_data[3]->name), StrBuf, sizeof(p_data[3]->name));
						//���M�f�[�^�̍X�V
						strcpy_s(Send_Data->data[3].name, sizeof(p_data[3]->name), p_data[3]->name);
						Send_Data->data[3].pos = p_data[3]->pos;//�ʒu
						Send_Data->data[3].ip = p_data[3]->ip;//IP
						Send_Data->data[3].ID = p_data[3]->ID;

						//�f�[�^�𑗐M
						NetWorkSend(p4_NetHandle, Send_Data, sizeof(SendData));
					}
				}
			}
		}
	);

	//���C�����[�v(�T�[�o�[�Ǘ����)
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();//��ʃN���A
		//�N���[����IP�A�h���X
		GetMyIPAddress(&IP, 1, NULL);

		//�ؒf�󋵂��`�F�b�N
		int LostHandle = GetLostNetWork();


		for (int i = 0; i < MAX; i++) {
			//�f�[�^�𑗐M����
			if (NetHandle[i]!=-1){
				NetWorkSend(NetHandle[i], Send_Data, sizeof(SendData));
			}

			//�ؒf�����v���C���[��������
			if (LostHandle == NetHandle[i]) {
				NetHandle[i] = -1;
				//�f�[�^�̏�����
				p_data[i]->Data_Init();
			}
		}
		//�󋵕\��
		DrawFormatString(0, 0, GetColor(255, 255, 255),
			"PC��IP�A�h���X:%d.%d.%d.%d �ڑ��|�[�g:%d",
			IP.d1,
			IP.d2,
			IP.d3,
			IP.d4,
			Port
		);
		for (int i = 0; i < MAX; i++)
		{
			DrawFormatString(0, i*16+32, GetColor(255, 255, 255),
				"�X���b�h1 IP:%d.%d.%d.%d name=%8s x=%f:y=%f",
				p_data[i]->ip.d1,
				p_data[i]->ip.d2,
				p_data[i]->ip.d3,
				p_data[i]->ip.d4,
				p_data[i]->name,
				p_data[i]->pos.x,
				p_data[i]->pos.y
			);
		}
		
		ScreenFlip();//��ʍX�V
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