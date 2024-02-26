/* @file note.h
 * @brief Définition des fonctions pour manipuler une note de musique
 */

#ifndef NOTE_H
#define NOTE_H
/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include <string.h>
#include <sys/time.h>
/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */
#define CHANNEL_MAX_NOTES 50
#define MUSIC_MAX_CHANNELS 3

//Fréquences des notes
#define NOTE_C_FQ 261.63 /*!< Fréquence du DO à l’octave de référence */
#define NOTE_CS_FQ 277.18 /*!< Fréquence du DO# à l’octave de référence */
#define NOTE_D_FQ 293.66 /*!< Fréquence du RÉ à l’octave de référence */
#define NOTE_DS_FQ 311.13 /*!< Fréquence du RÉ# à l’octave de référence */
#define NOTE_E_FQ 329.63 /*!< Fréquence du MI à l’octave de référence */
#define NOTE_F_FQ 349.23 /*!< Fréquence du FA à l’octave de référence */
#define NOTE_FS_FQ 369.99 /*!< Fréquence du FA# à l’octave de référence */
#define NOTE_G_FQ 392.00 /*!< Fréquence du SOL à l’octave de référence */
#define NOTE_GS_FQ 415.30 /*!< Fréquence du SOL# à l’octave de référence */
#define NOTE_A_FQ 440.00 /*!< Fréquence du LA à l’octave de référence */
#define NOTE_AS_FQ 466.16 /*!< Fréquence du LA# à l’octave de référence */
#define NOTE_B_FQ 493.88 /*!< Fréquence du SI à l’octave de référence */
#define NOTE_NA_FQ 0 /*!< Fréquence d’une non note */

//Noms des notes
#define NOTE_C_NAME "C" /*!< Nom du DO à l’octave de référence */
#define NOTE_CS_NAME "C#"/*!< Nom du DO# à l’octave de référence */
#define NOTE_D_NAME "D"/*!< Nom du RÉ à l’octave de référence */
#define NOTE_DS_NAME "D#"/*!< Nom du RÉ# à l’octave de référence */
#define NOTE_E_NAME "E"/*!< Nom du MI à l’octave de référence */
#define NOTE_F_NAME "F"/*!< Nom du FA à l’octave de référence */
#define NOTE_FS_NAME "F#"/*!< Nom du FA# à l’octave de référence */
#define NOTE_G_NAME "G"/*!< Nom du SOL à l’octave de référence */
#define NOTE_GS_NAME "G#"/*!< Nom du SOL# à l’octave de référence */
#define NOTE_A_NAME "A"/*!< Nom du LA à l’octave de référence */
#define NOTE_AS_NAME "A#"/*!< Nom du LA# à l’octave de référence */
#define NOTE_B_NAME "B"/*!< Nom du SI à l’octave de référence */
#define NOTE_NA_NAME "--"/*!< Nom d'une non note */

/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */

/**
 * \enum instrument_t
 * \brief Enumeration des instruments 
 *
 */
typedef enum {
	INSTRUMENT_STEPMOTOR = 0, /*!< Utilisation du moteur pas à pas */
	INSTRUMENT_SIN,/*!< Utilisation d’un signal sinusoïdale */
	INSTRUMENT_SAWTOOTH,/*!< Utilisation d’un signal en dent de scie*/
	INSTRUMENT_TRIANGLE, /*!< Utilisation d’un signal en triangle*/
}instrument_t;


/**
 * \enum time_t
 * \brief Enumeration des durées des notes
 */
typedef enum {

	TIME_CROCHE_DOUBLE = 1,/*!< elle vaut ¼ temps*/
	TIME_CROCHE = 2,/*!< elle vaut ½ temps*/
	TIME_NOIRE = 4,/*!< Valeur par défaut, elle vaut 1 temps*/
	TIME_BLANCHE = 6,/*!< elle vaut 2 temps*/
	TIME_RONDE = 8,/*!< elle vaut 4 temps*/

}time_duration_t;

/**
 * \struct note_t
 * \brief Structure des notes
 */
typedef struct {
	char note[3];/*!<  Nom de la note*/
	double frequency; /*!< Fréquence en Hz à l’octave de référence*/
	short octave;/*!< octave de la note*/
	instrument_t instrument; /*!< Istrument sur lequel la jouer*/
	time_t time;/*!<  Durée de la note */
}note_t;

/**
 * \struct channel_t
 * \brief Structure pour jouer les notes dans les channels
 */
typedef struct {
	short id ; /*!< Identifiant du channel*/ 
	note_t notes [CHANNEL_MAX_NOTES];/*!< Nombre de note (dernière note non vide)*/
	int nbNotes;/*!< Fréquence en Hz à l’octave de référence*/
}channel_t;

/**
 * \struct music_t
 * \brief Structure de la musique
 */
typedef struct {
	struct timeval date;/*!< Date de création de la musique*/
	channel_t channels [MUSIC_MAX_CHANNELS];/*!< Les canaux disponibles */
	int bpm;/*!< Le bpm de la musique*/
}music_t;

/* ------------------------------------------------------------------------ */
/*                 V A R I A B L E S    G L O B A L E S                     */
/* ------------------------------------------------------------------------ */
char* list_scale [13] = {NOTE_C_NAME,NOTE_CS_NAME,NOTE_D_NAME,NOTE_DS_NAME,NOTE_E_NAME,NOTE_F_NAME,NOTE_FS_NAME,NOTE_G_NAME,NOTE_GS_NAME,NOTE_A_NAME,NOTE_AS_NAME,NOTE_B_NAME,NOTE_NA_NAME};


/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */
/**
 * \fn note_t create_note(char[3] note, double frequency, short octave,instrument_t instrument, time_t time);
 * \brief Créé une note avec les param donnés
 * \param note Nom de la note
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t create_note(char note[3], double frequency, short octave,instrument_t instrument, time_duration_t time);

/**
 * \fn void mod_note(note_t noteModif, char[3] note, double frequency, short octave,instrument_t instrument, time_t time);
 * \brief modifie la note avec les param donnés
 * \param noteModif note à modifier
 * \param note Nom de la note
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t * mod_note(note_t * noteModif,char note[3], double frequency, short octave,instrument_t instrument, time_duration_t time);

/**
 * \fn char* get_next_note(char[3] note);
 * \brief récupérer la note suivante avec les paramètres donnés
 * \param note note de référence 
 */
char* get_next_note(char note[3] );

/**
 * \fn char* get_previous_note(char[3] note);
 * \brief récupérer la note précédente avec les paramètres donnés
 * \param note note de référence 
 */
char* get_previous_note(char note[3]);

/**
 * \fn note_t * cp_note(note_t * dest, note_t src);
 * \brief copier les données d'une note vers une autre
 * \param dest note qui va etre modifiée
 * \param src note qui va etre copiée 
 */
note_t * cp_note(note_t * dest, note_t src);

#endif
