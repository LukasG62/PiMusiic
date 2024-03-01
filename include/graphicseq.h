/**
 * \file graphicseq.h
 * \details Couche d'abstraction de ncurses pour les élément graphiques du sequenceur
*/

#ifndef GRAPHIC_SEQ_H
#define GRAPHIC_SEQ_H

#include <ncurses.h>
#include <string.h>
#include "note.h"

#define RPI_COLS 106 /*!< Nombre de colonnes de la fenêtre sur le RPI */
#define RPI_LINES 30 /*!< Nombre de lignes de la fenêtre sur le RPI */

// X : Colonne Y : Ligne

// Constantes pour le l'entête d'information du séquenceur
#define SEQUENCER_INFO_X0 0 /*!< Position X de l'entête d'information */
#define SEQUENCER_INFO_Y0 0 /*!< Position Y de l'entête d'information */
#define SEQUENCER_INFO_LINES 6 /*!< Largeur de l'entête d'information */
#define SEQUENCER_INFO_COLS 52 /*!< Hauteur de l'entête d'information */
// Constantes pour le l'entête d'aide du séquenceur
#define SEQUENCER_HELP_X0 53 /*!< Position X de l'entête d'aide */
#define SEQUENCER_HELP_Y0 0 /*!< Position Y de l'entête d'aide */
#define SEQUENCER_HELP_LINES 6 /*!< Largeur de l'entête d'aide */
#define SEQUENCER_HELP_COLS 53 /*!< Hauteur de l'entête d'aide */
// Constantes pour le body du séquenceur
#define SEQUENCER_BODY_X0 0 /*!< Position X du body */
#define SEQUENCER_BODY_Y0 7 /*!< Position Y du body */
#define SEQUENCER_BODY_LINES 23 /*!< Largeur du body */
#define SEQUENCER_BODY_COLS 106 /*!< Hauteur du body */

#define SEQUENCER_CH1_X0 7 /*!< Position X du channel 1 */
#define SEQUENCER_CH2_X0 37 /*!< Position X du channel 2 */
#define SEQUENCER_CH3_X0 67 /*!< Position X du channel 3 */
#define SEQUENCER_CH1_Y0 8 /*!< Position Y du channel 1 */
#define SEQUENCER_CH2_Y0 8 /*!< Position Y du channel 2 */
#define SEQUENCER_CH3_Y0 8 /*!< Position Y du channel 3 */



#define PC_ENTER 10 /*!< Touche entrée sur PC */

/**
 * \enum choices_t
 * \brief Enumération des choix possibles dans les menus
 * \details Cette énumération permet de définir les choix possibles dans les menus
*/
typedef enum {
    CHOICE_QUITAPP, /*!< Quitter l'application */
    CHOICE_HELP, /*!< Afficher l'aide */
    CHOICE_CONECTION_MENU, /*!< Aller au menu de connexion */
    CHOICE_MENU_LIST, /*!< Aller au menu de listes */
    CHOICE_SEQUENCER, /*!< Aller au séquenceur */
    CHOICE_SAVENQUIT, /*!< Sauvegarder et quitter */
    CHOICE_CREATEMUSIC, /*!< Créer une musique */
    CHOICE_MAIN_MENU, /*!< Retourner au menu principal */
} choices_t; 

/**
 * \enum color_pairs_t
 * \brief Enumération des paires de couleurs utilisées dans l'application
*/
typedef enum {
    // Couleurs pour les menus
    COLOR_PAIR_MENU = 10, /*!< Couleur du menu */
    COLOR_PAIR_MENU_WARNING, /*!< Couleur du menu en cas d'erreur */
    COLOR_PAIR_MENU_PROMPT, /*!< Couleur du menu d'entrée utilisateur */

    // Couleurs pour le séquenceur
    COLOR_PAIR_SEQ = 20, /*!< Couleur du séquenceur */
    COLOR_PAIR_SEQ_SELECTED, /*!< Couleur du séquenceur sélectionné */
    COLOR_PAIR_SEQ_BORDER, /*!< Couleur des bordures du séquenceur */
    COLOR_PAIR_SEQ_PLAYED, /*!< Couleur de la ligne jouée */
    COLOR_PAIR_SEQ_OCTAVE, /*!< Couleur de la ligne d'octave */
    COLOR_PAIR_SEQ_NOTE, /*!< Couleur de la ligne de note */
    COLOR_PAIR_SEQ_INSTRUMENT, /*!< Couleur de la ligne d'instrument */
    COLOR_PAIR_SEQ_SHIFT, /*!< Couleur de la ligne de temps */
    COLOR_PAIR_SEQ_HEADER_CH1, /*!< Couleur de l'entête du channel 1 */
    COLOR_PAIR_SEQ_HEADER_CH2, /*!< Couleur de l'entête du channel 2 */
    COLOR_PAIR_SEQ_HEADER_CH3, /*!< Couleur de l'entête du channel 3 */
    COLOR_PAIR_SEQ_HEADER_INFO, /*!< Couleur de l'entête d'information */
    COLOR_PAIR_SEQ_HEADER_TITLE, /*!< Couleur de l'entête de titre */
} color_pairs_t;

typedef enum {
    COLOR_LIGHTGREY = 10, /*!< Gris clair */
} custom_colors_t;

/**
 * \fn void init_ncurses()
 * \brief Initialisation de ncurses et de la fenêtre
 * \details Cette fonction s'occupe de l'initialisation de ncurses, des couleurs, de la fenêtre et des bordures
 */
void init_ncurses();

/**
 * \fn void exit_ncurses()
 * \brief Fin de ncurses
 * \details Cette fonction s'occupe de la fin de ncurses
 */
void exit_ncurses();

/**
 * \fn choices_t show_main_menu()
 * \brief Affichage du menu principal
 * \details Cette fonction affiche le menu principal et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see choices_t
 */
choices_t show_main_menu();

/**
 * \fn choices_t show_connection_menu
 * \brief Affichage du menu de connexion
 * \details Cette fonction affiche le menu de connexion et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see choices_t
 */
choices_t show_connection_menu();

/**
 * \fn choices_t show_create_music_menu
 * \brief Affichage du menu de création de musique
 * \details Cette fonction affiche le menu de création de musique et gère la navigation dans le menu
 * \param music La musique à créer
 * \param connected Si l'utilisateur est connecté
 * \note Si l'utilisateur n'est pas connecté, il ne peut pas sauvegarder la musique
 * \note music doit être initialisé et alloué
 * \return Le choix de l'utilisateur
 * \see choices_t
 * \see music_t
 */
choices_t show_create_music_menu(music_t *music, int connected);

/**
 * \fn choices_t show_sequencer
 * \brief Affichage du séquenceur
 * \details Cette fonction affiche le séquenceur et gère la navigation dans le séquenceur
 * \param music La musique à afficher
 * \param connected Si l'utilisateur est connecté
 * \note music doit être initialisé et alloué
 * \return Le choix de l'utilisateur
 * \see choices_t
 * \see music_t
 */
choices_t show_sequencer(music_t *music, int connected);

#endif