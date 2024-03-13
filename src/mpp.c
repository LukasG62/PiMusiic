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
 * \fn void deserialize_music(buffer_t buffer, music_t *music);
 * \brief Désérialise une musique contenue dans un buffer
 * \param music Musique à désérialiser
 * \param token position dans le buffer ou se trouve la musique sérialisée
 * \param saveptr pointeur de sauvegarde pour strtok_r
*/
void deserialize_music(char *token, music_t *music, char *saveptr);


/**********************************************************************************************************************/
/*                                           Public  functions                                                        */
/**********************************************************************************************************************/


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
 * \fn mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds);
 * \brief Créer une réponse MPP
 * \param code Code de la réponse
 * \param username Nom de l'utilisateur
 * \param music Musique à envoyer
 * \param musicIds Liste des identifiants de musiques
 * \param musicId Identifiant de la musique selectionnée (timestamp)
 * \return La réponse MPP
*/
mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds) {
    mpp_response_t response;
    response.code = code;
    strcpy(response.username, username);
    response.music = music;
    response.musicIds = musicIds;
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
    int i; 
    char *code = code2str_response(response->code);
    printf("Response code: %s\n", code);
    printf("Username: %s\n", response->username);
    if(response->musicIds != NULL) {
        printf("Music IDs: ");
        for(i = 0; i < response->musicIds->size; i++) {
            printf("%ld ", response->musicIds->musicIds[i]);
        }
        printf("\n");
    }
    free(code);
}

