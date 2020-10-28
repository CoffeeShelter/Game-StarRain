#include "MyGame.h"

//�÷��̾��� ������ ������� �Լ�
void* PlayerMove(void* data) {
	PLAYERINFO* player = (PLAYERINFO*)data;
	char key;

	while (gameOver == 0) {

		key = GetKey();

		//������ �̵�
		if (key == RIGHT) {
			if (player->x < GAMEBOARD_COL - 1) {
				GameBoard[player->x][player->y] = BLANK;
				player->x += 1;
				// ������ üũ
				if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
					GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
					|| GameBoard[player->x][player->y] == DEATHSTAR)
					Hit(GameBoard[player->x][player->y],player);
				GameBoard[player->x][player->y] = PLAYER;
			}
		}
		//���� �̵�
		else if (key == LEFT) {
			if (player->x > 0) {
				GameBoard[player->x][player->y] = BLANK;
				player->x -= 1;
				// ������ üũ
				if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
					GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
					|| GameBoard[player->x][player->y] == DEATHSTAR)
					Hit(GameBoard[player->x][player->y],player);
				GameBoard[player->x][player->y] = PLAYER;
			}

		}
		//���� ���� �ν�Ʈ(3ĭ �̵�)
		else if (key == 'a') {
			if (player->boost > 0) {
				player->boost -= 1;
				if (player->x > 0) {
					GameBoard[player->x][player->y] = BLANK;
					player->x -= 3;
					// ������ üũ
					if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
						GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
						|| GameBoard[player->x][player->y] == DEATHSTAR)
						Hit(GameBoard[player->x][player->y],player);
					GameBoard[player->x][player->y] = PLAYER;
				}
			}
		}
		//������ ���� �ν�Ʈ(3ĭ �̵�)
		else if (key == 'd') {
			if (player->boost > 0) {
				player->boost -= 1;
				if (player->x > 0) {
					GameBoard[player->x][player->y] = BLANK;
					player->x += 3;
					// ������ üũ
					if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
						GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
						|| GameBoard[player->x][player->y] == DEATHSTAR)
						Hit(GameBoard[player->x][player->y],player);
					GameBoard[player->x][player->y] = PLAYER;
				}
			}
		}

		Sleep(80);

	}

	return NULL;
}