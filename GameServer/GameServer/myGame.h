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

//���� ����
typedef struct userInfo {
	char id[16];
	char password[16];
	char name[32];
	int score;
}Info;

//���� ����Ʈ
typedef struct LinkedList {
	Info data;
	struct LinkedList* next;
}Users;

//Accept������ �Ű� ����
struct acceptData {
	Users* head;
	SOCKET* server;
};

//Client������ �Ű� ����
struct clientData {
	Users* head;
	SOCKET* client;
};

//Server.c
int Server(u_short port); //���� ����

//Accept.c
void* Accept(void* acceptData); //Accept������

//Client.c
void* Client(void* data); //Ŭ���̾�Ʈ ������

//UserData.c
int AddUser(Users* head, Info userInfo);	//���� �߰�
void SortUser(Users* head, Users* target);	//���� ����
void DelUser(Users* head, char* nickName);	//���� ����
Users* FindUser(Users* head, char* nickName);	//���� ã��(�̸�)
Users* FindUser_ID(Users* head, char* id);	//���� ã��(���̵�)
int FindUser_RANKING(Users* head, char* nickName);	//���� ��ŷ Ȯ��
void PrintUser(Users* head);	//���� ���
void PrintUser_FILE(FILE* file, Users* head);	//���� ���Ͽ� ����
void FreeList(Users* head);	//���� ����Ʈ ����