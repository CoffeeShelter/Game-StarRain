#include "myGame.h"

//SDL_mixer ����
int Release_Sound() {

	Mix_FreeMusic(sound.BGM);				//�����
	Mix_FreeChunk(sound.GET_ITEM_SOUND);	//������ ȹ��
	Mix_FreeChunk(sound.HIT_SOUND);		//�ε��� ��

	return 0;
}