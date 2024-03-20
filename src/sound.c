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
void switch_instrument(short * buffer,note_t note,double freq,size_t time);

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
    snd_pcm_hw_params_set_period_time(*pcm, hw_params, 100000, 0); // 0.1 seconds

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
void play_note(note_t note,short bpm,snd_pcm_t *pcm){
	
	
	//fonction qui transforme un note_t en freq ( réelle )
	double freq = noteToFreq(note);
	
	//calculer la durée de la note en fonction du bpm
	size_t time = noteToTime(note,bpm);
	
	short * buffer = (short*)malloc(sizeof(short)*time);
	
	switch_instrument(buffer,note,freq,time);//on joue la note 
	

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

/**
 * \fn switch_instrument()
 * \brief joue une note sur un instrument
 * \param note_t note note à jouer
 * \param double freq frequence réelle de la note
 * \param double time durée du temps
 */
 //sample rate x la durée = sample_count
void switch_instrument(short *buffer,note_t note,double freq,size_t time){
	
	
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
		
		default : 
				silent_wave(buffer,time,freq);
		break;
		
	}

	
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
short * pdt_convolution(short * buffer1,short * buffer2,size_t time){
	int i,j = 0;
	short * buffer = (short*)malloc(sizeof(short)*time);
	for(i=0;i<time;i++){
		//la valeur de notre t
		short somme = 0;
		for(j=0;j<time;j++){
			if(i>=j)
				somme=buffer1[j]*buffer[i-j]+somme;
		}
		if(somme<BASE_AMPLITUDE)
			buffer[i]=somme;
		else
			buffer[i]=BASE_AMPLITUDE;
	}	

	return buffer; 
}
/* ------------------------------------------------------------------------ */


