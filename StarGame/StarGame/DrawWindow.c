#include "myGame.h"

void DrawGameDIsplay_(PLAYERINFO* player);	//������ ���
void DrawState_(PLAYERINFO* player); //����â ���

//���� â
void DrawGameDIsplay_(PLAYERINFO* player) {

	int col, line;

	//���� �÷��� ȭ��
	for (col = 0; col < GAMEBOARD_COL; col++) {

		for (line = 0; line < GAMEBOARD_LINES; line++) {

			GotoXY(col, line);

			switch (GameBoard[col][line]) {
				//���� (���)
			case BLANK:
				SetColor(0, 0);
				printf(" ");
				SetColor(0, 15);
				break;
				//�÷��̾�
			case PLAYER:
				//�������� ��Ʈ ������ ǥ��
				if (player->strong) {
					SetColor(11, 0);
					printf(" ");
					SetColor(0, 15);
				}
				//������� �Ķ�
				else {
					SetColor(9, 0);
					printf(" ");
					SetColor(0, 15);
				}
				break;
				//�⺻ ���˺�
			case STAR:
				SetColor(14, 0);
				printf(" ");
				SetColor(0, 15);
				break;
				//ġ�� ���˺� : ��
			case HEALINGSTAR:
				SetColor(0, 12);
				printf("H");
				SetColor(0, 15);
				break;
				//�������� ���˺� : �Ķ���
			case FASTSTAR:
				SetColor(0, 9);
				printf("F");
				SetColor(0, 15);
				break;
				//ưư ���˺� : ȸ��
			case STRONGSTAR:
				SetColor(0, 8);
				printf("S");
				SetColor(0, 15);
				break;
				//���ù��� ����...��? (��� ���˺�) : ������
			case DEATHSTAR:
				SetColor(12, 0);
				printf(" ");
				SetColor(0, 15);
				break;
			}
		}

	}
}

//���� ����â
void DrawState_(PLAYERINFO* player) {

	int col, line;

	//���� ���� â ���
	for (col = 0; col < GAMEBOARD_STATE_COL; col++) {

		for (line = GAMEBOARD_LINES; line < GAMEBOARD_LINES + GAMEBOARD_STATE_LINES; line++) {

			GotoXY(col, line);
			SetColor(8, 15);
			printf(" ");

		}

	}

	//���� ����

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

//��Ʈ�� ȭ�� ���
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

//���� ���
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
		printf("���� ������ �о�� �� �����ϴ�.\n");
	}



	//�⺻ ���˺�
	GotoXY(3, 12);
	SetColor(14, 0);
	printf(" ");

	//��� ���˺�
	GotoXY(3, 14);
	SetColor(12, 0);
	printf(" ");

	//ġ�� ���˺�
	GotoXY(3, 16);
	SetColor(0, 12);
	printf("H");

	//�������� ���˺�
	GotoXY(3, 18);
	SetColor(0, 9);
	printf("F");

	//ưư ���˺�
	GotoXY(3, 20);
	SetColor(0, 8);
	printf("S");

	//�⺻ ���� �÷��̾�
	GotoXY(3, 24);
	SetColor(9, 0);
	printf(" ");

	//���� ���� �÷��̾�
	GotoXY(3, 26);
	SetColor(11, 0);
	printf(" ");



	SetColor(0, 15);

	while (GetKey() != ESC);
}

//���� ȭ�� ��ü ���
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

//�α��� â ���
void DrawLoginWindow() {
	Clear();	//ȭ�� ����
	DrawIntro();	//StarGame �̹��� ���
	GotoXY(7, 22);
	printf(" �α��� :");
	GotoXY(7, 24);
	printf("��й�ȣ :");
}

//ȸ������ â ���
void DrawSigninWindow() {
	Clear();	//ȭ�� ����
	DrawIntro();	//StarGame �̹��� ���
	GotoXY(7, 22);
	printf(" �α��� :");
	GotoXY(7, 24);
	printf("��й�ȣ :");
	GotoXY(7, 26);
	printf(" �г��� :");

}