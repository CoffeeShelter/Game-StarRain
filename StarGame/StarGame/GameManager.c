#include "myGame.h"

#define MAX 16
#define _C (DWORD)1046.502
#define _D (DWORD)1108.731
#define _E (DWORD)1318.510
#define _F (DWORD)1396.913
#define _G (DWORD)1567.982
#define _A (DWORD)1760.000
#define _B (DWORD)1975.533

int Input_password(char* password);	//��й�ȣ �Է� (* �� ǥ��)

void GameRun(PLAYERINFO* player) {
	pthread_t myThread[2];
	int threadResult;

	GameInit(player);

	//�����̴� �Լ� ������
	threadResult = pthread_create(&myThread[0], NULL, StarLogic, (void*)player);
	if (threadResult < 0) {
		printf("������ ����\n");
		exit(1);
	}

	//����� �Լ� ������
	threadResult = pthread_create(&myThread[1], NULL, PlayerMove, (void*)player);
	if (threadResult < 0) {
		printf("������ ����\n");
		exit(1);
	}

	DrawConsole(player);

	GameOver(player);
}

//Ű �Է�
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

//ȸ�� ����
//	signin/<���̵�>/<��й�ȣ>/<�г���>
int Signin(SOCKET* client, PLAYERINFO* player) {
	char message[1024] = { '\0' };
	char id[MAX];
	char password[MAX];
	char nickName[32];
	int result = 0;
	
	do {
		SetMyCursor(TRUE);	//Ŀ�� ��

		DrawSigninWindow();	//ȸ������ ȭ�� ���

		GotoXY(18, 22);		gets_s(id, sizeof(id));			//���̵� �Է�
		GotoXY(18, 24);		Input_password(password);		//��й�ȣ �Է�
		GotoXY(18, 26);		gets_s(nickName, sizeof(id));	//�г��� �Է�
		
		SetMyCursor(FALSE);	//Ŀ�� ����

		//message => signin/id/password/nickName
		strcpy(message, "signin/");
		strcat(message, id);
		strcat(message, "/");
		strcat(message, password);
		strcat(message, "/");
		strcat(message, nickName);

		//����
		result = send(*client, message, strlen(message), 0);
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			return -1;
		}
		//�ޱ�
		result = recv(*client, message, sizeof(message) - 1, 0);	//���� null�� ���� ���� ����.
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			return -1;
		}
		message[result] = '\0';

		if (strcmp(message, "no_id") == 0) {
			GotoXY(3, 28);	printf("   �ߺ��Ǵ� ���̵� �ֽ��ϴ�   ");
			GotoXY(3, 29);	printf("ESCŰ�� ������ �ٽ� �Է��Ͻʽÿ�.");
			while (GetKey()!=ESC);
		}
		else if (strcmp(message, "no_nickname") == 0) {
			GotoXY(3, 28);	printf("   �ߺ��Ǵ� �г����� �ֽ��ϴ�.   ");
			GotoXY(3, 29);	printf("ESCŰ�� ������ �ٽ� �Է��Ͻʽÿ�.");
			while (GetKey()!=ESC);
		}

	} while (strcmp(message, "welcome") != 0);

	strcpy(player->name, nickName);
	GotoXY(3, 28);	printf("  ȯ���մϴ�. %s ��", player->name);
	GotoXY(5, 29);	printf("ENTERŰ�� ������ �����մϴ�.");
	while (GetKey() != ENTER);

	return 0;
}

//�α���
//	login/<���̵�>/<��й�ȣ>
int Login(SOCKET* client, PLAYERINFO* player) {
	char message[1024] = { '\0' };
	char id[MAX];
	char password[MAX];
	int result = 0;

	do {
		SetMyCursor(TRUE);	//Ŀ�� ��
		DrawLoginWindow(); //�α��� ȭ�� ���

		GotoXY(18, 22);	gets_s(id, sizeof(id));
		GotoXY(18, 24); Input_password(password);

		SetMyCursor(FALSE);	//Ŀ�� ����

		// message => login/id/password
		strcpy(message, "login/");
		strcat(message, id);
		strcat(message, "/");
		strcat(message, password);

		result = send(*client, message, strlen(message), 0);
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			return -1;
		}

		result = recv(*client, message, sizeof(message) - 1, 0);	//���� null�� ���� ���� ����.
		if (result == SOCKET_ERROR) {
			printf("�����ڵ� %d\n", WSAGetLastError());
			return -1;
		}
		message[result] = '\0';

		if (strcmp(message, "no_id") == 0) {
			GotoXY(3, 26);	printf("   �������� �ʴ� ���̵� �Դϴ�.  \n");
			GotoXY(3, 27);	printf("ESCŰ�� ������ �ٽ� �Է��Ͻʽÿ�.\n");
			while (GetKey() != ESC);
		}
		else if (result = strcmp(message, "no_password") == 0) {
			GotoXY(3, 26);	printf("    ��й�ȣ �� Ʋ�Ƚ��ϴ�.      \n");
			GotoXY(3, 27);	printf("ESCŰ�� ������ �ٽ� �Է��Ͻʽÿ�.\n");
			while (GetKey() != ESC);
		}
		else
			break;

	} while (result != 0);

	// <�г���>/<�ְ�����>
	char* tok = NULL;
	tok = strtok(message, "/");
	if (tok != NULL) strcpy(player->name, tok);
	tok = strtok(NULL, "/");
	if (tok != NULL) player->maxScore = atoi(tok);

	GotoXY(5, 26); printf("   ������� %s ��\n", player->name);
	GotoXY(5, 27); printf("ENTERŰ�� ������ �����մϴ�.");
	while (GetKey() != ENTER);

	return 0;
}