/**
 * \fn serialize_mpp_request(request_t *request, buffer_t buffer);
 * \brief Sérialise une requête MPP
 * \param request Requête MPP
 * \param buffer Buffer dans lequel sérialiser la requête
*/
void serialize_mpp_request(mpp_request_t *request, buffer_t buffer) {
    /* On sérialise une requête MPP de la manière suivante :
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
    char *saveptr = NULL;
    int i, j;
    if(strlen(buffer) == 0) BAD_REQUEST(request);
    // lecture de la première ligne
    char *token = strtok_r(buffer, "\n", &saveptr);
    if(token == NULL) BAD_REQUEST(request);
    sscanf(token, "%d %s %ld", (int *) &request->code, request->rfidId, &request->musicId);
    // lecture des musiques si elles existent
    token = strtok_r(NULL, "\n", &saveptr);
    if(token != NULL) {
        request->music = (music_t *)malloc(sizeof(music_t));
        init_music(request->music, 0);
        deserialize_music(token, request->music, saveptr);
    }
    else {
        request->music = NULL;
    }
}

/**
 * \fn serialize_mpp_response(mpp_response_t *response, buffer_t buffer);
 * \brief Sérialise une réponse MPP
 * \param response Réponse MPP
 * \param buffer Buffer dans lequel sérialiser la réponse
*/
void serialize_mpp_response(mpp_response_t *response, buffer_t buffer) {
    int i;
    /**
     * <code> <username>
     * <list_size> <musicId> <musicId> <musicId> ...
     * <music> (même format que pour la requête)
    */
    sprintf(buffer, "%d %s\n", response->code, response->username);
    if(response->musicIds != NULL) {
        sprintf(buffer, "%s%d\n", buffer, response->musicIds->size);
        for(i = 0; i < response->musicIds->size; i++) {
            sprintf(buffer, "%s%ld\n", buffer, response->musicIds->musicIds[i]);
        }
        sprintf(buffer, "%s", buffer);
    } else {
        // permet de discriminer les cas ou la liste est vide 
        sprintf(buffer, "%s0\n", buffer);
    }

    if(response->music != NULL) serialize_music(response->music, buffer);
}

/**
 * \fn deserialize_mpp_response(buffer_t buffer, mpp_response_t *response);
 * \brief Désérialise une réponse MPP
 * \param buffer Buffer dans lequel désérialiser la réponse
 * \param response Réponse MPP
*/
void deserialize_mpp_response(buffer_t buffer, mpp_response_t *response) {
    // strtok n'est pas thread safe, on utilise strtok_r donc pour cela on doit déclarer un pointeur saveptr
    // TODO : robustesse !!
    char *saveptr = NULL;
    int i, j;
    // lecture de la première ligne
    char *token = strtok_r(buffer, "\n", &saveptr);
    if(token == NULL) BAD_REQUEST(response);
    sscanf(token, "%d %s", (int *) &response->code, response->username);

    // Creation de la liste de musiques 
    token = strtok_r(NULL, "\n", &saveptr);
    printf("Token: %s\n", token);
    if(token != NULL) {
        response->musicIds = (musicId_list_t *)malloc(sizeof(musicId_list_t));
        init_music_list(response->musicIds);
        int size;
        sscanf(token, "%d", &size);
        for(i = 0; i < size; i++) {
            token = strtok_r(NULL, "\n", &saveptr);
            time_t musicId;
            sscanf(token, "%ld", &musicId);
            add_music_id(response->musicIds, musicId);
        }
        // On lit la musique
        token = strtok_r(NULL, "\n", &saveptr);
        if(token != NULL) {
            response->music = (music_t *)malloc(sizeof(music_t));
            init_music(response->music, 0);
            deserialize_music(token, response->music, saveptr);
        } 
        else response->music = NULL;

    } else {
        // Si token est NULL, alors il n'y a pas de liste de musiques ni de musique
        response->musicIds = NULL;
        response->music = NULL;
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

/**
 * \fn void init_music_list(musicId_list_t *list);
 * \brief Initialise une liste de musiques
 * \param list Liste de musiques
 * \details La liste est initialisée avec une taille de 0 et un pointeur NULL
*/
void init_music_list(musicId_list_t *list) {
    list->size = 0;
    list->musicIds = NULL;
}

/**
 * \fn void add_music_id(musicId_list_t *list, int musicId);
 * \brief Ajoute un identifiant de musique à une liste de musiques
 * \param list Liste de musiques
 * \param musicId Identifiant de la musique à ajouter
 * \details La liste est réallouée pour contenir un identifiant de plus
 * \note la liste est réallouée toutes les REALLLOC_SIZE musiques
 * \see REALLLOC_SIZE 
*/
void add_music_id(musicId_list_t *list, int musicId) {
    if(list->size % REALLLOC_SIZE == 0) {
        list->musicIds = (time_t *)realloc(list->musicIds, (list->size + REALLLOC_SIZE) * sizeof(time_t));
    }
    list->musicIds[list->size] = musicId;
    list->size++;
}

/**
 * \fn void free_music_list(musicId_list_t *list);
 * \brief Libère une liste de musiques
 * \param list Liste de musiques
 * \details La liste est libérée et son pointeur est mis à NULL
 * \warning La liste doit être initialisée avant d'être libérée
*/
void free_music_list(musicId_list_t *list) {
    free(list->musicIds);
    list->musicIds = NULL;
    list->size = 0;
}

/**
 * \fn void get_music_list_from_db(musicId_list_t *list, char *rfidId);
 * \brief Récupère la liste des musiques dans la base de données
 * \param list Liste de musiques
*/
void get_music_list_from_db(musicId_list_t *list, char *rfidId) {
    char filename[50];
    sprintf(filename, "%s/%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, MPP_DB_MUSIC_FILE);
    FILE *file = fopen(filename, "r");
    // Si le fichier n'existe pas, on initialise la liste
    if(file == NULL) {
        init_music_list(list);
        file = fopen(filename, "w");
        fwrite(list, sizeof(musicId_list_t), 1, file);
        fclose(file);
        return;
    }
    fread(list, sizeof(musicId_list_t), 1, file);
    fclose(file);
}

/**
 * \fn int add_music_to_db(music_t *music, char *rfidId);
 * \brief Ajoute une musique à la base de données
 * \param music Musique à ajouter
 * \param rfidId Identifiant RFID de l'utilisateur
*/
int add_music_to_db(music_t *music, char *rfidId) {
    char filename[50];
    sprintf(filename, "%s/%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, MPP_DB_MUSIC_FILE);
    FILE *file = fopen(filename, "w");
    printf("Filename: %s\n", filename);
    if(file == NULL) {
        return -1;
    }
    // On écrit l'identifiant de la musique dans le fichier
    musicId_list_t list;
    if(fread(&list, sizeof(musicId_list_t), 1, file) == 0) {
        init_music_list(&list);
    }
    int music_exists = search_music(&list, music->date.tv_sec);
    if (music_exists == -1) {
        printf("Music does not exist\n");
        add_music_id(&list, music->date.tv_sec);

        printf("Size: %d\n", list.size);
    }
    fwrite(&list, sizeof(musicId_list_t), 1, file);
    fclose(file);

    // On écrit la musique dans un fichier séparé
    sprintf(filename, "%s/%s/%s/%ld.mipi", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, music->date.tv_sec);
    printf("Filename: %s\n", filename);
    file = fopen(filename, "w");
    if(file == NULL) {
        return -1;
    }
    fwrite(music, sizeof(music_t), 1, file);
    fclose(file);
}

/**
 * \fn get_music_from_db(music_t *music, time_t musicId, char *rfidId);
 * \brief Récupère une musique de la base de données
 * \param music Musique à récupérer
 * \param musicId Identifiant de la musique à récupérer
 * \param rfidId Identifiant RFID de l'utilisateur
*/
void get_music_from_db(music_t *music, time_t musicId, char *rfidId) {
    char filename[50];
    sprintf(filename, "%s/%s/%s/%ld.mipi", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, musicId);
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        return;
    }
    fread(music, sizeof(music_t), 1, file);
    fclose(file);

}

/**
 * \fn void delete_music_from_db(time_t musicId, char *rfidId);
 * \brief Supprime une musique de la base de données
 * \param musicId Identifiant de la musique à supprimer
 * \param rfidId Identifiant RFID de l'utilisateur
 * \details La musique est supprimée de la base de données
*/
void delete_music_from_db(time_t musicId, char *rfidId) {
    char filename[50];
    sprintf(filename, "%s/%s/%s/%ld.mipi", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, musicId);
    remove(filename);
    sprintf(filename, "%s/%s/%s/%s", MPP_DB_FOLDER, MPP_DB_MUSIC_FOLDER, rfidId, MPP_DB_MUSIC_FILE);
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        return;
    }
    char line[50];
    FILE *temp = fopen("temp", "w");
    while(fgets(line, 50, file) != NULL) {
        time_t id;
        sscanf(line, "%ld", &id);
        if(id != musicId) {
            fprintf(temp, "%s", line);
        }
    }
    fclose(file);
    fclose(temp);
    remove(filename);
    rename("temp", filename);
}

/**
 * \fn void search_music(musicId_list_t *list, time_t musicId);
 * \brief Recherche une musique dans une liste de musiques
 * \param music Musique à mettre à jour
 * \param musicId Identifiant de la musique à mettre à jour
 * \return l'indice de la musique dans la liste si elle existe, -1 sinon
*/
int search_music(musicId_list_t *list, time_t musicId) {
    int i;
    for(i = 0; i < list->size; i++) {
        if(list->musicIds[i] == musicId) {
            return i;
        }
    }
    return -1;
}

/**
 * \fn void get_username_from_db(char *username, char *rfidId);
 * \brief Récupère le nom d'utilisateur dans la base de données
 * \param username Nom de l'utilisateur
 * \param rfidId Identifiant RFID de l'utilisateur
 * \details Le nom d'utilisateur est récupéré de la base de données 
 * \note si l'utilisateur n'existe pas, le nom est égal au caractère nul
*/
void get_username_from_db(char *username, char *rfidId) {
    char filename[50];  
    sprintf(filename, "%s/%s", MPP_DB_FOLDER, MPP_DB_USER_FILE);
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        username[0] = '\0';
        return;
    }
    char line[50];
    while(fgets(line, 50, file) != NULL) {
        char id[50];
        char name[50];
        sscanf(line, "%s %s", id, name);
        if(strcmp(id, rfidId) == 0) {
            strcpy(username, name);
            fclose(file);
            return;
        }
    }
}

/**
 * \fn void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête de connexion
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient le nom de l'utilisateur
*/
void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        BAD_REQUEST(response);
        return;
    }
    response->code = MPP_RESPONSE_OK;
}

