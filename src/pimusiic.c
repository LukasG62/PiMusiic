/**
 * \file pimusii.c
 * \details Application principale
*/
#include "sound.h"

int main() {
	note_t melodie [8];
	melodie[0] = create_note(1, NOTE_C_FQ, 2,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[1] = create_note(2, NOTE_D_FQ, 3,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[2] = create_note(3, NOTE_E_FQ, 3,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[3] = create_note(4, NOTE_F_FQ, 2,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[4] = create_note(5, NOTE_G_FQ, 3,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[5] = create_note(6, NOTE_A_FQ, 3,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[6] = create_note(7, NOTE_B_FQ, 3,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	melodie[7] = create_note(8, NOTE_C_FQ, 4,INSTRUMENT_TRIANGLE, TIME_NOIRE);
	
	snd_pcm_t *pcm = NULL;
	init_sound(&pcm);
	short bpm = 60;
	for(int i=0;i<8;i++){
		play_note(melodie[i],bpm,pcm);
	}
	end_sound(pcm);
	
	
	
	
	
	
	
    return 0;
}