//��ŷ �ޱ�
//	[send] : rank/<�г���>
//  [recv] : <�г���>/<����>/ ...(10�����) .. /<�г���>/<����>/myRank/<����>/<���>
int Ranking(SOCKET* client, PLAYERINFO* player) {
	char message[1024] = { '\0' };
	int result = 0;

	strcpy(message, "rank/");
	strcat(message, player->name);

	//���� ��û
	result = send(*client, message, strlen(message), 0);
	if (result == SOCKET_ERROR) {
		printf("�����ڵ� %d\n", WSAGetLastError());
		return -1;
	}

	//���� �ޱ�
	result = recv(*client, message, sizeof(message) - 1, 0);	//���� null�� ���� ���� ����.
	if (result == SOCKET_ERROR) {
		printf("�����ڵ� %d\n", WSAGetLastError());
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

		printf("< %2d�� > %s\t\t%s��\n", count, name, score);
		if (strcmp(tok, "myRank") == 0) break;
		count += 1;
	}

	tok = strtok(NULL, "/");
	strcpy(score, tok);	//�� ����
	tok = strtok(NULL, "/");
	strcpy(rank, tok);	//�� ���

	printf("���� ������ %s�� %s�� �Դϴ�.\n", score, rank);

	while (1) { if (GetKey() == ESC) break; }

	return 0;
}

//���� ����
// score/<�г���>/<����>
int SendScore(SOCKET* client, PLAYERINFO* player) {
	char message[1024];
	char maxScore[32];
	int result = 0;

	strcpy(message, "score/");
	strcat(message, player->name);
	strcat(message, "/");
	sprintf(maxScore, "%d", player->maxScore);
	strcat(message, maxScore);

	//���� ��û
	result = send(*client, message, strlen(message), 0);
	if (result == SOCKET_ERROR) {
		printf("�����ڵ� %d\n", WSAGetLastError());
		return -1;
	}

	return 0;
}

// ��й�ȣ �Է� �Լ�
// * ���ڷ� ǥ��
int Input_password(char* password) {
	char key;	//Ű���� Ű
	int index = 0;	//�迭 ��ġ ��

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
			//�ҹ��� a~z , �빮�� A~Z , ���� 1~9 �Է� ����.
			//Ư������ �Է� �Ұ�
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

//�ε�����
void Hit(int whatStar, PLAYERINFO* player) {

	switch (whatStar) {
	case STAR:
		Beep(_A, 100);
		if (player->strong == 1)	player->strong -= 1; //�����̶�� ���� x
		else player->hp -= 1;	//������ �ƴ϶�� HP -1
		if (player->hp <= 0)	gameOver = 1;	//HP �� 0 �� �Ǹ� ���� ����
		break;

	case HEALINGSTAR:
		Beep(_C, 100);
		if (player->hp < MAX_HP)	player->hp += 1;	//ġ����: ��� ����
		break;

	case FASTSTAR:
		Beep(_C, 100);
		if (player->boost < MAX_BOOST) player->boost += 1; //�������� ��: �ν�Ʈ ����
		break;

	case STRONGSTAR:
		Beep(_C, 100);
		player->strong = 1;//ưư ��: ���� �ο�
		break;

	case DEATHSTAR:
		Beep(_A, 100);
		if (player->strong == 1) player->strong -= 1; //�����̶�� ���� x
		else gameOver = 1; //��� ���˺��� HP ������� ���ӿ���
		break;
	}

}

// ���� ����
void GameOver(PLAYERINFO* player) {
	FILE* file;
	int i;

	Sleep(300);

	for (i = 0; i < 3; i++) {
		GotoXY(15, (int)CONSOLESIZE_LINES / 2 - 3);
		SetColor(0, 12);
		printf("���� ����");
		Sleep(700);

		GotoXY(15, (int)CONSOLESIZE_LINES / 2 - 3);
		printf("         ");
		Sleep(500);
	}
	SetColor(0, 15);


	Clear();	//ȭ�� ����

	if (player->score > player->maxScore)
		player->maxScore = player->score;

	GotoXY(15, (int)CONSOLESIZE_LINES / 2 - 2);			//���� ���� ȭ�鿡 ǥ��
	printf("���� %d", player->score);
	GotoXY(4, (int)CONSOLESIZE_LINES / 2);
	printf(" %s ���� �ְ� ���� : %d", player->name, player->maxScore);

	Sleep(2000);

	fopen_s(&file, "non_user.txt", "w");
	if (file != NULL) {
		fprintf(file, "%s %d\n", player->name, player->maxScore);
		fclose(file);
	}

	while (!(_kbhit())) {
		GotoXY(2, (int)CONSOLESIZE_LINES / 2 + 2);
		printf("��� �Ͻ÷��� �ƹ�Ű�� ��������.");
		Sleep(700);
		GotoXY(2, (int)CONSOLESIZE_LINES / 2 + 2);
		printf("                                ");
		Sleep(500);
	}
}