/**
 * \fn void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête pour lister les musiques
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient la liste des identifiants de musiques
*/
void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        BAD_REQUEST(response);
        return;
    }
    // On récupère la liste des musiques
    response->musicIds = (musicId_list_t *)malloc(sizeof(musicId_list_t));
    get_music_list_from_db(response->musicIds, request->rfidId);
    response->code = MPP_RESPONSE_OK;
}

/**
 * \fn void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête pour ajouter une musique
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient la musique ajoutée
*/
void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        BAD_REQUEST(response);
        return;
    }
    // On ajoute la musique à la base de données
    add_music_to_db(request->music, request->rfidId);
    response->code = MPP_RESPONSE_MUSIC_CREATED;
}

/**
 * \fn void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête pour récupérer une musique
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient la musique récupérée
*/
void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response) {
    // On vérifie que l'utilisateur est présent dans user.db
    get_username_from_db(response->username, request->rfidId);
    if(*response->username == '\0') {
        BAD_REQUEST(response);
        return;
    }
    // On récupère la musique de la base de données
    response->music = (music_t *)malloc(sizeof(music_t));
    get_music_from_db(response->music, request->musicId, request->rfidId);
    response->code = MPP_RESPONSE_OK;
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

/**
 * \fn void deserialize_music(buffer_t buffer, music_t *music);
 * \brief Désérialise une musique contenue dans un buffer
 * \param music Musique à désérialiser
 * \param token position dans le buffer ou se trouve la musique sérialisée
 * \param saveptr pointeur de sauvegarde pour strtok_r
*/
void deserialize_music(char *token, music_t *music, char *saveptr) {
    int channelCount = 0;
    sscanf(token, "%ld %hd", &music->date.tv_sec, &music->bpm);
    while (token != NULL && channelCount < MUSIC_MAX_CHANNELS) {
        token = strtok_r(NULL, "\n", &saveptr);
        if (token != NULL) {
            int channelId = channelCount;
            channel_t *channel = &music->channels[channelId];
            while (token != NULL && strcmp(token, "END") != 0) {
                int line = 0;
                // on récupère d'abord la ligne
                sscanf(token, "%d", &line);
                // on récupère les notes
                note_t *note = &channel->notes[line];
                sscanf(token, "%d %hd %hd %d %d", &line, &note->id, &note->octave, (int *)&note->instrument, (int *)&note->time);
                token = strtok_r(NULL, "\n", &saveptr);
            }
            channelCount++;
        }
    }
}