#include "myGame.h"

void main(void) {
	SOCKET client;
	PLAYERINFO* player = (PLAYERINFO*)malloc(sizeof(PLAYERINFO));
	if (player == NULL) return;

	Init();

	//Ŭ���̾�Ʈ ����( ���� ���� )
	if (Client(&client, 7788) != 0) {
		printf("������ ���� �ֽ��ϴ�.\n");
		return;
	}

	int select = 0;
	int result = 0;
	do {
		select = LoginMenu();
		switch (select) {
		case SIGNIN:
			result = Signin(&client, player);
			break;
		case LOGIN:
			result = Login(&client,player);
			break;
		}
		if (result == 0) break;
	} while (select != GAMEEXIT);

	if (select == GAMEEXIT) {
		send(client, "exit", sizeof("exit"), 0);
		printf("�����մϴ�.\n");
		return;
	}

	select = 0;
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
		free(player);
		return;
	}
}