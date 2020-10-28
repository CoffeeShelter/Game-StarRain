#include "MyGame.h"

/*
���˺� ���� �Լ�

�⺻ ���˺�: �÷��̰Ű� �̿� ������ HP -1 ����
��� ���˺�: �÷��̰Ű� �̿� ������ HP�� ���� ���� ���� ����
*/
void SetStar(PLAYERINFO* player) {
	int startStar = 0;	//���˺��� ������ ���� ��ġ
	int maxStarCount = 0;	//�ִ� ���˺� ����

	int item = rand() % 150; //������ Ȯ�� ���� (0~N-1(������ ��ġ) �� ������ 10 = ġ�� ���˺� 11 = �������� ���˺� 12 = ưư ���˺� �ش� ������ ������ ����);

	startStar = rand() % GAMEBOARD_COL;	//���� ��ġ ����

	if (player->score >= 0 && player->score <= 500) {
		maxStarCount = 10;
	}
	else if (player->score > 500 && player->score <= 3000) {
		maxStarCount = 20;
		gameSpeed = 100; // 0.1��
	}
	else if (player->score > 3000 && player->score <= 7000) {
		maxStarCount = 40;
		gameSpeed = 60; // 0.06��
	}
	else if (player->score > 7000) {
		maxStarCount = 50;
		gameSpeed = 44; // 0.44��
	}

	if (starCount < maxStarCount) {
		//�̹� ���˺��� ������ �ٽ� ����
		if (GameBoard[startStar][0] == STAR) {
			SetStar(player);
		}
		else {
			switch (item) {
			case HEALINGSTAR:
				GameBoard[startStar][0] = HEALINGSTAR;	//ġ�� ���ʺ� (HP ����)
				break;
			case FASTSTAR:
				GameBoard[startStar][0] = FASTSTAR;	//�������� ���˺� (�ν�Ʈ Ƚ�� ����)
				break;
			case STRONGSTAR:
				GameBoard[startStar][0] = STRONGSTAR; //���� ���� �ο�
				break;
			case DEATHSTAR:
				GameBoard[startStar][0] = DEATHSTAR;	//���(��� ���� ����)
				break;
			default:
				GameBoard[startStar][0] = STAR;	// �⺻ ���˺� (���� �� HP -1)
				break;
			}
			starCount++;
		}
	}


}
/*
���˺� ���� ���
�÷��̾� �� ������ HP -1
*/
void DropStar(PLAYERINFO* player) {
	int col, line;
	int whatStar = 0;

	//������ ���� ���� �˻�.
	for (col = GAMEBOARD_COL - 1; col >= 0; col--) {
		for (line = GAMEBOARD_LINES - 1; line >= 0; line--) {

			//������ ���˺� Ȯ��
			if (GameBoard[col][line] == STAR || GameBoard[col][line] == HEALINGSTAR ||
				GameBoard[col][line] == FASTSTAR || GameBoard[col][line] == STRONGSTAR
				|| GameBoard[col][line] == DEATHSTAR)
			{
				whatStar = GameBoard[col][line];
				GameBoard[col][line] = BLANK;

				if (line < GAMEBOARD_LINES - 1) {

					//�÷��̾� �ε��� Ȯ��
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
���˺� ���� ->(�ణ�� ��_���Ӽӵ�)-> ���� /�ݺ�/
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