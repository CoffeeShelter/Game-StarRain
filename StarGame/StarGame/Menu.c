#include "MyGame.h"



/*
로그인
회원가입
*/

int LoginMenu() {
	char key = 0;
	int choice = LOGIN;

	Clear();
	DrawIntro();	//인트로 화면 출력

	while (!((key == ENTER) && (choice == GAMEEXIT))) {

		GotoXY(15, LOGIN);
		printf("로 그 인");

		GotoXY(15, SIGNIN);
		printf("회원가입");

		GotoXY(15, GAMEEXIT);
		SetColor(0, 4);
		printf("게임 종료");
		SetColor(0, 15);

		GotoXY(12, choice);
		printf("▶");

		key = GetKey();

		switch (key) {
		case UP:
			if (choice != LOGIN) {
				GotoXY(12, choice);
				printf("   ");
				choice -= 2;
				GotoXY(12, choice);
				printf("▶");
			}
			break;

		case DOWN:
			if (choice != GAMEEXIT) {
				GotoXY(12, choice);
				printf("   ");
				choice += 2;
				GotoXY(12, choice);
				printf("▶");
			}
			break;

		case ENTER:
			Clear();
			return choice;

			break;
		}

	}
	return 0;
}

/*
게임시작
게임설명
랭킹확인
게임종료
*/

int Menu() {
	char key = 0;
	int choice = GAMESTART;

	Clear();
	DrawIntro();	//인트로 화면 출력

	while (!((key == ENTER) && (choice == GAMEEXIT))) {


		GotoXY(15, GAMESTART);
		printf("게임 시작");

		GotoXY(15, GAMEEXPLAIN);
		printf("게임 설명");

		GotoXY(15, RANKING);
		printf("랭킹 확인");

		GotoXY(15, GAMEEXIT);
		SetColor(0, 4);
		printf("게임 종료");
		SetColor(0, 15);

		GotoXY(12, choice);
		printf("▶");

		key = GetKey();

		switch (key) {
			
		case UP:
			if (choice != GAMESTART) {
				GotoXY(12, choice);
				printf("   ");
				choice -= 2;
				GotoXY(12, choice);
				printf("▶");
			}
			break;

		case DOWN:
			if (choice != GAMEEXIT) {
				GotoXY(12, choice);
				printf("   ");
				choice += 2;
				GotoXY(12, choice);
				printf("▶");
			}
			break;

		case ENTER:
			Clear();
			return choice;

			break;
		}

	}

	return 0;
}