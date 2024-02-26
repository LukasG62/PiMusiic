/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "note.h"
/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */
int get_scale(char note [3]);
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
 * \fn char* get_next_note(char[3] note);
 * \brief récupérer la note suivante avec les paramètres donnés
 * \param note note de référence 
 */

char* get_next_note(char note[3] ){
	int result = get_scale(note);
	if(result == 12)
		result = 0;
	else
		result = result+1;
	
	return list_scale[result];
}

/**
 * \fn char* get_previous_note(char[3] note);
 * \brief récupérer la note précédente avec les paramètres donnés
 * \param note note de référence 
 */
char* get_previous_note(char note[3] ){
	int result = get_scale(note);
	if(result == 0)
		result = 12;
	else
		result = result-1;
	
	return list_scale[result];
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

int get_scale(char note[3]){
	for(int i = 0; i<13 ; i++){
		if(!strncmp(note,list_scale[i],3))
			return i	;
	}
}
