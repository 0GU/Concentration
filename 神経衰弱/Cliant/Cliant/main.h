#pragma once
#include "DxLib.h"

#include <string.h>
#include <string>
#include <sstream>
#include <Windows.h>

#define INITIALIZE 0                      //������
#define MAX 4                             //�ڑ���
#define FLAG_MAX 3                        //�t���O�̐�
#define WIDTH 1900                        //windows�T�C�Y�@��
#define HEIGHT 1000                       //windowa�T�C�Y�@�c
#define WHITE 255,255,255                 //�����̐F�F��
#define RED 255,0,0                       //�����̐F�G��
#define GREEN 0,255,0                     //�����̐F�F��
#define BLUE 0,0,255                      //�����̐F�F��
#define CONNECTION_POS_X 0                //�ڑ��m������X��
#define CONNECTION_POS_Y 0                //�ڑ��m������Y��
#define CONNECTION_CMP_POS_X 0            //�ڑ���������X��
#define CONNECTION_CMP_POS_Y 16           //�ڑ���������Y��
#define TRUMP_WIDTH 120                   //�J�[�h�̏c�̒���
#define TRUMP_HEIGHT 170                  //�J�[�h�̉��̒���
#define HORIZONTAL_SPACING (20 + TRUMP_WIDTH) //�g�����v�̉��̊Ԋu
#define VERTICAL_SPACING (30 + TRUMP_HEIGHT) //�g�����v�̏c�̊Ԋu
#define OFFSET_X 50                       //��ʕ\���ʒu�␳X��
#define OFFSET_Y 100                      //��ʕ\���ʒu�␳Y��
#define NAME_INPUT_POS_X 0                //���O�\���ʒuX��
#define NAME_INPUT_POS_Y 0                //���O�\���ʒuY��
#define NAME_INPUT_ANSWER_POS_X 0         //���O�𓚕����\���ʒuX��
#define NAME_INPUT_ANSWER_POS_Y 16        //���O�𓚕����\���ʒuY��
#define NAME_INPUT_MAX 8                  //���O���͍ő啶����
#define IP_INPUT_POS_X 0                  //IP���͕����\���ʒuX��
#define IP_INPUT_POS_Y 0                  //IP���͕����\���ʒuY��
#define IP_DIS_POS_X 0                    //IP�\���ʒuX��
#define IP_DIS_POS_Y 16                   //IP�\���ʒuY��
#define IP_CONFIRMATION_POS_X 0           //IP�m�F�����\���ʒuX��
#define IP_CONFIRMATION_POS_Y 32          //IP�m�F�����\���ʒuY��
#define IP_ANSWER_POS_X 0                 //IP�𓚕����\���ʒuX��
#define IP_ANSWER_POS_Y 48                //IP�𓚕����\���ʒuY��
#define IP_KEY_INPUT_POS_X 0              //IP���͕����\���ʒuX��
#define IP_KEY_INPUT_POS_Y 16             //IP���͕����\���ʒuY��
#define IP_KEY_INPUT_MAX_NUM 255          //IP���͕����ő啶����
#define IP_KEY_INPUT_MIN_NUM 0            //IP���͕����ŏ�������
#define IP_KEY_INPUT_ANSWER_POS_X 16      //IP���͕����m�F�����\���ʒuX��
#define IP_KEY_INPUT_ANSWER_POS_Y 64      //IP���͕����m�F�����\���ʒuY��
#define RANK_DIS_POS_X 1000 //�����L���O�\��X���W
#define RANK_DIS_POS_Y 100 //�����L���O�\��Y���W

#define SUIT 4                            //�g�����v�̃X�[�g�̐�
#define TRUMP_NUMBER 13                   //�g�����v�̐����̐�
#define TRUMP_MAX (SUIT*TRUMP_NUMBER)     //�g�����v�̍��v����

IPDATA IP_set(); //IP����

//�X���b�h�i���o�[
enum {
	T_A,
	T_B,
	T_C,
	T_D,
};

//�ʒu���
struct Point {
	int x, y;
};

//�J�[�h�p�\����
struct Card {
	float x = INITIALIZE, y = INITIALIZE;
	int suit = INITIALIZE;
	int num = INITIALIZE;
};

//���ʊm�F
struct Rank {
	char name[MAX][NAME_INPUT_MAX] = { { "null" },{ "null" },{ "null" },{ "null" } };//���O
	int count[MAX] = { 0,0,0,0 };
	int ranking[MAX] = { 0,0,0,0 };
};

//�S���̖��O
struct ALL_Name {
	char p_name[MAX][NAME_INPUT_MAX] = { { "null" },{ "null" },{ "null" },{ "null" } };//���O
};

//�v���C���[�N���X
class Data {
private:
public:
	char name[NAME_INPUT_MAX] = { "null" };//���O
	int count = INITIALIZE;
	int ID{ -1 };//�I�u�W�F�N�g���ʗp
	IPDATA ip{ 0,0,0,0 };//IP�A�h���X�ۑ��p
	bool flag[FLAG_MAX] = { false,false,false };//0�E�E�E�^�[�����ʗp�@1�E�E�E�N���b�N�p�@2�E�E�E�����m�F�p

	//�R���X�g���N�^
	Data();
	Data(char* _name, bool* _flag);
	//�������p���\�b�h
	void Data_Init();
};

//�J�[�h�N���X
class Trump {
private:
public:
	Card line_card ;
	int ID{ 10 }; //�I�u�W�F�N�g���ʗp
	bool FandB_flag = false; //���\�t���O

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
	Rank_Data(ALL_Name _name, int* _count, int* _ranking);
	//�������p���\�b�h
	void Rank_Data_Init();
};

//����M�f�[�^�p�̃N���X
class RecvData {
private:
public:
	Data data[MAX];
	Trump trump[TRUMP_MAX];
	bool End_flag;
};

class SendData {
private:
public:
	Point pos;
	bool Ready_flag;
	bool turn_flag;
};

class SendMyData {
private:
public:
	Data data;
};

////����M�g�����v�p�̃N���X
//class SendTrump {
//private:
//public:
//	Trump trump[52];
//};


//����M�����L���O�f�[�^�p�̃N���X
class RecvRankData {
private:
public:
	Rank_Data Rdata;
};