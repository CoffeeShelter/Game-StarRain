#include "myGame.h"

#define MAX 16
#define _C (DWORD)1046.502
#define _D (DWORD)1108.731
#define _E (DWORD)1318.510
#define _F (DWORD)1396.913
#define _G (DWORD)1567.982
#define _A (DWORD)1760.000
#define _B (DWORD)1975.533

int Input_password(char* password);	//비밀번호 입력 (* 로 표시)

void GameRun(PLAYERINFO* player) {
	pthread_t myThread[2];
	int threadResult;

	GameInit(player);

	//움직이는 함수 쓰레드
	threadResult = pthread_create(&myThread[0], NULL, StarLogic, (void*)player);
	if (threadResult < 0) {
		printf("쓰레드 오류\n");
		exit(1);
	}

	//별기능 함수 쓰레드
	threadResult = pthread_create(&myThread[1], NULL, PlayerMove, (void*)player);
	if (threadResult < 0) {
		printf("쓰레드 오류\n");
		exit(1);
	}

	DrawConsole(player);

	GameOver(player);
}

//키 입력
int GetKey() {
	char key;

	if (_kbhit()) {
		key = _getch();

		if (key == ARROW_KEY) {

			key = _getch();

			return key;

		}
		else {
			return key;
		}
	}

	return -1;

}

//회원 가입
//	signin/<아이디>/<비밀번호>/<닉네임>
int Signin(SOCKET* client, PLAYERINFO* player) {
	char message[1024] = { '\0' };
	char id[MAX];
	char password[MAX];
	char nickName[32];
	int result = 0;
	
	do {
		SetMyCursor(TRUE);	//커서 온

		DrawSigninWindow();	//회원가입 화면 출력

		GotoXY(18, 22);		gets_s(id, sizeof(id));			//아이디 입력
		GotoXY(18, 24);		Input_password(password);		//비밀번호 입력
		GotoXY(18, 26);		gets_s(nickName, sizeof(id));	//닉네임 입력
		
		SetMyCursor(FALSE);	//커서 오프

		//message => signin/id/password/nickName
		strcpy(message, "signin/");
		strcat(message, id);
		strcat(message, "/");
		strcat(message, password);
		strcat(message, "/");
		strcat(message, nickName);

		//전송
		result = send(*client, message, strlen(message), 0);
		if (result == SOCKET_ERROR) {
			printf("에러코드 %d\n", WSAGetLastError());
			return -1;
		}
		//받기
		result = recv(*client, message, sizeof(message) - 1, 0);	//끝에 null을 포함 하지 않음.
		if (result == SOCKET_ERROR) {
			printf("에러코드 %d\n", WSAGetLastError());
			return -1;
		}
		message[result] = '\0';

		if (strcmp(message, "no_id") == 0) {
			GotoXY(3, 28);	printf("   중복되는 아이디가 있습니다   ");
			GotoXY(3, 29);	printf("ESC키를 누르고 다시 입력하십시오.");
			while (GetKey()!=ESC);
		}
		else if (strcmp(message, "no_nickname") == 0) {
			GotoXY(3, 28);	printf("   중복되는 닉네임이 있습니다.   ");
			GotoXY(3, 29);	printf("ESC키를 누르고 다시 입력하십시오.");
			while (GetKey()!=ESC);
		}

	} while (strcmp(message, "welcome") != 0);

	strcpy(player->name, nickName);
	GotoXY(3, 28);	printf("  환영합니다. %s 님", player->name);
	GotoXY(5, 29);	printf("ENTER키를 누르면 입장합니다.");
	while (GetKey() != ENTER);

	return 0;
}

