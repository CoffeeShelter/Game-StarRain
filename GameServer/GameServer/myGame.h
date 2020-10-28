#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib,"ws2_32")

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <pthread.h>

//유저 정보
typedef struct userInfo {
	char id[16];
	char password[16];
	char name[32];
	int score;
}Info;

//유저 리스트
typedef struct LinkedList {
	Info data;
	struct LinkedList* next;
}Users;

//Accept쓰레드 매개 변수
struct acceptData {
	Users* head;
	SOCKET* server;
};

//Client쓰레드 매개 변수
struct clientData {
	Users* head;
	SOCKET* client;
};

//Server.c
int Server(u_short port); //서버 생성

//Accept.c
void* Accept(void* acceptData); //Accept쓰레드

//Client.c
void* Client(void* data); //클라이언트 쓰레드

//UserData.c
int AddUser(Users* head, Info userInfo);	//유저 추가
void SortUser(Users* head, Users* target);	//유저 정렬
void DelUser(Users* head, char* nickName);	//유저 삭제
Users* FindUser(Users* head, char* nickName);	//유저 찾기(이름)
Users* FindUser_ID(Users* head, char* id);	//유저 찾기(아이디)
int FindUser_RANKING(Users* head, char* nickName);	//유저 랭킹 확인
void PrintUser(Users* head);	//유저 출력
void PrintUser_FILE(FILE* file, Users* head);	//유저 파일에 저장
void FreeList(Users* head);	//유저 리스트 해제