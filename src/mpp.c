#include "mpp.h"


/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/

/**
 * \fn void serialize_music(music_t *music, buffer_t buffer);
 * \brief Sérialise une musique
 * \param music Musique à sérialiser
 * \param buffer Buffer dans lequel sérialiser la musique
*/
void serialize_music(music_t *music, buffer_t buffer);

/**
 * \fn mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId);
 * \brief Créer une requête MPP
 * \param code Code de la requête
 * \param rfidId Identifiant RFID de l'utilisateur
 * \param music Musique à envoyer
 * \param musicId Identifiant de la musique selectionnée (timestamp)
 * \return La requête MPP
*/
mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId) {
    mpp_request_t request;
    request.code = code;
    strcpy(request.rfidId, rfidId);
    request.music = music;
    request.musicId = musicId;
    return request;
}

/**
 * \fn mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, time_t *musicIds, time_t musicId);
 * \brief Créer une réponse MPP
 * \param code Code de la réponse
 * \param username Nom de l'utilisateur
 * \param music Musique à envoyer
 * \param musicIds Liste des identifiants de musiques
 * \param musicId Identifiant de la musique selectionnée (timestamp)
 * \return La réponse MPP
*/
mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, time_t *musicIds, time_t musicId) {
    mpp_response_t response;
    response.code = code;
    strcpy(response.username, username);
    response.music = music;
    response.musicIds = musicIds;
    response.musicId = musicId;
    return response;
}


/**
 * \fn void print_mpp_request(mpp_request_t *request);
 * \brief Affiche une requête MPP
 * \param request Requête MPP
*/
void print_mpp_request(mpp_request_t *request) {
    char *code = code2str_request(request->code);
    printf("Request code: %s\n", code);
    printf("RFID: %s\n", request->rfidId);
    printf("Music ID: %ld\n", request->musicId);
    free(code);
}

/**
 * \fn void print_mpp_response(mpp_response_t *response);
 * \brief Affiche une réponse MPP
 * \param response Réponse MPP
*/
void print_mpp_response(mpp_response_t *response) {
    char *code = code2str_response(response->code);
    printf("Response code: %s\n", code);
    printf("Username: %s\n", response->username);
    printf("Music ID: %ld\n", response->musicId);
    free(code);
}

/**
 * \fn serialize_mpp_request(request_t *request, buffer_t buffer);
 * \brief Sérialise une requête MPP
 * \param request Requête MPP
 * \param buffer Buffer dans lequel sérialiser la requête
*/
void serialize_mpp_request(mpp_request_t *request, buffer_t buffer) {
    /*
    <code> <rfidId>
    <musicId> <bpm>
    <line> <noteid> <octave> <instrument> <time>
    ...
    END
    <line> <noteid> <octave> <instrument> <time>
    ...
    END
    <line> <noteid> <octave> <instrument> <time>
    END

    */
    sprintf(buffer, "%d %s %ld\n", request->code, request->rfidId, request->musicId);
    if(request->music != NULL) serialize_music(request->music, buffer);  
}

/**
 * \fn deserialize_mpp_request(buffer_t buffer, mpp_request_t *request);
 * \brief Désérialise une requête MPP
 * \param buffer Buffer dans lequel désérialiser la requête
 * \param request Requête MPP
*/
void deserialize_mpp_request(buffer_t buffer, mpp_request_t *request) {
    // strtok n'est pas thread safe, on utilise strtok_r donc pour cela on doit déclarer un pointeur saveptr
    char *saveptr;
    int i, j;
    if(strlen(buffer) == 0) BAD_REQUEST(request);
    // lecture de la première ligne
    char *token = strtok_r(buffer, "\n", &saveptr);
    if(token == NULL) BAD_REQUEST(request);
    sscanf(token, "%d %s %ld", (int *) &request->code, request->rfidId, &request->musicId);
    printf("Code: %d\n", request->code);
    printf("RFID: %s\n", request->rfidId);
    printf("Music ID: %ld\n", request->musicId);
    // lecture des musiques si elles existent
    token = strtok_r(NULL, "\n", &saveptr);
    if(token != NULL) {
        request->music = (music_t *)malloc(sizeof(music_t));
        init_music(request->music, 0);
        int channelCount = 0;
        sscanf(token, "%ld %hd", &request->music->date.tv_sec, &request->music->bpm);
        printf("Date: %ld\n", request->music->date.tv_sec);
        printf("BPM: %hd\n", request->music->bpm);
        while(token != NULL && channelCount < MUSIC_MAX_CHANNELS) {
            token = strtok_r(NULL, "\n", &saveptr);
            if(token != NULL) {
                int channelId = channelCount;
                channel_t *channel = &request->music->channels[channelId];
                while(token != NULL && strcmp(token, "END") != 0) {
                    printf("CHANNEL: %d\n", channelId);
                    int line=0;
                    // on récupère d'abord la ligne
                    sscanf(token, "%d", &line);
                    // on récupère les notes
                    note_t *note = &channel->notes[line];
                    sscanf(token, "%d %hd %hd %d %d", &line, &note->id, &note->octave, (int *) &note->instrument,(int *) &note->time);
                    printf("Line: %d ", line);
                    printf("Note ID: %hd ", note->id);
                    printf("Octave: %hd ", note->octave);
                    printf("Instrument: %d ", note->instrument);
                    printf("Time: %d\n", note->time);
                    token = strtok_r(NULL, "\n", &saveptr);
                }
                channelCount++;
            }
        }
    }

}

