#include "MyGame.h"

void Init();	//�ý���(�ܼ�â ũ��...����..���) �ʱ�ȭ _���� �ѹ� ����

void Init() {

	SetConsoleTitle(TEXT("StarGame"));	//�ܼ� â ����
	SetConsoleSize(CONSOLESIZE_COL, CONSOLESIZE_LINES);	//�ܼ� â ������ ����
	srand((unsigned)time(NULL)); //������ ���� �õ� ����

}

void GameInit(PLAYERINFO* player) {
	SetMyCursor(FALSE);	//Ŀ�� ����
	player->x = (int)GAMEBOARD_COL / 2;
	player->y = GAMEBOARD_LINES - 1;
	player->hp = MAX_HP;
	//player.hp = MAX_HP;			//�÷��̾� ���� �ʱ�ȭ
	player->boost = MAX_BOOST;
	player->score = 0;
	player->strong = 0;

	gameOver = 0;

	starCount = 0;	//�����Ǿ��� �ִ� �� ����.
	gameSpeed = 160; //���� ���ǵ�

	memset(GameBoard, 0, sizeof(GameBoard));	//������ ��ǥ 0���� �ʱ�ȭ
	GameBoard[player->x][player->y] = PLAYER; //�÷��̾� ��ġ �ʱ�ȭ
}