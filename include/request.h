/**
 * \file note.h
 * \brief Définition des fonctions pour envoyer des requetes MPP
 */

#ifndef REQUEST_H
#define REQUEST_H

#include "mpp.h"

/**
 * \fn mpp_response_t send_connection_request(char *rfid)
 * \brief Envoie une requete pour obtenir la liste des musiques
 * \return mpp_response_t La réponse du serveur
*/
mpp_response_t send_connection_request(socket_t socket, char *rfid);

/**
 * \fn send_list_music_request(char *rfid)
 * \brief Envoie une requete pour obtenir la liste des musiques
 * \return mpp_response_t La réponse du serveur
 * \see mpp_response_t
*/
mpp_response_t send_list_music_request(char *rfid);

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


#endif