//로그인
//	login/<아이디>/<비밀번호>
int Login(SOCKET* client, PLAYERINFO* player) {
	char message[1024] = { '\0' };
	char id[MAX];
	char password[MAX];
	int result = 0;

	do {
		SetMyCursor(TRUE);	//커서 온
		DrawLoginWindow(); //로그인 화면 출력

		GotoXY(18, 22);	gets_s(id, sizeof(id));
		GotoXY(18, 24); Input_password(password);

		SetMyCursor(FALSE);	//커서 오프

		// message => login/id/password
		strcpy(message, "login/");
		strcat(message, id);
		strcat(message, "/");
		strcat(message, password);

		result = send(*client, message, strlen(message), 0);
		if (result == SOCKET_ERROR) {
			printf("에러코드 %d\n", WSAGetLastError());
			return -1;
		}

		result = recv(*client, message, sizeof(message) - 1, 0);	//끝에 null을 포함 하지 않음.
		if (result == SOCKET_ERROR) {
			printf("에러코드 %d\n", WSAGetLastError());
			return -1;
		}
		message[result] = '\0';

		if (strcmp(message, "no_id") == 0) {
			GotoXY(3, 26);	printf("   존재하지 않는 아이디 입니다.  \n");
			GotoXY(3, 27);	printf("ESC키를 누르고 다시 입력하십시오.\n");
			while (GetKey() != ESC);
		}
		else if (result = strcmp(message, "no_password") == 0) {
			GotoXY(3, 26);	printf("    비밀번호 가 틀렸습니다.      \n");
			GotoXY(3, 27);	printf("ESC키를 누르고 다시 입력하십시오.\n");
			while (GetKey() != ESC);
		}
		else
			break;

	} while (result != 0);

	// <닉네임>/<최고점수>
	char* tok = NULL;
	tok = strtok(message, "/");
	if (tok != NULL) strcpy(player->name, tok);
	tok = strtok(NULL, "/");
	if (tok != NULL) player->maxScore = atoi(tok);

	GotoXY(5, 26); printf("   어서오세요 %s 님\n", player->name);
	GotoXY(5, 27); printf("ENTER키를 누르면 입장합니다.");
	while (GetKey() != ENTER);

	return 0;
}

//랭킹 받기
//	[send] : rank/<닉네임>
//  [recv] : <닉네임>/<점수>/ ...(10등까지) .. /<닉네임>/<점수>/myRank/<점수>/<등수>
int Ranking(SOCKET* client, PLAYERINFO* player) {
	char message[1024] = { '\0' };
	int result = 0;

	strcpy(message, "rank/");
	strcat(message, player->name);

	//정보 요청
	result = send(*client, message, strlen(message), 0);
	if (result == SOCKET_ERROR) {
		printf("에러코드 %d\n", WSAGetLastError());
		return -1;
	}

	//정보 받기
	result = recv(*client, message, sizeof(message) - 1, 0);	//끝에 null을 포함 하지 않음.
	if (result == SOCKET_ERROR) {
		printf("에러코드 %d\n", WSAGetLastError());
		return -1;
	}
	message[result] = '\0';

	int count = 1;
	char* tok = strtok(message, "/");
	char name[32];
	char score[32];
	char rank[32];
	while (1) {
		strcpy(name, tok);
		tok = strtok(NULL, "/");
		strcpy(score, tok);
		tok = strtok(NULL, "/");

		printf("< %2d등 > %s\t\t%s점\n", count, name, score);
		if (strcmp(tok, "myRank") == 0) break;
		count += 1;
	}

	tok = strtok(NULL, "/");
	strcpy(score, tok);	//내 점수
	tok = strtok(NULL, "/");
	strcpy(rank, tok);	//내 등수

	printf("나의 점수는 %s점 %s등 입니다.\n", score, rank);

	while (1) { if (GetKey() == ESC) break; }

	return 0;
}

