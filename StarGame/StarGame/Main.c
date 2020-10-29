#pragma once

#include "myGame.h"

int main(int argc, char* argv[]) {
	SOCKET client;	//����
	PLAYERINFO* player = (PLAYERINFO*)malloc(sizeof(PLAYERINFO));	//�÷��̾�
	if (player == NULL) return -1;

	if (Init() != 0)	return -1;
	if (Sound_init() != 0) return -1;

	//Ŭ���̾�Ʈ ����( ���� ���� )
	if (Client(&client, 7788) != 0) {
		printf("������ ���� �ֽ��ϴ�.\n");
		return 0;
	}
	
	int select = 0;
	int result = 0;

	// �α��� �޴�â
	do {
		select = LoginMenu();
		switch (select) {
		case SIGNIN:
			result = Signin(&client, player);
			break;
		case LOGIN:
			result = Login(&client, player);
			break;
		}
		if (result == 0) break;
	} while (select != GAMEEXIT);
	
	if (select == GAMEEXIT) {
		send(client, "exit", sizeof("exit"), 0);
		printf("�����մϴ�.\n");
		//�ڿ� ����
		Release_Sound();	//SDL_mixer ����
		free(player);
		closesocket(client);
		return 0;
	}

	select = 0;

	// ���� �޴�â
	do {
		select = Menu();
		switch (select) {
		case GAMESTART:
			GameRun(player);
			SendScore(&client, player);
			break;
		case GAMEEXPLAIN:
			PrintingExplanation();
			break;
		case RANKING:
			Ranking(&client, player);
			break;
		}
	} while (select != GAMEEXIT);

	if (select == GAMEEXIT) {
		send(client, "exit", sizeof("exit"), 0);
		printf("�����մϴ�.\n");
		//�ڿ� ����
		Release_Sound();	//SDL_mixer ����
		free(player);
		closesocket(client);
		return 0;
	}

	return 0;
}