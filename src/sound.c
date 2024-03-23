#include "sound.h"
/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */


/**
 * \fn short *sine_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *sine_wave(short *buffer, size_t sample_count, double freq);

/**
 * \fn short *square_wave()
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *square_wave(short *buffer, size_t sample_count, double freq);

/**
 * \fn short *sawtooth_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *sawtooth_wave(short *buffer, size_t sample_count, double freq);

/**
 * \fn short *triangle_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *triangle_wave(short *buffer, size_t sample_count, double freq);

/**
 * \fn short **warm_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *warm_wave(short *buffer, size_t sample_count, double freq);

/**
 * \fn short **silent_wave() 
 * \brief joue une note en silence (lol)
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *silent_wave(short *buffer, size_t sample_count,double freq);

/**
 * \fn switch_instrument()
 * \brief joue une note sur un instrument
 * \param note_t note note à jouer
 * \param double freq frequence réelle de la note
 * \param double time durée du temps
 */
void switch_instrument(short * buffer,note_t note,double freq,size_t time,short effect);

/**
 * \fn  noteToTime()
 * \brief transforme une note en temps
 * \param note_t note note à jouer
 * \param short bpm bpm de la musique
 * \return time temps de la note en double
 */
size_t noteToTime(note_t note, short bpm);

/**
 * \fn  noteToFreq()
 * \brief transforme une note en fréquence
 * \param note_t note note à jouer
 * \return frequence de la note en double
 */
double noteToFreq(note_t note);

/**
 * \fn  pdt_convolution()
 * \brief fait un pdt de convolution entre buffer1 et 2 et écrase le buffer 1
 * \param 
 * \return le pointeur sur le buffer résultat
 */
short * pdt_convolution(short * buffer1,short * buffer2,size_t time);

/**
 * \fn short **organ_wave() 
 * \brief joue une note en orgue 
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *organ_wave(short *buffer, size_t sample_count, double freq);


/**
 * \fn short sine_sound(int time, int amplitude , int phase, double freq ) 
 * \brief retourne la valeur de sin 
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short sine_sound(int time, int amplitude , int phase, double freq );

/**
 * \fn  noteToFreq()
 * \brief transforme une note en fréquence
 * \param note_t note note à jouer
 * \return frequence de la note en double
 */
short *sinphaser_wave(short *buffer,size_t time,double freq);

/**
 * \fn  fuzz_effect()
 * \brief transforme une note en fréquence
 * \param note_t note note à jouer
 * \return frequence de la note en double
 */
short *fuzz_effect(short *buffer,size_t time);

/**
 * \fn  compression_effect()
 * \brief transforme une note en fréquence
 * \param note_t note note à jouer
 * \return frequence de la note en double
 */
short * compression_effect(short *buffer,size_t time);
/* ------------------------------------------------------------------------ */
/*                  C O D E    D E S    F O N C T I O N S                   */
/* ------------------------------------------------------------------------ */


/**
 * \fn void init_sound(snd_pcm_t *pcm);
 * \brief initialise la bibliothèque 
 */
