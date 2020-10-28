#include "myGame.h"

void DrawGameDIsplay_(PLAYERINFO* player);	//게임판 출력
void DrawState_(PLAYERINFO* player); //상태창 출력

//게임 창
void DrawGameDIsplay_(PLAYERINFO* player) {

	int col, line;

	//게임 플레이 화면
	for (col = 0; col < GAMEBOARD_COL; col++) {

		for (line = 0; line < GAMEBOARD_LINES; line++) {

			GotoXY(col, line);

			switch (GameBoard[col][line]) {
				//공백 (배경)
			case BLANK:
				SetColor(0, 0);
				printf(" ");
				SetColor(0, 15);
				break;
				//플레이어
			case PLAYER:
				//무적상태 민트 색으로 표시
				if (player->strong) {
					SetColor(11, 0);
					printf(" ");
					SetColor(0, 15);
				}
				//보통상태 파랑
				else {
					SetColor(9, 0);
					printf(" ");
					SetColor(0, 15);
				}
				break;
				//기본 별똥별
			case STAR:
				SetColor(14, 0);
				printf(" ");
				SetColor(0, 15);
				break;
				//치유 별똥별 : 빨
			case HEALINGSTAR:
				SetColor(0, 12);
				printf("H");
				SetColor(0, 15);
				break;
				//빨리빨리 별똥별 : 파란색
			case FASTSTAR:
				SetColor(0, 9);
				printf("F");
				SetColor(0, 15);
				break;
				//튼튼 별똥별 : 회색
			case STRONGSTAR:
				SetColor(0, 8);
				printf("S");
				SetColor(0, 15);
				break;
				//무시무시 별똥...별? (즉사 별똥별) : 빨강색
			case DEATHSTAR:
				SetColor(12, 0);
				printf(" ");
				SetColor(0, 15);
				break;
			}
		}

	}
}

//게임 상태창
void DrawState_(PLAYERINFO* player) {

	int col, line;

	//유저 정보 창 배경
	for (col = 0; col < GAMEBOARD_STATE_COL; col++) {

		for (line = GAMEBOARD_LINES; line < GAMEBOARD_LINES + GAMEBOARD_STATE_LINES; line++) {

			GotoXY(col, line);
			SetColor(8, 15);
			printf(" ");

		}

	}

	//유저 정보

//HP
	GotoXY(5, GAMEBOARD_LINES + 1);
	SetColor(8, 10);
	printf("HP");

	GotoXY(9, GAMEBOARD_LINES + 1);
	SetColor(12, 0);
	for (int i = 0; i < player->hp; i++) {
		printf(" ");
	}


	//BOOST
	GotoXY(3, GAMEBOARD_LINES + 3);
	SetColor(8, 10);
	printf("BOOST");

	GotoXY(9, GAMEBOARD_LINES + 3);
	SetColor(9, 0);
	for (int i = 0; i < player->boost; i++) {
		printf(" ");
	}


	//SCORE
	GotoXY(25, GAMEBOARD_LINES + 2);
	SetColor(8, 10);
	printf("SCORE");
	GotoXY(31, GAMEBOARD_LINES + 2);
	printf("%d", player->score);

	SetColor(0, 15);
	GotoXY(0, 0);

}

//인트로 화면 출력
void DrawIntro() {
	FILE* introText = NULL;
	char c = 0;

	introText = fopen("intro.txt", "r");

	if (introText == NULL) return;

	while (1) {
		c = fgetc(introText);
		if (c == EOF)
			break;
		else {
			switch (c) {
			case '0':
				printf(" ");
				break;
			case '1':
				SetColor(6, 15);
				printf(" ");
				SetColor(0, 15);
				break;

			case '2':
				SetColor(2, 15);
				printf(" ");
				SetColor(0, 15);
				break;
			}
		}
	}

	fclose(introText);
}

//설명 출력
void PrintingExplanation() {
	FILE* file;
	char buf[1024];

	Clear();

	fopen_s(&file, "Explanation.txt", "r");
	if (file != NULL) {

		while (fgets(buf, sizeof(buf), file) != NULL)
			printf("%s", buf);

		fclose(file);
	}
	else {
		printf("설명 파일을 읽어올 수 없습니다.\n");
	}



	//기본 별똥별
	GotoXY(3, 12);
	SetColor(14, 0);
	printf(" ");

	//즉사 별똥별
	GotoXY(3, 14);
	SetColor(12, 0);
	printf(" ");

	//치유 별똥별
	GotoXY(3, 16);
	SetColor(0, 12);
	printf("H");

	//빨리빨리 별똥별
	GotoXY(3, 18);
	SetColor(0, 9);
	printf("F");

	//튼튼 별똥별
	GotoXY(3, 20);
	SetColor(0, 8);
	printf("S");

	//기본 상태 플레이어
	GotoXY(3, 24);
	SetColor(9, 0);
	printf(" ");

	//무적 상태 플레이어
	GotoXY(3, 26);
	SetColor(11, 0);
	printf(" ");



	SetColor(0, 15);

	while (GetKey() != ESC);
}

//게임 화면 전체 출력
void DrawConsole(PLAYERINFO* player) {
	clock_t curTime, oldTime;

	while (gameOver == 0) {
		oldTime = clock();

		DrawGameDIsplay_(player);
		DrawState_(player);

		curTime = clock();
		while (curTime - oldTime < 33) {
			curTime = clock();
		}
	}
}

// 22,24 . y

//로그인 창 출력
void DrawLoginWindow() {
	Clear();	//화면 지움
	DrawIntro();	//StarGame 이미지 출력
	GotoXY(7, 22);
	printf(" 로그인 :");
	GotoXY(7, 24);
	printf("비밀번호 :");
}

//회원가입 창 출력
void DrawSigninWindow() {
	Clear();	//화면 지움
	DrawIntro();	//StarGame 이미지 출력
	GotoXY(7, 22);
	printf(" 로그인 :");
	GotoXY(7, 24);
	printf("비밀번호 :");
	GotoXY(7, 26);
	printf(" 닉네임 :");

}