//점수 전송
// score/<닉네임>/<점수>
int SendScore(SOCKET* client, PLAYERINFO* player) {
	char message[1024];
	char maxScore[32];
	int result = 0;

	strcpy(message, "score/");
	strcat(message, player->name);
	strcat(message, "/");
	sprintf(maxScore, "%d", player->maxScore);
	strcat(message, maxScore);

	//정보 요청
	result = send(*client, message, strlen(message), 0);
	if (result == SOCKET_ERROR) {
		printf("에러코드 %d\n", WSAGetLastError());
		return -1;
	}

	return 0;
}

// 비밀번호 입력 함수
// * 문자로 표시
int Input_password(char* password) {
	char key;	//키보드 키
	int index = 0;	//배열 위치 값

	while (1) {
		key = _getch();

		switch (key) {
		case ENTER:
			return index;
			break;

		case BACKSPACE:
			if (index > 0) {
				index--;
				password[index] = '\0';
				printf("\b \b");
			}
			break;

		case ESC:
			return -1;
			break;
		default:
			//소문자 a~z , 대문자 A~Z , 숫자 1~9 입력 가능.
			//특수문자 입력 불가
			if ((key >= 'a' && key <= 'z') ||
				(key >= 'A' && key <= 'Z') ||
				(key >= '1' && key <= '9'))
			{
				if (index < MAX - 2) {
					password[index] = key;
					password[index + 1] = '\0';
					index++;
					printf("*");
				}
			}

			break;
		}
	}

}

//부딪힐때
void Hit(int whatStar, PLAYERINFO* player) {

	switch (whatStar) {
	case STAR:
		Beep(_A, 100);
		if (player->strong == 1)	player->strong -= 1; //무적이라면 피해 x
		else player->hp -= 1;	//무적이 아니라면 HP -1
		if (player->hp <= 0)	gameOver = 1;	//HP 가 0 이 되면 게임 오버
		break;

	case HEALINGSTAR:
		Beep(_C, 100);
		if (player->hp < MAX_HP)	player->hp += 1;	//치유별: 목숨 증가
		break;

	case FASTSTAR:
		Beep(_C, 100);
		if (player->boost < MAX_BOOST) player->boost += 1; //빨리빨리 별: 부스트 증가
		break;

	case STRONGSTAR:
		Beep(_C, 100);
		player->strong = 1;//튼튼 별: 무적 부여
		break;

	case DEATHSTAR:
		Beep(_A, 100);
		if (player->strong == 1) player->strong -= 1; //무적이라면 피해 x
		else gameOver = 1; //즉사 별똥별은 HP 상관없이 게임오버
		break;
	}

}

// 게임 오버
void GameOver(PLAYERINFO* player) {
	FILE* file;
	int i;

	Sleep(300);

	for (i = 0; i < 3; i++) {
		GotoXY(15, (int)CONSOLESIZE_LINES / 2 - 3);
		SetColor(0, 12);
		printf("게임 오버");
		Sleep(700);

		GotoXY(15, (int)CONSOLESIZE_LINES / 2 - 3);
		printf("         ");
		Sleep(500);
	}
	SetColor(0, 15);


	Clear();	//화면 지움

	if (player->score > player->maxScore)
		player->maxScore = player->score;

	GotoXY(15, (int)CONSOLESIZE_LINES / 2 - 2);			//게임 오버 화면에 표시
	printf("점수 %d", player->score);
	GotoXY(4, (int)CONSOLESIZE_LINES / 2);
	printf(" %s 님의 최고 점수 : %d", player->name, player->maxScore);

	Sleep(2000);

	fopen_s(&file, "non_user.txt", "w");
	if (file != NULL) {
		fprintf(file, "%s %d\n", player->name, player->maxScore);
		fclose(file);
	}

	while (!(_kbhit())) {
		GotoXY(2, (int)CONSOLESIZE_LINES / 2 + 2);
		printf("계속 하시려면 아무키나 누르세요.");
		Sleep(700);
		GotoXY(2, (int)CONSOLESIZE_LINES / 2 + 2);
		printf("                                ");
		Sleep(500);
	}
}