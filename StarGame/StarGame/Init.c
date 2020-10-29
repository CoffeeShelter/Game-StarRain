#include "MyGame.h"

//프로그램 시작 첫 초기화
int Init() {
	SetConsoleTitle(TEXT("StarGame"));	//콘솔 창 제목
	SetConsoleSize(CONSOLESIZE_COL, CONSOLESIZE_LINES);	//콘솔 창 사이즈 변경
	srand((unsigned)time(NULL)); //난수를 위한 시드 생성

	return 0;
}

//SDL_Mixer 초기화
int Sound_init() {
	//SDL
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {	//SDL 초기화
		printf("SDL Init error : %s\n", Mix_GetError());
		return -1;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) { //오디오 초기화
		printf("SDL Mix_Init error : %s\n", Mix_GetError());
		return -1;
	}

	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);	//볼륨 최대 볼륨의 반절로 설정

	sound.BGM = Mix_LoadMUS("music/BGM.wav");
	sound.GET_ITEM_SOUND = Mix_LoadWAV("music/item.wav");
	sound.HIT_SOUND = Mix_LoadWAV("music/hit.wav");

	return 0;
}

//게임 설정 초기화
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