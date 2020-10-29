#pragma once

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "ws2_32.lib")

// < 사용 헤더 >

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


// < 사용 매크로 >

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define CONSOLESIZE_COL 40	//콘솔 가로 크기
#define CONSOLESIZE_LINES 30 //콘솔 세로 크기
#define GAMEBOARD_COL CONSOLESIZE_COL //실제 게임을 플레이하는 공간 가로 크기
#define GAMEBOARD_LINES (CONSOLESIZE_LINES - 5) //실제 게임을 플레이하는 공간 세로 크기
#define GAMEBOARD_STATE_COL CONSOLESIZE_COL //게임 플레이 화면 밑 유저 정보 확인 칸(hp,점수 등) 가로 크기
#define GAMEBOARD_STATE_LINES (CONSOLESIZE_LINES - GAMEBOARD_LINES) //게임 플레이 화면 밑 유저 정보 확인 칸(hp,점수 등) 세로 크기

//키 코드
#define ARROW_KEY -32
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ENTER 13
#define BACKSPACE 8
#define ESC 27

//메뉴 코드
#define GAMESTART 20
#define GAMEEXPLAIN 22
#define RANKING 24
#define SENDINGSCORE 100
#define GAMEEXIT 26

#define LOGIN 22
#define SIGNIN 24

//게임 정보
#define MAX_HP 10 //최대 hp
#define MAX_BOOST 10 //최대 부스트 횟수

// < 사용 변수 >

//소리
typedef struct _GAME_SOUND {
	Mix_Music* BGM;
	Mix_Chunk* GET_ITEM_SOUND;
	Mix_Chunk* HIT_SOUND;
}GAME_SOUND;

GAME_SOUND sound;

//게임판 값
//공백, 플레이어, 별똥별(기본,치유,부스트,무적,즉사)
enum {
	BLANK, PLAYER, STAR, HEALINGSTAR, FASTSTAR, STRONGSTAR, DEATHSTAR
};

int gameOver;	//게임플레이 상태 값	1 = 게임 오버
int starCount;	//생성된 별별똥 개수
int gameSpeed;	//별똥별 떨어지는 속도
int GameBoard[GAMEBOARD_COL][GAMEBOARD_LINES];	//게임판 좌표 배열

//송신 쓰레드 매개변수
struct recvThreadData {
	SOCKET* socket;
};

//플레이어 정보
typedef struct PLAYER_STATE {
	int x;	//x좌표
	int y; //y좌표

	int hp; //목숨
	int boost; //부스트 횟수
	int strong; //무적 상태 0 일반상태, 1 무적상태 (별똥별에 맞아도 피해를 받지 않음)

	int score; //점수
	int maxScore; //최고 점수
	char name[64]; //닉네임
}PLAYERINFO;

// < 사용 함수 >

//Client.c
int Client(SOCKET* client, u_short port);	// 클라이언트 실행 (서버 접속)

//SimpleWindow.c
void SetConsoleSize(int col, int lines);	//콘솔창 크기 설정
void GotoXY(int x, int y); //커서 위치 설정
void SetMyCursor(BOOL _bShow); //커서 온/오프
void SetColor(unsigned char _BgColor, unsigned char _TextColor); //배경, 글자색 변경
void Clear(void); //화면 지우기

//Init.c
int Init();	//초기화
int Sound_init();	//SDL_mixer 초기화
void GameInit(PLAYERINFO* player);	//게임 초기화

//GameManager.c
int GetKey();	//키 입력
int Login(SOCKET* client, PLAYERINFO* player);	//로그인
int Signin(SOCKET* client,PLAYERINFO* player);	//회원가입
int Ranking(SOCKET* client, PLAYERINFO* player);//랭킹 확인
int SendScore(SOCKET* client, PLAYERINFO* player);	//점수 전송
void GameRun(PLAYERINFO* player); //게임 시작

void Hit(int whatStar, PLAYERINFO* player); //부딪힐 때 
void GameOver(PLAYERINFO* player); //게임 오버

//DrawWindow.c
void DrawIntro();	//인트로 출력
void PrintingExplanation();	//설명 출력
void DrawConsole(PLAYERINFO* player); //게임 화면 출력
void DrawLoginWindow();	//로그인 화면 출력
void DrawSigninWindow();	//회원가입 화면 출력

//Menu.c
int LoginMenu();	//로그인 메뉴	( 로그인, 회원가입 )
int Menu();	//게임 메뉴	( 게임시작, 게임설명, 랭킹확인, 게임종료 )

//Star.c
void* StarLogic(void* data);	//별똥별

//Player.c
void* PlayerMove(void* data); //플레이어 움직임

//Release.c
int Release_Sound();	//SDL_mixer 자원 해제