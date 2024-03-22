/**
 * \file pimusii.c
 * \details Application principale
*/
#include "sound.h"
#include <pthread.h>


snd_pcm_t *pcm1 = NULL;
snd_pcm_t *pcm2 = NULL;
snd_pcm_t *pcm3 = NULL;
short bpm = 150;


int main() {
	init_sound(&pcm3);
	note_t melodie = create_note(1, NOTE_A_FQ, 3,INSTRUMENT_SIN,TIME_RONDE);
	play_note(melodie,120,pcm3);
	end_sound(pcm3);
	
	melodie = create_note(1, NOTE_A_FQ, 2,INSTRUMENT_ORGAN,TIME_RONDE);
	init_sound(&pcm1);

	play_note(melodie,120,pcm1);
	melodie = create_note(1, NOTE_C_FQ, 2,INSTRUMENT_ORGAN,TIME_RONDE);
	//play_note(melodie,120,pcm1);
	end_sound(pcm1);
    return 0;
}
