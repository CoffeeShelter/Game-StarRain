#include "myGame.h"

//SDL_mixer «ÿ¡¶
int Release_Sound() {

	Mix_FreeMusic(sound.BGM);				//πË∞Ê¿Ω
	Mix_FreeChunk(sound.GET_ITEM_SOUND);	//æ∆¿Ã≈€ »πµÊ
	Mix_FreeChunk(sound.HIT_SOUND);		//∫Œµ˙»˙ ∂ß

	return 0;
}