/**
 * \fn void code2str_request(mpp_request_code_t code, char *str);
 * \brief Convertit un code de requête MPP en chaîne de caractères
 * \param code Code de requête MPP
 * \return La chaîne de caractère
 * \warning La chaine de caractères doit être libérée après utilisation
*/
char *code2str_request(mpp_request_code_t code) {
    char *str = (char *)malloc(50);
    switch (code) {
        case MPP_ADD_MUSIC:
            strcpy(str, "Ajouter une musique");
            break;
        case MPP_GET_MUSIC:
            strcpy(str, "Récupérer une musique");
            break;
        case MPP_LIST_MUSIC:
            strcpy(str, "Lister les musiques");
            break;

        default:
            strcpy(str, "Code inconnu");
            break;

    }
    return str;

}

/**
 * \fn void code2str_response(mpp_response_code_t code, char *str);
 * \brief Convertit un code de réponse MPP en chaîne de caractères
 * \param code Code de réponse MPP
 * \return La chaîne de caractères
 * \warning La chaine de caractères doit être libérée après utilisation
*/
char *code2str_response(mpp_response_code_t code) {
    char *str = (char *)malloc(50);
    switch (code) {
        case MPP_RESPONSE_OK:
            strcpy(str, "L'opération a réussi");
            break;
        case MPP_RESPONSE_NOK:
            strcpy(str, "L'opération a échoué");
            break;

        case MPP_RESPONSE_MUSIC_CREATED:
            strcpy(str, "La musique a été créée");
            break;

        case MPP_RESPONSE_BAD_REQUEST:
            strcpy(str, "La requête est mal formée");
            break;

        case MPP_RESPONSE_MUSIC_UPDATED:
            strcpy(str, "La musique a été mise à jour");
            break;
        
        case MPP_RESPONSE_NOT_FOUND:
            strcpy(str, "La musique n'a pas été trouvée");
            break;

        default:
            strcpy(str, "Code inconnu");
            break;
    }
    return str;

}


/**********************************************************************************************************************/
/*                                           Private Fonction Definitions                                             */
/**********************************************************************************************************************/

/**
 * \fn void serialize_music(music_t *music, buffer_t buffer);
 * \brief Sérialise une musique
 * \param music Musique à sérialiser
 * \param buffer Buffer dans lequel sérialiser la musique
*/
void serialize_music(music_t *music, buffer_t buffer) {
    int i, j;
    sprintf(buffer, "%s%ld %d\n", buffer, music->date.tv_sec, music->bpm);
    // On parcourt chaque channel et on écrit seulement les notes non vides
    for(i = 0; i < MUSIC_MAX_CHANNELS; i++) {
        for(j = 0; j < CHANNEL_MAX_NOTES; j++) {
            note_t note = music->channels[i].notes[j];
            if(note.id != 0) {
                sprintf(buffer, "%s%d %d %d %d %d\n", buffer, j, note.id, note.octave, note.instrument, note.time);
            }
        }
        // On marque la fin du channel
        sprintf(buffer, "%sEND\n", buffer);
    }
}