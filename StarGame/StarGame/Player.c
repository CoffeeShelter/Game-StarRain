#include "MyGame.h"

//플레이어의 움직임 쓰레드용 함수
void* PlayerMove(void* data) {
	PLAYERINFO* player = (PLAYERINFO*)data;
	char key;

	while (gameOver == 0) {

		key = GetKey();

		//오른쪽 이동
		if (key == RIGHT) {
			if (player->x < GAMEBOARD_COL - 1) {
				GameBoard[player->x][player->y] = BLANK;
				player->x += 1;
				// 게임판 체크
				if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
					GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
					|| GameBoard[player->x][player->y] == DEATHSTAR)
					Hit(GameBoard[player->x][player->y],player);
				GameBoard[player->x][player->y] = PLAYER;
			}
		}
		//왼쪽 이동
		else if (key == LEFT) {
			if (player->x > 0) {
				GameBoard[player->x][player->y] = BLANK;
				player->x -= 1;
				// 게임판 체크
				if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
					GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
					|| GameBoard[player->x][player->y] == DEATHSTAR)
					Hit(GameBoard[player->x][player->y],player);
				GameBoard[player->x][player->y] = PLAYER;
			}

		}
		//왼쪽 방향 부스트(3칸 이동)
		else if (key == 'a') {
			if (player->boost > 0) {
				player->boost -= 1;
				if (player->x > 0) {
					GameBoard[player->x][player->y] = BLANK;
					player->x -= 3;
					// 게임판 체크
					if (GameBoard[player->x][player->y] == STAR || GameBoard[player->x][player->y] == HEALINGSTAR ||
						GameBoard[player->x][player->y] == FASTSTAR || GameBoard[player->x][player->y] == STRONGSTAR
						|| GameBoard[player->x][player->y] == DEATHSTAR)
						Hit(GameBoard[player->x][player->y],player);
					GameBoard[player->x][player->y] = PLAYER;
				}
			}
		}
		//오른쪽 방향 부스트(3칸 이동)
		else if (key == 'd') {
			if (player->boost > 0) {
				player->boost -= 1;
				if (player->x > 0) {
					GameBoard[player->x][player->y] = BLANK;
					player->x += 3;
					// 게임판 체크
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