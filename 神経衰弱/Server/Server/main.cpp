//�}���`�X���b�hTCP�ʐM
//�T�[�o�[
//Dx���C�u�����g�p
#pragma once
#include "main.h"

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
	for (int i = INITIALIZE; i < MAX; i++)p_data[i] = new Data();

	//�g�����v�f�[�^
	Point SetTrumpPos[MAX_TRUMP];////�Z�b�g����Ă�����W�m�F�p�z��
	bool setposflag = false;//�Đݒ�p�t���O
	//������
	for (int i = INITIALIZE; i < MAX_TRUMP; i++)
	{
		SetTrumpPos[i] = { TRUMP_INIT,TRUMP_INIT };
	}

	Trump* All_trump[MAX_TRUMP];//�g�����v�p�z��
	//�����ƃX�[�g�̐ݒ�
	for (int i = INITIALIZE; i < SUIT; i++)
	{
		for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
		{
			All_trump[(i * TRUMP_NUMBER) + j] = new Trump();
			All_trump[(i * TRUMP_NUMBER) + j]->line_card.num = j;
			All_trump[(i * TRUMP_NUMBER) + j]->line_card.suit = i;
		}
	}
	//�e�g�����v�̍��W����
	for (int i = INITIALIZE; i < SUIT; i++)
	{
		for (int j = INITIALIZE; j < TRUMP_NUMBER; j++)
		{
			do
			{
				setposflag = false;//�Đݒ�p�t���O��false�ɂ���
				All_trump[(i * TRUMP_NUMBER) + j]->line_card.x = GetRand(12);//x���W�̌���
				All_trump[(i * TRUMP_NUMBER) + j]->line_card.y = GetRand(3);//y���W�̌���
				for (int k = INITIALIZE; k < (i * TRUMP_NUMBER) + j; k++)
				{
					//���肵�����W�ɑ��̃J�[�h�����ɃZ�b�g����Ă���ꍇ�A�Đݒ�p�t���O��true�ɂ���
					if (SetTrumpPos[k].x == All_trump[(i * TRUMP_NUMBER) + j]->line_card.x &&
						SetTrumpPos[k].y == All_trump[(i * TRUMP_NUMBER) + j]->line_card.y)
					{
						setposflag = true; //�Đݒ�p�t���O��true�ɂ���
					}
				}
			} while (/*(i * TRUMP_NUMBER) + j != 0 &&*/ setposflag == true);
			SetTrumpPos[(i * TRUMP_NUMBER) + j].x = All_trump[(i * TRUMP_NUMBER) + j]->line_card.x;//�Z�b�g����Ă�����W�m�F�p�z���x���W��ۑ�����
			SetTrumpPos[(i * TRUMP_NUMBER) + j].y = All_trump[(i * TRUMP_NUMBER) + j]->line_card.y;//�Z�b�g����Ă�����W�m�F�p�z���y���W��ۑ�����
		}
	}

	//�o�^���M�p�f�[�^
	SendSetData* Send_Set_Data = new SendSetData();
	for (int i = INITIALIZE; i < MAX_TRUMP; i++)
	{
		Send_Set_Data->trump[i] = *All_trump[i];
	}
	//���C�����M�p�f�[�^
	SendMainData* Send_Main_Data = new SendMainData();
	for (int i = 0; i < MAX; i++)
	{
		Send_Main_Data->data[i].Data_Init();
	}
	

	//�����L���O���M�p�f�[�^
	SendRankData* Send_RankData = new SendRankData();
	Send_RankData->Rdata.Rank_Data_Init();
	//���ʃ\�[�g�p�̔z��
	int RankSort[4];
	//������
	for (int i = INITIALIZE; i < 4; i++)
	{
			RankSort[i] = -1;
	}


	//��M�p�f�[�^
	RecvData* Recv_Data[MAX];
	for (int i = INITIALIZE; i < MAX; i++)
	{
		Recv_Data[i] = new RecvData();
	}

	//�l�b�g���[�N�֌W
	IPDATA IP;
	int Port = 26;
	int NetHandle[MAX] = { INITIALIZE };

	//�g�����v�`�F�b�N�p�ϐ�
	int Check_count = INITIALIZE;
	int Save_Trump[2] = { INITIALIZE,INITIALIZE };
	int GetCord_num = 0;

	//�Q�[���J�n/�I���p�t���O
	bool GameStart_flag = false;
	bool GameSet_flag = false;
	bool player_check = false;

	//�^�[�������p�ϐ�
	int Turn_Player_num = INITIALIZE;//�^�[���v���C���[���ʗp
	int Join_Player_num = INITIALIZE;//�Q���l���ۑ��p
	///////////

	//�ڑ��ҋ@��Ԃɂ���
	PreparationListenNetWork(Port);

	//�T�u�X���b�h
	//p_data[0]
	thread* p1 = new thread([&]()
		{
			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p1_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				p1_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
				if (p1_NetHandle != -1)
				{
					NetHandle[Player1] = p1_NetHandle;
					break;
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p1_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p1_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p1_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[Player1]->ip.d1 == ip.d1 &&
						p_data[Player1]->ip.d2 == ip.d2 &&
						p_data[Player1]->ip.d3 == ip.d3 &&
						p_data[Player1]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�

						//��M�f�[�^��ϊ�
						memcpy_s(Recv_Data[Player1], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player1]->flag[2] = Recv_Data[Player1]->Ready_flag;
						if (p_data[Player1]->flag[2] == true)
						{
							Send_Main_Data->data[Player1].flag[2] = true;
						}

						//�N���b�N����

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
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[Player1]->ip = ip;
						p_data[Player1]->ID = INITIALIZE;
						memcpy_s(p_data[Player1]->name, sizeof(p_data[Player1]->name), StrBuf, sizeof(p_data[Player1]->name));
						//����̐ڑ��p���M�f�[�^�̍X�V
						strcpy_s(Send_Set_Data->data[Player1].name, sizeof(p_data[Player1]->name), p_data[Player1]->name);

						Send_Set_Data->data[Player1].ip = p_data[Player1]->ip;//IP
						Send_Set_Data->data[Player1].ID = p_data[Player1]->ID;
						Send_Set_Data->data[Player1].flag[0] = p_data[Player1]->flag[0];
						//���ڈȍ~�̑��M�f�[�^�̍X�V
						strcpy_s(Send_Set_Data->data[Player1].name, sizeof(p_data[Player1]->name), p_data[Player1]->name);

						Send_Main_Data->data[Player1].ip = p_data[Player1]->ip;//IP
						Send_Main_Data->data[Player1].ID = p_data[Player1]->ID;
						Send_Main_Data->data[Player1].flag[0] = p_data[Player1]->flag[0];
						//�f�[�^�𑗐M
						NetWorkSend(p1_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
					}
				}
			}
		}
	);

	//p_data[1]
	thread* p2 = new thread([&]()
		{

			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p2_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
			{
				if (NetHandle[Player1] != 0)
				{
					p2_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
					if (p2_NetHandle != -1)
					{
						NetHandle[Player2] = p2_NetHandle;
						break;
					}
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p2_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p2_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p2_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[Player2]->ip.d1 == ip.d1 &&
						p_data[Player2]->ip.d2 == ip.d2 &&
						p_data[Player2]->ip.d3 == ip.d3 &&
						p_data[Player2]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�

						//��M�f�[�^��ϊ�
						memcpy_s(Recv_Data[Player2], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player2]->flag[2] = Recv_Data[Player2]->Ready_flag;
						if (p_data[Player2]->flag[2] == true)
						{
							Send_Main_Data->data[Player2].flag[2] = true;
						}
						//�N���b�N����

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
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[Player2]->ip = ip;
						p_data[Player2]->ID = INITIALIZE;
						memcpy_s(p_data[Player2]->name, sizeof(p_data[Player2]->name), StrBuf, sizeof(p_data[Player2]->name));
						//����̐ڑ��p���M�f�[�^�̍X�V
						strcpy_s(Send_Set_Data->data[Player2].name, sizeof(p_data[Player2]->name), p_data[Player2]->name);

						Send_Set_Data->data[Player2].ip = p_data[Player2]->ip;//IP
						Send_Set_Data->data[Player2].ID = p_data[Player2]->ID;
						Send_Set_Data->data[Player2].flag[0] = p_data[Player2]->flag[0];

						//���ڈȍ~�̑��M�f�[�^�̍X�V
						strcpy_s(Send_Main_Data->data[Player2].name, sizeof(p_data[Player2]->name), p_data[Player2]->name);

						Send_Main_Data->data[Player2].ip = p_data[Player2]->ip;//IP
						Send_Main_Data->data[Player2].ID = p_data[Player2]->ID;
						Send_Main_Data->data[Player2].flag[0] = p_data[Player2]->flag[0];

						//�f�[�^�𑗐M
						NetWorkSend(p2_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
					}
				}
			}
		}
	);

	//p_data[2]
	thread* p3 = new thread([&]()
		{


			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p3_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				if (NetHandle[Player2] != 0)
				{
					p3_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
					if (p3_NetHandle != -1)
					{
						NetHandle[Player3] = p3_NetHandle;
						break;
					}
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p3_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p3_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p3_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[Player3]->ip.d1 == ip.d1 &&
						p_data[Player3]->ip.d2 == ip.d2 &&
						p_data[Player3]->ip.d3 == ip.d3 &&
						p_data[Player3]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�

						//��M�f�[�^��ϊ�
						memcpy_s(Recv_Data[Player3], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player3]->flag[2] = Recv_Data[Player3]->Ready_flag;
						if (p_data[Player3]->flag[2] == true)
						{
							Send_Main_Data->data[Player3].flag[2] = true;
						}
						//�N���b�N����

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
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[Player3]->ip = ip;
						p_data[Player3]->ID = INITIALIZE;
						memcpy_s(p_data[Player3]->name, sizeof(p_data[Player3]->name), StrBuf, sizeof(p_data[Player3]->name));
						//����̐ڑ��p���M�f�[�^�̍X�V
						strcpy_s(Send_Set_Data->data[Player3].name, sizeof(p_data[Player3]->name), p_data[Player3]->name);

						Send_Set_Data->data[Player3].ip = p_data[Player3]->ip;//IP
						Send_Set_Data->data[Player3].ID = p_data[Player3]->ID;
						Send_Set_Data->data[Player3].flag[0] = p_data[Player3]->flag[0];

						//���ڈȍ~�̑��M�f�[�^�̍X�V
						strcpy_s(Send_Main_Data->data[Player3].name, sizeof(p_data[Player3]->name), p_data[Player3]->name);

						Send_Main_Data->data[Player3].ip = p_data[Player3]->ip;//IP
						Send_Main_Data->data[Player3].ID = p_data[Player3]->ID;
						Send_Main_Data->data[Player3].flag[0] = p_data[Player3]->flag[0];

						//�f�[�^�𑗐M
						NetWorkSend(p3_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
					}
				}
			}
		}
	);

	//p_data[3]
	thread* p4 = new thread([&]()
		{

			IPDATA ip{ INITIALIZE,INITIALIZE,INITIALIZE,INITIALIZE };//IP�A�h���X
			int DataLength = -1;//��M�f�[�^�̑傫���擾�p
			int p4_NetHandle = -1;//�l�b�g���[�N�n���h��
			char StrBuf[256]{ "null" };//����M�f�[�^�p

			//����ڑ�����
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				if (NetHandle[Player3] != 0)
				{
					p4_NetHandle = GetNewAcceptNetWork();//�l�b�g���[�N�n���h���擾
					if (p4_NetHandle != -1)
					{
						NetHandle[Player4] = p4_NetHandle;
						break;
					}
				}
			}

			//�T�u�X���b�h�̃��C�����[�v
			while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && GameSet_flag == false)
			{
				DataLength = GetNetWorkDataLength(p4_NetHandle);
				if (DataLength != 0)
				{
					//��M�f�[�^��StrBuf�Ɏ擾
					NetWorkRecv(p4_NetHandle, StrBuf, DataLength);
					//�ڑ����Ă����}�V����Ip�A�h���X���擾
					GetNetWorkIP(p4_NetHandle, &ip);

					//IP�A�h���X���珉��̐ڑ����m�F
					if (p_data[Player4]->ip.d1 == ip.d1 &&
						p_data[Player4]->ip.d2 == ip.d2 &&
						p_data[Player4]->ip.d3 == ip.d3 &&
						p_data[Player4]->ip.d4 == ip.d4)
					{
						//2��ڈȍ~�̐ڑ�

						//��M�f�[�^��ϊ�
						memcpy_s(Recv_Data[Player4], sizeof(RecvData), StrBuf, sizeof(RecvData));

						p_data[Player4]->flag[2] = Recv_Data[Player4]->Ready_flag;
						if (p_data[Player4]->flag[2] == true)
						{
							Send_Main_Data->data[Player4].flag[2] = true;
						}
						//�N���b�N����

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
						//����̐ڑ�
						//IP�Ɩ��O��o�^
						p_data[Player4]->ip = ip;
						p_data[Player4]->ID = INITIALIZE;
						memcpy_s(p_data[Player4]->name, sizeof(p_data[Player4]->name), StrBuf, sizeof(p_data[Player4]->name));
						//����̐ڑ��p���M�f�[�^�̍X�V
						strcpy_s(Send_Set_Data->data[Player4].name, sizeof(p_data[Player4]->name), p_data[Player4]->name);

						Send_Set_Data->data[Player4].ip = p_data[Player4]->ip;//IP
						Send_Set_Data->data[Player4].ID = p_data[Player4]->ID;
						Send_Set_Data->data[Player4].flag[0] = p_data[Player4]->flag[0];

						//���ڈȍ~�̑��M�f�[�^�̍X�V
						strcpy_s(Send_Main_Data->data[Player4].name, sizeof(p_data[Player4]->name), p_data[Player4]->name);

						Send_Main_Data->data[Player4].ip = p_data[Player4]->ip;//IP
						Send_Main_Data->data[Player4].ID = p_data[Player4]->ID;
						Send_Main_Data->data[Player4].flag[0] = p_data[Player4]->flag[0];

						//�f�[�^�𑗐M
						NetWorkSend(p4_NetHandle, Send_Set_Data, sizeof(Send_Set_Data));
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



		//�e�X�g�p
		/*int Mouse = GetMouseInput();

		if (Mouse & MOUSE_INPUT_RIGHT)
		{
			p_data[0]->flag[0] = true;
		}*/
		//�Q�[���J�n�p�̃f�[�^�̃Z�b�g
		if (GameStart_flag == false)
		{
			for (int i = INITIALIZE; i < MAX; i++) {
				//�����m�F
				if (NetHandle[i] != 0) {
					if (p_data[i]->flag[2] == false)
					{
						break;
					}
					player_check = true;
				}
				if (i == MAX - 1 && player_check == true)
				{
					//�v���C���[�̏��Ԃ����߂鏈��
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
		//�Q�[�����̏���
		if (GameSet_flag == false)
		{
			if (GameStart_flag == true)
			{
				//�^�[���ڍs����
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
				//�^�[���t���O�̍X�V
				for (int i = INITIALIZE; i < MAX; i++)
				{
					if (NetHandle[i] != -1) {
						Send_Main_Data->data[i].flag[0] = p_data[i]->flag[0];
					}
				}

				//�N���b�N�t���O��false�ɂ���
				if (Send_Main_Data->data[Turn_Player_num].flag[1] == true && Check_count != 2)
				{
					Send_Main_Data->data[Turn_Player_num].flag[1] = false;
					//�N���b�N�t���O���X�V������f�[�^�𑗂�
					for (int i = INITIALIZE; i < MAX; i++) {
						//�f�[�^�𑗐M����
						if (NetHandle[i] != -1) {
							NetWorkSend(NetHandle[i], Send_RankData, sizeof(SendRankData));
						}
					}

				}

			



				////���M�f�[�^�̍X�V
				//for (int i = INITIALIZE; i < MAX_TRUMP; i++)
				//{
				//	Send_Main_Data->trump[i] = *All_trump[i];
				//}
			}

			
			//�󋵕\��
			DrawFormatString(IP_POS_X, IP_POS_Y, GetColor(WHITE),
				"PC��IP�A�h���X:%d.%d.%d.%d �ڑ��|�[�g:%d",
				IP.d1,
				IP.d2,
				IP.d3,
				IP.d4,
				Port
			);
			for (int i = INITIALIZE; i < MAX; i++)
			{

				DrawFormatString(IP_DIS_POS_X, i * IP_DIS_POS_Y + 32, GetColor(WHITE),
					"�X���b�h1 IP:%d.%d.%d.%d name=%8s ����=%d",
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
					//�f�[�^�𑗐M����
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
						//�I������
						if (NetHandle[i] != 0) {
							NetWorkSend(NetHandle[i], Send_Main_Data, sizeof(Send_Main_Data));

						}
					}

				}

				WaitTimer(1000);
				for (int i = INITIALIZE; i < MAX; i++) {
					//�f�[�^�𑗐M����
					if (NetHandle[i] != -1) {
						NetWorkSend(NetHandle[i], Send_Main_Data, sizeof(Send_Main_Data));
					}
				}

				////���M�f�[�^�̍X�V
				//for (int i = INITIALIZE; i < MAX_TRUMP; i++)
				//{
				//	Send_Main_Data->trump[i] = *All_trump[i];
				//}
						//�ؒf�����v���C���[��������
				for (int i = 0; i < MAX; i++)
				{
					if (LostHandle == NetHandle[i]) {
						NetHandle[i] = -1;
						//�f�[�^�̏�����
						p_data[i]->Data_Init();
					}
				}
			}
		}
		if (GameSet_flag == true)
		{
			//�f�[�^���X�V����
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
				//�f�[�^�𑗐M����
				if (NetHandle[i] != -1) {
					NetWorkSend(NetHandle[i], Send_RankData, sizeof(SendRankData));
				}
			}
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