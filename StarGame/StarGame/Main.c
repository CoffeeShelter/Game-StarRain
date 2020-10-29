#pragma once

#include "myGame.h"

int main(int argc, char* argv[]) {
	SOCKET client;	//소켓
	PLAYERINFO* player = (PLAYERINFO*)malloc(sizeof(PLAYERINFO));	//플레이어
	if (player == NULL) return -1;

	if (Init() != 0)	return -1;
	if (Sound_init() != 0) return -1;

	//클라이언트 실행( 서버 접속 )
	if (Client(&client, 7788) != 0) {
		printf("서버가 닫혀 있습니다.\n");
		return 0;
	}
	
	int select = 0;
	int result = 0;

	// 로그인 메뉴창
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
		printf("종료합니다.\n");
		//자원 해제
		Release_Sound();	//SDL_mixer 해제
		free(player);
		closesocket(client);
		return 0;
	}

	select = 0;

	// 게임 메뉴창
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
		printf("종료합니다.\n");
		//자원 해제
		Release_Sound();	//SDL_mixer 해제
		free(player);
		closesocket(client);
		return 0;
	}

	return 0;
}