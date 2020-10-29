#include "MyGame.h"

//���α׷� ���� ù �ʱ�ȭ
int Init() {
	SetConsoleTitle(TEXT("StarGame"));	//�ܼ� â ����
	SetConsoleSize(CONSOLESIZE_COL, CONSOLESIZE_LINES);	//�ܼ� â ������ ����
	srand((unsigned)time(NULL)); //������ ���� �õ� ����

	return 0;
}

//SDL_Mixer �ʱ�ȭ
int Sound_init() {
	//SDL
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {	//SDL �ʱ�ȭ
		printf("SDL Init error : %s\n", Mix_GetError());
		return -1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) { //����� �ʱ�ȭ
		printf("SDL Mix_Init error : %s\n", Mix_GetError());
		return -1;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);	//���� �ִ� ������ ������ ����

	sound.BGM = Mix_LoadMUS("music/BGM.wav");
	sound.GET_ITEM_SOUND = Mix_LoadWAV("music/item.wav");
	sound.HIT_SOUND = Mix_LoadWAV("music/hit.wav");

	return 0;
}

//���� ���� �ʱ�ȭ
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