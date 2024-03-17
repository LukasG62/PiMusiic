/**
 * @file request.c
 * @brief Requêtes pour le serveur MPP
 * @version 1.0
 */
#include "request.h"

/**
 * @fn mpp_response_t send_connection_request(socket_t *socket, char *rfid)
 * @brief Envoie une requête de connexion
 * @param socket 
 * @param rfid 
 * @return mpp_response_t 
 */
mpp_response_t send_connection_request(socket_t *socket, char *rfid) {
    mpp_response_t response;
    mpp_request_t request;

    // On créer la requête
    request = create_mpp_request(MPP_CONNECT, rfid, NULL, NO_MUSIC_ID);

    // On envoie la requête
    send_data(socket, &request, (serialize_t) serialize_mpp_request);
    // On attend la réponse
    recv_data(socket, &response, (serialize_t) deserialize_mpp_response);

    return response;
}

/**
 * @fn mpp_response_t send_list_music_request(socket_t *socket, char *rfid)
 * @brief Envoie une requête pour obtenir la liste des musiques
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @return mpp_response_t 
 */
mpp_response_t send_list_music_request(socket_t *socket, char *rfid) {
    mpp_response_t response = create_mpp_response(MPP_RESPONSE_BAD_REQUEST, "", NULL, NULL);
    mpp_request_t request = create_mpp_request(MPP_LIST_MUSIC, rfid, NULL, NO_MUSIC_ID);
    // On envoie la requête
    send_data(socket, &request, (serialize_t) serialize_mpp_request);
    // On attend la réponse
    recv_data(socket, &response, (serialize_t) deserialize_mpp_response);
    return response;
}

/**
 * @fn mpp_response_t send_save_music_request(socket_t *socket, char *rfid, music_t *music)
 * @brief Envoie une requête pour sauvegarder une musique
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @param music La musique à sauvegarder
 * @return mpp_response_t 
 */
mpp_response_t send_save_music_request(socket_t *socket, char *rfid, music_t *music) {
    mpp_response_t response;
    mpp_request_t request = create_mpp_request(MPP_ADD_MUSIC, rfid, music, NO_MUSIC_ID);
    
    send_data(socket, &request, (serialize_t) serialize_mpp_request);
    recv_data(socket, &response, (serialize_t) deserialize_mpp_response);

    return response;
}

/**
 * @fn mpp_response_t send_delete_music_request(socket_t *socket, char *rfid, time_t musicId)
 * @brief Envoie une requête pour supprimer une musique
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @param musicId L'id musique à supprimer
 * @return mpp_response_t 
 */
mpp_response_t send_delete_music_request(socket_t *socket, char *rfid, time_t musicId) {
    mpp_response_t response;
    mpp_request_t request = create_mpp_request(MPP_DELETE_MUSIC, rfid, NULL, musicId);
    
    send_data(socket, &request, (serialize_t) serialize_mpp_request);
    recv_data(socket, &response, (serialize_t) deserialize_mpp_response);

    return response;
}

/**
 * @fn mpp_response_t send_get_music_request(socket_t *socket, char *rfid, time_t musicId)
 * @brief Envoie une requête pour récupérer une musique
 * @param socket La socket de connexion au serveur
 * @param rfid Le rfid de l'utilisateur
 * @param musicId L'identifiant de la musique
 * @return mpp_response_t 
 * @see mpp_response_t
 */
mpp_response_t send_get_music_request(socket_t *socket, char *rfid, time_t musicId) {
    mpp_response_t response;
    mpp_request_t request = create_mpp_request(MPP_GET_MUSIC, rfid, NULL, musicId);
    
    send_data(socket, &request, (serialize_t) serialize_mpp_request);
    recv_data(socket, &response, (serialize_t) deserialize_mpp_response);

    return response;
}