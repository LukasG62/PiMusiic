/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "note.h"


/* ------------------------------------------------------------------------ */
/*                  C O D E    D E S    F O N C T I O N S                   */
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
note_t create_note(char note[3] , double frequency, short octave,instrument_t instrument, time_duration_t time){
	note_t  maNote ;
	strncpy(maNote.note,note,3);
	maNote.frequency = frequency ;
	maNote.octave = octave;
	maNote.instrument =instrument;
	maNote.time =time;
	return maNote;
}

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
note_t* mod_note(note_t * noteModif,char note[3], double frequency, short octave,instrument_t instrument, time_duration_t time){
	strncpy(noteModif->note,note,3);
	noteModif->frequency = frequency ;
	noteModif->octave = octave;
	noteModif->instrument =instrument;
	noteModif->time =time;
	return noteModif;
}


/**
 * \fn scale_t init_scale()
 * \brief Initialiser la gamme
 * \return la gamme initialisée
 * \details Cette fonction initialise la gamme avec le nom des notes
*/
scale_t init_scale() {
	scale_t scale;
	scale.current = 0;
	scale.nbNotes = 13;
	strcpy(scale.list_scale[0], NOTE_NA_NAME);
	strcpy(scale.list_scale[1], NOTE_C_NAME);
	strcpy(scale.list_scale[2], NOTE_CS_NAME);
	strcpy(scale.list_scale[3], NOTE_D_NAME);
	strcpy(scale.list_scale[4], NOTE_DS_NAME);
	strcpy(scale.list_scale[5], NOTE_E_NAME);
	strcpy(scale.list_scale[6], NOTE_F_NAME);
	strcpy(scale.list_scale[7], NOTE_FS_NAME);
	strcpy(scale.list_scale[8], NOTE_G_NAME);
	strcpy(scale.list_scale[9], NOTE_GS_NAME);
	strcpy(scale.list_scale[10], NOTE_A_NAME);
	strcpy(scale.list_scale[11], NOTE_AS_NAME);
	strcpy(scale.list_scale[12], NOTE_B_NAME);
	return scale;
}

/**
 * \fn char* get_next_note(scale_t *scale);
 * \brief récupérer la note suivante
 * \param scale la gamme
 * \return la note suivante
 * \note Elle met à jour la position dans la gamme
 */
char *get_next_note(scale_t *scale) {
	scale->current = (scale->current + 1) % scale->nbNotes;
	return scale->list_scale[scale->current];
}

/**
 * \fn char* get_previous_note(scale_t *scale);
 * \brief récupérer la note précédente avec les paramètres donnés
 * \param note note de référence 
 */
char *get_previous_note(scale_t *scale) {
	scale->current = (scale->current - 1) % scale->nbNotes;
	return scale->list_scale[scale->current];
}

/**
 * \fn note_t * cp_note(note_t * dest, note_t src);
 * \brief copier les données d'une note vers une autre
 * \param dest note qui va etre modifiée
 * \param src note qui va etre copiée 
 */
note_t * cp_note(note_t * dest, note_t src){
	strncpy(dest->note,src.note,3);
	dest->frequency = src.frequency ;
	dest->octave = src.octave;
	dest->instrument =src.instrument;
	dest->time =src.time;
	return dest;
}

/**
 * \fn void init_channel(channel_t *channel);
 * \brief Initialiser un channel avec des notes vides
 * \param channel le channel à initialiser
 * \param id l'identifiant du channel
 * \see channel_t
 */
void init_channel(channel_t *channel, int id) {
	note_t note = create_note(NOTE_NA_NAME, NOTE_NA_FQ, -1, INSTRUMENT_NA, TIME_CROCHE_DOUBLE);
	for (int i = 0; i < CHANNEL_MAX_NOTES; i++) channel->notes[i] = note;
	channel->nbNotes = 0; // Aucune note non vide
	channel->id  = id;
}

/**
 * \fn init_music(music_t *music, short bpm);
 * \brief Initialiser une musique avec des channels vides
 * \param music la musique à initialiser
 * \param bpm le bpm de la musique
*/
void init_music(music_t *music, short bpm) {
	music->bpm = bpm;
	for (int i = 0; i < MUSIC_MAX_CHANNELS; i++) init_channel(&music->channels[i], i);
}
