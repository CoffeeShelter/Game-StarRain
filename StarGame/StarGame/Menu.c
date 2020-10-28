#include "MyGame.h"



/*
�α���
ȸ������
*/

int LoginMenu() {
	char key = 0;
	int choice = LOGIN;

	Clear();
	DrawIntro();	//��Ʈ�� ȭ�� ���

	while (!((key == ENTER) && (choice == GAMEEXIT))) {

		GotoXY(15, LOGIN);
		printf("�� �� ��");

		GotoXY(15, SIGNIN);
		printf("ȸ������");

		GotoXY(15, GAMEEXIT);
		SetColor(0, 4);
		printf("���� ����");
		SetColor(0, 15);

		GotoXY(12, choice);
		printf("��");

		key = GetKey();

		switch (key) {
		case UP:
			if (choice != LOGIN) {
				GotoXY(12, choice);
				printf("   ");
				choice -= 2;
				GotoXY(12, choice);
				printf("��");
			}
			break;

		case DOWN:
			if (choice != GAMEEXIT) {
				GotoXY(12, choice);
				printf("   ");
				choice += 2;
				GotoXY(12, choice);
				printf("��");
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
���ӽ���
���Ӽ���
��ŷȮ��
��������
*/

int Menu() {
	char key = 0;
	int choice = GAMESTART;

	Clear();
	DrawIntro();	//��Ʈ�� ȭ�� ���

	while (!((key == ENTER) && (choice == GAMEEXIT))) {


		GotoXY(15, GAMESTART);
		printf("���� ����");

		GotoXY(15, GAMEEXPLAIN);
		printf("���� ����");

		GotoXY(15, RANKING);
		printf("��ŷ Ȯ��");

		GotoXY(15, GAMEEXIT);
		SetColor(0, 4);
		printf("���� ����");
		SetColor(0, 15);

		GotoXY(12, choice);
		printf("��");

		key = GetKey();

		switch (key) {
			
		case UP:
			if (choice != GAMESTART) {
				GotoXY(12, choice);
				printf("   ");
				choice -= 2;
				GotoXY(12, choice);
				printf("��");
			}
			break;

		case DOWN:
			if (choice != GAMEEXIT) {
				GotoXY(12, choice);
				printf("   ");
				choice += 2;
				GotoXY(12, choice);
				printf("��");
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