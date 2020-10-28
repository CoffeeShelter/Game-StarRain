#include "MyGame.h"

/*
별똥별 생성 함수

기본 별똥별: 플레이거가 이에 맞을시 HP -1 감소
즉사 별똥별: 플레이거가 이에 맞을시 HP에 관계 없이 게임 오버
*/
void SetStar(PLAYERINFO* player) {
	int startStar = 0;	//별똥별이 떨어질 시작 위치
	int maxStarCount = 0;	//최대 별똥별 개수

	int item = rand() % 150; //아이템 확률 설정 (0~N-1(설정된 수치) 의 숫자중 10 = 치유 별똥별 11 = 빨리빨리 별똥별 12 = 튼튼 별똥별 해당 세개의 수에만 반응);

	startStar = rand() % GAMEBOARD_COL;	//시작 위치 설정

	if (player->score >= 0 && player->score <= 500) {
		maxStarCount = 10;
	}
	else if (player->score > 500 && player->score <= 3000) {
		maxStarCount = 20;
		gameSpeed = 100; // 0.1초
	}
	else if (player->score > 3000 && player->score <= 7000) {
		maxStarCount = 40;
		gameSpeed = 60; // 0.06초
	}
	else if (player->score > 7000) {
		maxStarCount = 50;
		gameSpeed = 44; // 0.44초
	}

	if (starCount < maxStarCount) {
		//이미 별똥별이 있으면 다시 설정
		if (GameBoard[startStar][0] == STAR) {
			SetStar(player);
		}
		else {
			switch (item) {
			case HEALINGSTAR:
				GameBoard[startStar][0] = HEALINGSTAR;	//치유 별똘병 (HP 증가)
				break;
			case FASTSTAR:
				GameBoard[startStar][0] = FASTSTAR;	//빨리빨리 별똥별 (부스트 횟수 증가)
				break;
			case STRONGSTAR:
				GameBoard[startStar][0] = STRONGSTAR; //무적 상태 부여
				break;
			case DEATHSTAR:
				GameBoard[startStar][0] = DEATHSTAR;	//즉사(즉시 게임 오버)
				break;
			default:
				GameBoard[startStar][0] = STAR;	// 기본 별똥별 (닿을 시 HP -1)
				break;
			}
			starCount++;
		}
	}


}
/*
별똥별 낙하 기능
플레이어 와 닿으면 HP -1
*/
void DropStar(PLAYERINFO* player) {
	int col, line;
	int whatStar = 0;

	//게임판 밑쪽 부터 검사.
	for (col = GAMEBOARD_COL - 1; col >= 0; col--) {
		for (line = GAMEBOARD_LINES - 1; line >= 0; line--) {

			//게임판 별똥별 확인
			if (GameBoard[col][line] == STAR || GameBoard[col][line] == HEALINGSTAR ||
				GameBoard[col][line] == FASTSTAR || GameBoard[col][line] == STRONGSTAR
				|| GameBoard[col][line] == DEATHSTAR)
			{
				whatStar = GameBoard[col][line];
				GameBoard[col][line] = BLANK;

				if (line < GAMEBOARD_LINES - 1) {

					//플레이어 부딪힘 확인
					if (GameBoard[col][line + 1] == PLAYER) {
						Hit(whatStar,player);
						starCount--;
					}
					else {
						GameBoard[col][line + 1] = whatStar;
					}
				}
				else {
					player->score += 10;
					starCount--;
				}

			}
		}
	}
}

/*
별똥별 생성 ->(약간의 텀_게임속도)-> 낙하 /반복/
*/
void* StarLogic(void* data) {
	PLAYERINFO* player = (PLAYERINFO*)data;
	while (gameOver == 0) {
		SetStar(player);
		Sleep(gameSpeed);
		DropStar(player);
	}

	return NULL;
}