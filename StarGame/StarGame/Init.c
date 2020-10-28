#include "MyGame.h"

void Init();	//시스템(콘솔창 크기...제목..등등) 초기화 _최초 한번 실행

void Init() {

	SetConsoleTitle(TEXT("StarGame"));	//콘솔 창 제목
	SetConsoleSize(CONSOLESIZE_COL, CONSOLESIZE_LINES);	//콘솔 창 사이즈 변경
	srand((unsigned)time(NULL)); //난수를 위한 시드 생성

}

void GameInit(PLAYERINFO* player) {
	SetMyCursor(FALSE);	//커서 오프
	player->x = (int)GAMEBOARD_COL / 2;
	player->y = GAMEBOARD_LINES - 1;
	player->hp = MAX_HP;
	//player.hp = MAX_HP;			//플레이어 정보 초기화
	player->boost = MAX_BOOST;
	player->score = 0;
	player->strong = 0;

	gameOver = 0;

	starCount = 0;	//생성되어져 있는 별 갯수.
	gameSpeed = 160; //게임 스피드

	memset(GameBoard, 0, sizeof(GameBoard));	//게임판 좌표 0으로 초기화
	GameBoard[player->x][player->y] = PLAYER; //플레이어 위치 초기화
}