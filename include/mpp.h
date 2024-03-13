/**
 * \file mpp.H
 * \details Interface MusicPi Protocol
*/

#ifndef MPP_H
#define MPP_H

#include "note.h"
#include "data.h"

// TODO : DOIT ÊTRE DEFINI DANS WIRINGPISEQ H
#define RFID_ID_SIZE 10 /*!< Taille de l'identifiant RFID */
#define USERNAME_SIZE 15 /*!< Taille du nom d'utilisateur */
#define REALLLOC_SIZE 10 /*!< Taille de réallouement de la liste d'identifiants de musiques */
#define NO_MUSIC_ID -1 /*!< Identifiant de musique non défini */

#define MPP_DEFAULT_PORT 12345 /*!< Port par défaut du serveur MPP */
#define MPP_DEFAULT_IP "127.0.0.1" /*!< Adresse IP par défaut du serveur MPP */
#define MPP_MAX_CLIENTS 10 /*!< Nombre maximum de clients connectés au serveur MPP */

#define MPP_DB_FOLDER "ressources" /*!< Dossier de stockage de la base de données */
#define MPP_DB_USER_FILE "user.db" /*!< Chemin de la base de données */
#define MPP_DB_MUSIC_FOLDER "music" /*!< Dossier de stockage des musiques */
#define MPP_DB_MUSIC_FILE "music.db" /*!< Chemin de la base de données des musiques */



#define BAD_REQUEST(requestPtr) (requestPtr->code == MPP_RESPONSE_BAD_REQUEST) /*!< Vérifie si la requête est incorrecte */
#define NOK_REQUEST(requestPtr) (requestPtr->code == MPP_RESPONSE_NOK) /*!< Vérifie si la réponse est NOK */
#define NOT_FOUND(requestPtr) (requestPtr->code == MPP_RESPONSE_NOT_FOUND) /*!< Vérifie si la musique n'a pas été trouvée */

/**
 * \enum mpp_request_code_t
 * \brief Code de requête MPP
*/
typedef enum {
    MPP_CONNECT = 200, /*!< Requête de connexion */
    MPP_LIST_MUSIC = 300, /*!< Requête pour lister les musiques */
    MPP_GET_MUSIC, /*!< Requête pour récupérer une musique */
    MPP_ADD_MUSIC, /*!< Requête pour ajouter une musique */
} mpp_request_code_t;

/**
 * \enum mpp_response_code_t
 * \brief Code de réponse MPP
*/
typedef enum {
    MPP_RESPONSE_OK = 200, /*!< Réponse OK */
    MPP_RESPONSE_MUSIC_UPDATED = 201, /*!< Réponse pour indiquer que la musique a été mise à jour */
    MPP_RESPONSE_MUSIC_CREATED = 202, /*!< Réponse pour indiquer que la musique a été créée */
    MPP_RESPONSE_NOK = 400, /*!< Réponse NOK */
    MPP_RESPONSE_BAD_REQUEST = 401, /*!< Réponse pour indiquer que la requête est incorrecte */
    MPP_RESPONSE_NOT_FOUND = 404, /*!< Réponse pour indiquer que la musique n'a pas été trouvée */
} mpp_response_code_t;

/**
 * \struct musicId_list_t
 * \brief Structure de liste d'identifiants de musiques
 * \details elle contient une liste de timestamp de musiques et sa taille 
*/
typedef struct {
    time_t *musicIds; /*!< Liste des identifiants de musiques */
    int size;       /*!< Taille de la liste */
} musicId_list_t;

/**
 * \struct mpp_request_t
 * \brief Structure de representant une requête MPP
 * \details elle contient tous les champs pouvant être envoyés dans une requête MPP
*/
typedef struct {
    mpp_request_code_t code; /*!< Code de la requête */
    char rfidId[RFID_ID_SIZE]; /*!< Identifiant RFID de l'utilisateur */
    music_t *music ; /*!< Musique à envoyer */
    time_t musicId; /*!< Identifiant de la musique selectionnée (timestamp) */
} mpp_request_t;


/**
 * \struct mpp_response_t
 * \brief Structure de representant une réponse MPP
 * \details elle contient tous les champs pouvant être envoyés dans une réponse MPP
*/
typedef struct {
    mpp_response_code_t code; /*!< Code de la réponse */
    char username[USERNAME_SIZE]; /*!< Nom de l'utilisateur */
    music_t *music ; /*!< Musique à envoyer */
    musicId_list_t *musicIds ; /*!< Liste des identifiants de musiques */
} mpp_response_t;

/**
 * \fn mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId);
 * \brief Créer une requête MPP
 * \param code Code de la requête
 * \param rfidId Identifiant RFID de l'utilisateur
 * \param music Musique à envoyer
 * \param musicId Identifiant de la musique selectionnée (timestamp)
 * \return La requête MPP
*/
mpp_request_t create_mpp_request(mpp_request_code_t code, char *rfidId, music_t *music, time_t musicId);

/**
 * \fn mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds, time_t musicId);
 * \brief Créer une réponse MPP
 * \param code Code de la réponse
 * \param username Nom de l'utilisateur
 * \param music Musique à envoyer
 * \param musicIds Liste des identifiants de musiques
 * \return La réponse MPP
*/
mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, musicId_list_t *musicIds);

/**
 * \fn void print_mpp_request(mpp_request_t *request);
 * \brief Affiche une requête MPP
 * \param request Requête MPP
*/
void print_mpp_request(mpp_request_t *request);

/**
 * \fn void print_mpp_response(mpp_response_t *response);
 * \brief Affiche une réponse MPP
 * \param response Réponse MPP
*/
void print_mpp_response(mpp_response_t *response);

