/**
 * \file sound.h
 * \details Couche d'abstraction de la librairie ALSA
 * Permet de créer les signaux sonore et de les jouer
*/
#ifndef SOUND_H
#define SOUND_H

/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <unistd.h> 
#include "note.h"
/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */

#define SAMPLE_RATE 48000
#define BASE_AMPLITUDE 8000

/* ------------------------------------------------------------------------ */
/*                    M A C R O    F O N C T I O N S                        */
/* ------------------------------------------------------------------------ */




/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */

/**
 * \fn void init_sound();
 * \brief initialise la bibliothèque 
 */
void init_sound();


/**
 * \fn void play_sound();
 * \brief joue une note 
 * \param bpm le bpm de la musique 
 * \param note la note à jouer 
 */
void play_note(note_t note,short bpm);




#endif
