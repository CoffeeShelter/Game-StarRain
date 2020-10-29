#pragma once

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "ws2_32.lib")

// < ��� ��� >

#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include <pthread.h>
#include <mmsystem.h>
#include <SDL.h>
#include <SDL_mixer.h>

#pragma warning(disable:4996)


// < ��� ��ũ�� >

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define CONSOLESIZE_COL 40	//�ܼ� ���� ũ��
#define CONSOLESIZE_LINES 30 //�ܼ� ���� ũ��
#define GAMEBOARD_COL CONSOLESIZE_COL //���� ������ �÷����ϴ� ���� ���� ũ��
#define GAMEBOARD_LINES (CONSOLESIZE_LINES - 5) //���� ������ �÷����ϴ� ���� ���� ũ��
#define GAMEBOARD_STATE_COL CONSOLESIZE_COL //���� �÷��� ȭ�� �� ���� ���� Ȯ�� ĭ(hp,���� ��) ���� ũ��
#define GAMEBOARD_STATE_LINES (CONSOLESIZE_LINES - GAMEBOARD_LINES) //���� �÷��� ȭ�� �� ���� ���� Ȯ�� ĭ(hp,���� ��) ���� ũ��

//Ű �ڵ�
#define ARROW_KEY -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define BACKSPACE 8
#define ESC 27

//�޴� �ڵ�
#define GAMESTART 20
#define GAMEEXPLAIN 22
#define RANKING 24
#define SENDINGSCORE 100
#define GAMEEXIT 26

#define LOGIN 22
#define SIGNIN 24

//���� ����
#define MAX_HP 10 //�ִ� hp
#define MAX_BOOST 10 //�ִ� �ν�Ʈ Ƚ��

// < ��� ���� >

//�Ҹ�
typedef struct _GAME_SOUND {
	Mix_Music* BGM;
	Mix_Chunk* GET_ITEM_SOUND;
	Mix_Chunk* HIT_SOUND;
}GAME_SOUND;

GAME_SOUND sound;

//������ ��
//����, �÷��̾�, ���˺�(�⺻,ġ��,�ν�Ʈ,����,���)
enum {
	BLANK, PLAYER, STAR, HEALINGSTAR, FASTSTAR, STRONGSTAR, DEATHSTAR
};

int gameOver;	//�����÷��� ���� ��	1 = ���� ����
int starCount;	//������ ������ ����
int gameSpeed;	//���˺� �������� �ӵ�
int GameBoard[GAMEBOARD_COL][GAMEBOARD_LINES];	//������ ��ǥ �迭

//�۽� ������ �Ű�����
struct recvThreadData {
	SOCKET* socket;
};

//�÷��̾� ����
typedef struct PLAYER_STATE {
	int x;	//x��ǥ
	int y; //y��ǥ

	int hp; //���
	int boost; //�ν�Ʈ Ƚ��
	int strong; //���� ���� 0 �Ϲݻ���, 1 �������� (���˺��� �¾Ƶ� ���ظ� ���� ����)

	int score; //����
	int maxScore; //�ְ� ����
	char name[64]; //�г���
}PLAYERINFO;

// < ��� �Լ� >

//Client.c
int Client(SOCKET* client, u_short port);	// Ŭ���̾�Ʈ ���� (���� ����)

//SimpleWindow.c
void SetConsoleSize(int col, int lines);	//�ܼ�â ũ�� ����
void GotoXY(int x, int y); //Ŀ�� ��ġ ����
void SetMyCursor(BOOL _bShow); //Ŀ�� ��/����
void SetColor(unsigned char _BgColor, unsigned char _TextColor); //���, ���ڻ� ����
void Clear(void); //ȭ�� �����

//Init.c
int Init();	//�ʱ�ȭ
int Sound_init();	//SDL_mixer �ʱ�ȭ
void GameInit(PLAYERINFO* player);	//���� �ʱ�ȭ

//GameManager.c
int GetKey();	//Ű �Է�
int Login(SOCKET* client, PLAYERINFO* player);	//�α���
int Signin(SOCKET* client,PLAYERINFO* player);	//ȸ������
int Ranking(SOCKET* client, PLAYERINFO* player);//��ŷ Ȯ��
int SendScore(SOCKET* client, PLAYERINFO* player);	//���� ����
void GameRun(PLAYERINFO* player); //���� ����

void Hit(int whatStar, PLAYERINFO* player); //�ε��� �� 
void GameOver(PLAYERINFO* player); //���� ����

//DrawWindow.c
void DrawIntro();	//��Ʈ�� ���
void PrintingExplanation();	//���� ���
void DrawConsole(PLAYERINFO* player); //���� ȭ�� ���
void DrawLoginWindow();	//�α��� ȭ�� ���
void DrawSigninWindow();	//ȸ������ ȭ�� ���

//Menu.c
int LoginMenu();	//�α��� �޴�	( �α���, ȸ������ )
int Menu();	//���� �޴�	( ���ӽ���, ���Ӽ���, ��ŷȮ��, �������� )

//Star.c
void* StarLogic(void* data);	//���˺�

//Player.c
void* PlayerMove(void* data); //�÷��̾� ������

//Release.c
int Release_Sound();	//SDL_mixer �ڿ� ����