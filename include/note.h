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
#define CHANNEL_MAX_NOTES 4096 /*!< Nombre de notes maximum dans un channel doit tenir sur n symboles hexadécimaux */
#define MUSIC_MAX_CHANNELS 3 /*!< Nombre de channels maximum dans une musique */

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
#define NB_NOTES 13 /*!< Nombre de notes dans une octave en incluant la non note */

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
	INSTRUMENT_NA /*!< Pas d’instrument*/
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
 * \struct scale_t
 * \brief Structure representant une gamme
*/
typedef struct {
	char list_scale[NB_NOTES][3];/*!< Liste des notes de la gamme*/
	short nbNotes;/*!< Nombre de note dans la gamme*/
	short current; /*!< position dans la gamme*/
}scale_t;


/**
 * \struct channel_t
 * \brief Structure pour jouer les notes dans les channels
 */
typedef struct {
	short id ; /*!< Identifiant du channel*/ 
	note_t notes[CHANNEL_MAX_NOTES];/*!< Nombre de note (dernière note non vide)*/
	int nbNotes;/*!< Fréquence en Hz à l’octave de référence*/
}channel_t;

/**
 * \struct music_t
 * \brief Structure de la musique
 */
typedef struct {
	struct timeval date;/*!< Date de création de la musique*/
	channel_t channels [MUSIC_MAX_CHANNELS];/*!< Les canaux disponibles */
	short bpm;/*!< Le bpm de la musique*/
}music_t;


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
 * \fn note_t *mod_note(note_t *noteModif,char note[3], double frequency, short octave,instrument_t instrument, time_duration_t time);
 * \brief modifie la note avec les param donnés
 * \param noteModif note à modifier
 * \param note Nom de la note
 * \param frequency Fréquence en Hz à l’octave de référence
 * \param octave octave de la note
 * \param instrument Istrument sur lequel la jouer
 * \param time Durée de la note
 */
note_t *mod_note(note_t *noteModif,char note[3], double frequency, short octave,instrument_t instrument, time_duration_t time);


/**
 * \fn scale_t init_scale()
 * \brief Initialiser la gamme
 * \return la gamme initialisée
 * \details Cette fonction initialise la gamme avec le nom des notes
*/
scale_t init_scale();

/**
 * \fn char* get_next_note(scale_t *scale);
 * \brief récupérer la note suivante
 * \param scale la gamme
 * \return la note suivante
 * \note Elle met à jour la position dans la gamme
 */
char *get_next_note(scale_t *scale);

/**
 * \fn char* get_previous_note(scale_t *scale);
 * \brief récupérer la note précédente avec les paramètres donnés
 * \param note note de référence 
 */
char *get_previous_note(scale_t *scale);

/**
 * \fn note_t *cp_note(note_t *dest, note_t src);
 * \brief copier les données d'une note vers une autre
 * \param dest note qui va etre modifiée
 * \param src note qui va etre copiée 
 */
note_t *cp_note(note_t *dest, note_t src);

/**
 * \fn void init_channel(channel_t *channel);
 * \brief Initialiser un channel avec des notes vides
 * \param channel le channel à initialiser
 * \param id l'identifiant du channel
 * \see channel_t
 */
void init_channel(channel_t *channel, int id);

/**
 * \fn init_music(music_t *music, short bpm);
 * \brief Initialiser une musique avec des channels vides
 * \param music la musique à initialiser
 * \param bpm le bpm de la musique
*/
void init_music(music_t *music, short bpm);


#endif
