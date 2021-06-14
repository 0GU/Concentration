#pragma once
#include "DxLib.h"

#include <string.h>
#include <string>
#include <sstream>

#define INITIALIZE 0//������

#define MAX 4//�ڑ���
#define FLAG_MAX 3//�t���O�̐�
#define WIDTH 800//windows�T�C�Y�@��
#define HEIGHT 600//windowa�T�C�Y�@�c
#define OFFSET_X 50//�摜�\���ʒu�␳�@��
#define OFFSET_Y 100//�摜�\���ʒu�␳�@�c
#define TRUMP_WIDTH 120//�g�����v�̉摜�T�C�Y�@��
#define TRUMP_HEIGHT 170//�g�����v�̉摜�T�C�Y�@�c
#define HORIZONTAL_SPACING 20+TRUMP_WIDTH//�g�����v�̉��̊Ԋu
#define VERTICAL_SPACING 30+TRUMP_HEIGHT//�g�����v�̏c�̊Ԋu 

#define SUIT 4//�g�����v�̃X�[�g�̐�
#define TRUMP_NUMBER 13//�g�����v�̐����̐�


	
//IPDATA IP_set();//IP����

//�X���b�h�i���o�[
enum{
	T_A,
	T_B,
	T_C,
	T_D,
};

//�J�[�h�p�\����
struct Card {
	float x = 0, y = 0;
	int suit = 0;
	int num = 0;
};

//�ʒu���
struct Point{
	int x, y;
};

//���ʊm�F
struct Rank{
	char name[MAX][8] = { { "null" },{ "null" },{ "null" },{ "null" } };//���O
	int count[MAX] = {0,0,0,0};
	int ranking[MAX] = {0,0,0,0};
};

//�S���̖��O
struct ALL_Name{
	char p_name[MAX][8] = { { "null" },{ "null" },{ "null" },{ "null" } };//���O
};

//�v���C���[�N���X
class Data{
private:
public:
	char name[8] = { "null" };//���O
	int count=0;
	int ID{ -1 };//�I�u�W�F�N�g���ʗp
	IPDATA ip{ 0,0,0,0 };//IP�A�h���X�ۑ��p
	bool flag[FLAG_MAX] = { false,false,false };//0�E�E�E�^�[�����ʗp�@1�E�E�E�N���b�N�p�@2�E�E�E�����m�F�p
	
	//�R���X�g���N�^
	Data();
	Data( char* _name,bool* _flag);
	//�������p���\�b�h
	void Data_Init();
};

//�J�[�h�N���X
class Trump {
private:
public:
	Card line_card;
	int ID{ -1 }; //�I�u�W�F�N�g���ʗp
	bool FandB_flag = false;

	//�R���X�g���N�^
	Trump();
	Trump(Card _line_card, bool _FandB);
	//���������\�b�h
	void Card_Init();
};

//�����L���O�N���X
class Rank_Data {
private:
public:
	Rank allrank;
	//�R���X�g���N�^
	Rank_Data();
	Rank_Data(ALL_Name _name, int* _count,int* _ranking);
	//�������p���\�b�h
	void Rank_Data_Init();
};

//����M�f�[�^�p�̃N���X
class SendData {
private:
public:
	Data data[MAX];
};

//����M�g�����v�p�̃N���X
class SendTrump {
private:
public:
	Trump trump[52];
};

//����M�����L���O�f�[�^�p�̃N���X
class SendRankData {
private:
public:
	Rank_Data Rdata;
};