/**
 * \fn serialize_mpp_request(request_t *request, buffer_t buffer);
 * \brief Sérialise une requête MPP
 * \param request Requête MPP
 * \param buffer Buffer dans lequel sérialiser la requête
*/
void serialize_mpp_request(mpp_request_t *request, buffer_t buffer);

/**
 * \fn deserialize_mpp_request(buffer_t buffer, mpp_request_t *request);
 * \brief Désérialise une requête MPP
 * \param buffer Buffer dans lequel désérialiser la requête
 * \param request Requête MPP
*/
void deserialize_mpp_request(buffer_t buffer, mpp_request_t *request);

/**
 * \fn serialize_mpp_response(mpp_response_t *response, buffer_t buffer);
 * \brief Sérialise une réponse MPP
 * \param response Réponse MPP
 * \param buffer Buffer dans lequel sérialiser la réponse
*/
void serialize_mpp_response(mpp_response_t *response, buffer_t buffer);

/**
 * \fn deserialize_mpp_response(buffer_t buffer, mpp_response_t *response);
 * \brief Désérialise une réponse MPP
 * \param buffer Buffer dans lequel désérialiser la réponse
 * \param response Réponse MPP
*/
void deserialize_mpp_response(buffer_t buffer, mpp_response_t *response);

/**
 * \fn void code2str_request(mpp_request_code_t code, char *str);
 * \brief Convertit un code de requête MPP en chaîne de caractères
 * \param code Code de requête MPP
 * \return La chaîne de caractère
 * \warning La chaine de caractères doit être libérée après utilisation
*/
char *code2str_request(mpp_request_code_t code);

/**
 * \fn void code2str_response(mpp_response_code_t code, char *str);
 * \brief Convertit un code de réponse MPP en chaîne de caractères
 * \param code Code de réponse MPP
 * \return La chaîne de caractères
 * \warning La chaine de caractères doit être libérée après utilisation
*/
char *code2str_response(mpp_response_code_t code);

/**
 * \fn void init_music_list(musicId_list_t *list);
 * \brief Initialise une liste de musiques
 * \param list Liste de musiques
 * \details La liste est initialisée avec une taille de 0 et un pointeur NULL
*/
void init_music_list(musicId_list_t *list);

/**
 * \fn void add_music_id(musicId_list_t *list, int musicId);
 * \brief Ajoute un identifiant de musique à une liste de musiques
 * \param list Liste de musiques
 * \param musicId Identifiant de la musique à ajouter
 * \details La liste est réallouée pour contenir un identifiant de plus
 * \note la liste est réallouée toutes les REALLLOC_SIZE musiques
 * \see REALLLOC_SIZE 
*/
void add_music_id(musicId_list_t *list, int musicId);

/**
 * \fn void free_music_list(musicId_list_t *list);
 * \brief Libère une liste de musiques
 * \param list Liste de musiques
 * \details La liste est libérée et son pointeur est mis à NULL
 * \warning La liste doit être initialisée avant d'être libérée
*/
void free_music_list(musicId_list_t *list);

/**
 * \fn void get_music_list_from_db(musicId_list_t *list, char *rfidId);
 * \brief Récupère la liste des musiques dans la base de données
 * \param list Liste de musiques
*/
void get_music_list_from_db(musicId_list_t *list, char *rfidId);

/**
 * \fn void add_music_to_db(music_t *music, char *rfidId);
 * \brief Ajoute une musique à la base de données
 * \param music Musique à ajouter
 * \param rfidId Identifiant RFID de l'utilisateur
*/
int add_music_to_db(music_t *music, char *rfidId);

/**
 * \fn void get_music_from_db(music_t *music, time_t musicId);
 * \brief Récupère une musique de la base de données
 * \param music Musique à récupérer
 * \param musicId Identifiant de la musique à récupérer
 * \param rfidId Identifiant RFID de l'utilisateur
*/
void get_music_from_db(music_t *music, time_t musicId, char *rfidId);

/**
 * \fn void delete_music_from_db(time_t musicId, char *rfidId);
 * \brief Supprime une musique de la base de données
 * \param musicId Identifiant de la musique à supprimer
 * \param rfidId Identifiant RFID de l'utilisateur
 * \details La musique est supprimée de la base de données
*/
void delete_music_from_db(time_t musicId, char *rfidId);

/**
 * \fn void search_music(musicId_list_t *list, time_t musicId);
 * \brief Recherche une musique dans une liste de musiques
 * \param music Musique à mettre à jour
 * \param musicId Identifiant de la musique à mettre à jour
*/
int search_music(musicId_list_t *list, time_t musicId);

/**
 * \fn void get_username_from_db(char *username, char *rfidId);
 * \brief Récupère le nom d'utilisateur dans la base de données
 * \param username Nom de l'utilisateur
 * \param rfidId Identifiant RFID de l'utilisateur
 * \details Le nom d'utilisateur est récupéré de la base de données 
 * \note si l'utilisateur n'existe pas, le nom est égal au caractère nul
*/
void get_username_from_db(char *username, char *rfidId);

/**
 * \fn void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête de connexion
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient le nom de l'utilisateur
*/
void connect_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * \fn void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête pour lister les musiques
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient la liste des identifiants de musiques
*/
void list_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * \fn void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête pour ajouter une musique
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient la musique ajoutée
*/
void add_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

/**
 * \fn void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);
 * \brief Gère une requête pour récupérer une musique
 * \param sd Socket du client
 * \param request Requête MPP
 * \param response Réponse MPP
 * \details La réponse contient la musique récupérée
*/
void get_music_handler(socket_t *sd, mpp_request_t *request, mpp_response_t *response);

#endif