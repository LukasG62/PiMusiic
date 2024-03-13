#include "request.h"

/**
 * \fn mpp_response_t send_connection_request(char *rfid)
 * \brief Envoie une requete pour obtenir la liste des musiques
 * \return mpp_response_t La réponse du serveur
*/
mpp_response_t send_connection_request(socket_t socket, char *rfid) {
    mpp_response_t response;
    mpp_request_t request;

    // On créer la requête
    request = create_mpp_request(MPP_CONNECT, rfid, NULL, NO_MUSIC_ID);

    // On envoie la requête
    send_data(&socket, &request, (serialize_t) serialize_mpp_request);
    // On attend la réponse
    recv_data(&socket, &response, (serialize_t) deserialize_mpp_response);

    return response;
}

/**
 * \fn send_list_music_request(char *rfid)
 * \brief Envoie une requete pour obtenir la liste des musiques
 * \return mpp_response_t La réponse du serveur
 * \see mpp_response_t
*/
mpp_response_t send_list_music_request(char *rfid) {
    mpp_response_t response;
    mpp_request_t request;

    // On envoie la requête
}

/**
 * \fn send_save_music_request(char *rfid, music_t *music)
 * \brief Envoie une requete pour sauvegarder une musique
 * \param rfid Le rfid de l'utilisateur
 * \param music La musique à sauvegarder
 * \return mpp_response_t La réponse du serveur
*/
mpp_response_t send_save_music_request(char *rfid, music_t *music);

/**
 * \fn send_delete_music_request(char *rfid, music_t *music)
 * \brief Envoie une requete pour supprimer une musique
 * \param rfid Le rfid de l'utilisateur
 * \param music La musique à supprimer
 * \return mpp_response_t La réponse du serveur
*/
mpp_response_t send_delete_music_request(char *rfid, time_t musicId);