void init_sound(snd_pcm_t **pcm){

    snd_pcm_open(pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_alloca(&hw_params);

    snd_pcm_hw_params_any(*pcm, hw_params);
    snd_pcm_hw_params_set_access(*pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(*pcm, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(*pcm, hw_params, 1);
    snd_pcm_hw_params_set_rate(*pcm, hw_params, 48000, 0);
    snd_pcm_hw_params_set_periods(*pcm, hw_params, 10, 0);
    //snd_pcm_hw_params_set_period_time(*pcm, hw_params, 100000, 0); // 0.1 seconds
	snd_pcm_hw_params_set_period_time(*pcm, hw_params, 200000, 0); // 0.1 seconds
    snd_pcm_hw_params(*pcm, hw_params);
    
}

/**
 * \fn void end_sound(snd_pcm_t *pcm);
 * \brief termine le pcm
 */
void end_sound(snd_pcm_t *pcm){
	 snd_pcm_drain(pcm);
    snd_pcm_close(pcm);
}


/**
 * \fn void play_sound()
 * \brief joue une note 
 * \param bpm le bpm de la musique 
 * \param note la note à jouer 
 */
void play_note(note_t note,short bpm,snd_pcm_t *pcm,short effect){
	
	
	//fonction qui transforme un note_t en freq ( réelle )
	double freq = noteToFreq(note);
	
	//calculer la durée de la note en fonction du bpm
	size_t time = noteToTime(note,bpm);
	
	short * buffer = (short*)malloc(sizeof(short)*time);
	
	switch_instrument(buffer,note,freq,time,effect);//on joue la note 
	

        snd_pcm_writei(pcm, buffer, time);
        //usleep(1000000 * durations[i]);

	

}

/**
 * \fn short *sine_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *sine_wave(short *buffer, size_t sample_count, double freq) {
	int i;
    for (i = 0; i < sample_count; i++) {
        buffer[i] = BASE_AMPLITUDE * sin(2 * M_PI * freq * ((double)i / SAMPLE_RATE));
    }
    return buffer;
}


short *sinphaser_wave(short *buffer,size_t time,double freq){
	int i;
	for(i=0;i<time;i++){
	
		buffer[i] = sine_sound(i,BASE_AMPLITUDE , 0 , freq ) + sine_sound( i, BASE_AMPLITUDE ,1/(freq*2),  freq );
	
	}
	return buffer;
}

short sine_sound(int time, int amplitude , int phase, double freq ){
	
	short result = amplitude * sin(2 * M_PI * freq * ((double)time/ SAMPLE_RATE) + phase );
	return result;
}

/**
 * \fn short *sine_chelou_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *organ_wave(short *buffer, size_t sample_count, double freq){
	int i,j;
	
	short * buffer1 = (short*)malloc(sizeof(short)*sample_count);
		
		
		short * buffer2 = (short*)malloc(sizeof(short)*sample_count);
	
		
		
		short * buffer3 =(short*)malloc(sizeof(short)*sample_count);
	
		
		short * buffer4 =(short*)malloc(sizeof(short)*sample_count);
		
		
		short * buffer5 =(short*)malloc(sizeof(short)*sample_count);
	
	for (i = 0; i < sample_count; i++) {
        buffer[i] = BASE_AMPLITUDE * sin(2 * M_PI * (freq) * ((double)i / SAMPLE_RATE));
        
        buffer1[i] = BASE_AMPLITUDE * sin(2 * M_PI * freq*2 * ((double)i / SAMPLE_RATE));
        
        buffer2[i] = BASE_AMPLITUDE * sin(2 * M_PI * freq*3 * ((double)i / SAMPLE_RATE));
        
        buffer3[i] = BASE_AMPLITUDE * sin(2 * M_PI * freq*2/3 * ((double)i / SAMPLE_RATE));
        
        buffer4[i] = BASE_AMPLITUDE * sin(2 * M_PI * freq*6 * ((double)i / SAMPLE_RATE));
        
        buffer1[i] = BASE_AMPLITUDE * sin( M_PI * freq *4 *((double)i / SAMPLE_RATE));
        
        buffer[i]=buffer2[i]+buffer1[i]+buffer[i]+buffer4[i]+buffer5[i]+buffer3[i];
    }
		
		
		
	/*	
	FILE* file = fopen("mongrospcm.raw", "wb");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n","mongrospcm.raw");
        exit(1);
    }

    fwrite(buffer, sizeof(short), sample_count, file);
    fclose(file);*/
		
	free(buffer2);	
	free(buffer1);
	free(buffer3);
	free(buffer4);
	free(buffer5);
	return buffer;
}

/**
 * \fn short *square_wave()
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *square_wave(short *buffer, size_t sample_count, double freq) {
	int samples_full_cycle = (double)SAMPLE_RATE / (double)freq;
	int samples_half_cycle = samples_full_cycle / 2.0f;
	int cycle_index = 0;
	int i = 0;
	for (i = 0; i < sample_count; i++) {
		buffer[i] = cycle_index < samples_half_cycle ? BASE_AMPLITUDE : -BASE_AMPLITUDE;
		cycle_index = (cycle_index + 1) % samples_full_cycle;
	}
	return buffer;
}

/**
 * \fn short *sawtooth_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *sawtooth_wave(short *buffer, size_t sample_count, double freq) {
	int i = 0;
    for (i = 0; i < sample_count; i++) {
        double t = ((double)i / SAMPLE_RATE);
        double frac = t - floor(t); // Partie fractionnaire de t
        buffer[i] = BASE_AMPLITUDE * (2 * frac - 1); // Création du signal de scie
    }
    return buffer;
}


/**
 * \fn short *triangle_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *triangle_wave(short *buffer, size_t sample_count, double freq) {
	int i = 0;	
    for (i = 0; i < sample_count; i++) {
        double t = ((double)i / SAMPLE_RATE) * freq;
        double frac = t - (int)t; // Partie fractionnaire de t
        buffer[i] = BASE_AMPLITUDE * (2 * fabs(frac) - 1); // Utilisation de la fonction valeur absolue pour obtenir le signal triangulaire
    }
    return buffer;
}


/**
 * \fn short **warm_wave() 
 * \brief joue une note en sinus
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *warm_wave(short *buffer, size_t sample_count, double freq) {
	int i = 0;
    for (i = 0; i < sample_count; i++) {
        double t = ((double)i / SAMPLE_RATE);
        double value = 0.0;

        // Somme des sinus harmoniques
        int harmonics = 1;
        for ( harmonics = 1;harmonics <= 10; harmonics++) {
            value += sin(2 * M_PI * freq * harmonics * t) / harmonics;
        }

        buffer[i] = BASE_AMPLITUDE * value; // Ajustez l'amplitude selon vos besoins
    }
    return buffer;
}

/**
 * \fn short **silent_wave() 
 * \brief joue une note en silence (lol)
 * \param short *buffer buffer de short pour la note
 * \param size_t sample_count nb d'échantillonage
 * \param double freq fréquence d'échantillonage
 */
short *silent_wave(short *buffer, size_t sample_count,double freq){
	int i = 0;	
    for (i = 0; i < sample_count; i++) {
        buffer[i] = 0; // on met rien
    }
    return buffer;
}


short *fuzz_effect(short *buffer,size_t time){
int i;
	for(i=0;i<time;i++){
	double normalized_sample = (double)buffer[i]/ BASE_AMPLITUDE;

    // Appliquez la distorsion non linéaire
    double fuzzed_sample = tanh(normalized_sample * 4);

    // Remettez à l'échelle l'échantillon à sa plage d'amplitude originale
    buffer[i] = (short)(fuzzed_sample * BASE_AMPLITUDE);
	
	}

return buffer;

}

short * compression_effect(short *buffer,size_t time){
int i;
	for(i=0;i<time;i++){
		//per unit
	double normalized_sample = (double)buffer[i]/ BASE_AMPLITUDE;
	double compressed_sample = normalized_sample;
	if (fabs(normalized_sample) > 0.5) {
        compressed_sample = (1 + (normalized_sample - 0.5) / 2.0) * 0.5 * (normalized_sample > 0 ? 1 : -1);
    }
		buffer[i]=(short)(compressed_sample * BASE_AMPLITUDE);
	
	}

return buffer;
}

/**
 * \fn switch_instrument()
 * \brief joue une note sur un instrument
 * \param note_t note note à jouer
 * \param double freq frequence réelle de la note
 * \param double time durée du temps
 */
 //sample rate x la durée = sample_count
void switch_instrument(short *buffer,note_t note,double freq,size_t time,short effect){
	
	switch(note.instrument){
		
		case INSTRUMENT_SIN:
			sine_wave(buffer,time,freq);
		break;
		
		case INSTRUMENT_SAWTOOTH:
			warm_wave(buffer,time,freq);
		break;
		
		case INSTRUMENT_TRIANGLE:
			triangle_wave(buffer,time,freq);
		break;
		
		case INSTRUMENT_SQUARE:
			square_wave(buffer,time,freq);
		break;
		
		case INSTRUMENT_ORGAN:
			organ_wave(buffer,time,freq);
		break;
		
		case INSTRUMENT_SINPHASER:
			sinphaser_wave(buffer,time,freq);
		break;
		
		default : 
				silent_wave(buffer,time,freq);
		break;
		
	}
	
	if(effect == 1 ){
		fuzz_effect(buffer,time);
	}
	if(effect == 2 ){
		compression_effect(buffer,time);
	}
	return;
	
}

/**
 * \fn  noteToFreq()
 * \brief transforme une note en fréquence
 * \param note_t note note à jouer
 * \return frequence de la note en double
 */
double noteToFreq(note_t note){
	return note.frequency * pow(2,(double)note.octave-3);
	//return note.frequency;
}


/**
 * \fn  noteToTime()
 * \brief transforme une note en temps
 * \param note_t note note à jouer
 * \param short bpm bpm de la musique
 * \return time temps de la note en double
 */
size_t noteToTime(note_t note, short bpm){
	return round(SAMPLE_RATE*(60.0/bpm)*(note.time/4.0));
}

/**
 * \fn  pdt_convolution()
 * \brief fait un pdt de convolution entre buffer1 et 2 et écrase le buffer 1
 * \param 
 * \return le pointeur sur le buffer résultat
 */
short * pdt_convolution(short * buffer1, short * buffer2, size_t time) {
    short * buffer = (short*)malloc(sizeof(short) * time);
	size_t i,j;
    for ( i = 0; i < time; ++i) {
        short somme = 0;
        for ( j = i; j < time; ++j) {
            somme += buffer1[j] * buffer2[j - i];
        }
        buffer[i] = (somme < BASE_AMPLITUDE) ? (somme > -BASE_AMPLITUDE) ? somme : -BASE_AMPLITUDE : BASE_AMPLITUDE;
        
    }

    for ( i = 0; i < time; ++i) {
        buffer1[i] = buffer[i];
    }
    free(buffer); // Libérer la mémoire allouée pour le buffer temporaire
    return buffer1; // Retourner buffer1 modifié
}


void play_sample(char * fic,snd_pcm_t *pcm){
	
	
FILE *f = fopen(fic, "rb");
if(f==NULL)	{
	printf("erreur fic");
	return;
}
fseek(f, 0, SEEK_END);
long file_size = ftell(f);
fseek(f, 0, SEEK_SET);
short *samples = (short*)malloc(file_size);
fread(samples, 1, file_size, f);
fclose(f);

snd_pcm_writei(pcm, samples, SAMPLE_RATE);

free(samples);

}









