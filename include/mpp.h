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
    time_t *musicIds ; /*!< Liste des identifiants de musiques */
    time_t musicId; /*!< Identifiant de la musique selectionnée (timestamp) */
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
 * \fn mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, time_t *musicIds, time_t musicId);
 * \brief Créer une réponse MPP
 * \param code Code de la réponse
 * \param username Nom de l'utilisateur
 * \param music Musique à envoyer
 * \param musicIds Liste des identifiants de musiques
 * \param musicId Identifiant de la musique selectionnée (timestamp)
 * \return La réponse MPP
*/
mpp_response_t create_mpp_response(mpp_response_code_t code, char *username, music_t *music, time_t *musicIds, time_t musicId);

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


#endif