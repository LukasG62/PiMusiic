/**
 * \file graphicseq.c
 * \details Couche d'abstraction de ncurses pour les élément graphiques du sequenceur
 * \author Lukas Grando
 * \version 1.0
*/
#include "graphicseq.h"

/**********************************************************************************************************************/
/*                                           Private functions                                                        */
/**********************************************************************************************************************/

/**
 * \fn void init_colors()
 * \brief Initialisation des couleurs
 * \details Cette fonction s'occupe de l'initialisation des couleurs
 */
void init_colors() {
    // Initialisation des couleurs
    // TODO : CHANGER LES COULEURS
    init_pair(COLOR_PAIR_MENU, COLOR_BLUE, COLOR_WHITE);
    init_pair(COLOR_PAIR_MENU_WARNING, COLOR_RED, COLOR_WHITE);
    init_pair(COLOR_PAIR_MENU_PROMPT, COLOR_GREEN, COLOR_WHITE);
    init_pair(COLOR_PAIR_SEQ, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SELECTED, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_PAIR_SEQ_BORDER, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_PLAYED, COLOR_BLACK, COLOR_WHITE);
    init_pair(COLOR_PAIR_SEQ_OCTAVE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_NOTE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_INSTRUMENT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SHIFT, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_CH1, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_CH2, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_CH3, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_INFO, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_HEADER_TITLE, COLOR_WHITE, COLOR_BLACK);
    
}

/**
 * \fn void create_custom_colors(int color_id, const char *color)
 * \brief Création de couleurs personnalisées pour ncurses en passant par des codes hexadécimaux
 * \param color_id L'identifiant de la couleur
 * \param color La couleur en hexadécimal
 * \return La couleur personnalisée
 */
int create_custom_colors(int color_id, const char *color) {
    int r, g, b;
    sscanf(color, "#%02x%02x%02x", &r, &g, &b);
    // transformation des valeurs hexadécimales en valeurs de 0 à 1000
    r = r * 1000 / 255;
    g = g * 1000 / 255;
    b = b * 1000 / 255;
    // création de la couleur
    init_color(color_id, r, g, b);
    return color_id;      
}

/**
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \param centered Centre le texte verticalement si différent de 0
 * \note Elle ne gère pas la navigation dans le menu
 */
void init_menu(const char *title, const char *text, int centered) {
    int lenTitle = strlen(title);
    int lenText = strlen(text);
    // On change la couleur du background
    wbkgd(stdscr, COLOR_PAIR(COLOR_PAIR_MENU));
    // On ajoute les bordures
    box(stdscr, 0, 0);
    // On affiche le titre centrée en haut dans la bordure en gras
    attron(COLOR_PAIR(COLOR_PAIR_MENU) | A_BOLD);
    mvprintw(0, (RPI_COLS - lenTitle) / 2, "%s", title);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU) | A_BOLD);

    // Si le texte doit être centré
    if (centered) {
        // On centre le texte verticalement
        mvprintw(RPI_LINES/2, (RPI_COLS - lenText) / 2, "%s", text);
    }
    else {
        mvprintw(3, 4, "%s", text);
    }
    refresh();
}

/**
 * \fn choice_t create_menu(const char *choices[], int n_choices, int highlight, choices_t choices)
 * \brief Création d'un menu ncurses
 * \details Cette fonction crée un menu ncurses
 * \param title Le titre du menu
 * \param text Le texte du menu (optionnel)
 * \param choices[] Les choix du menu
 * \param nbChoices Le nombre de choix
 * \param highlight La couleur du choix sélectionné
 * \param choices Valeur de retour de chaque choix
 * 
*/
choices_t create_menu(const char *title, const char *text, char **choices, int nbChoices, int highlight, choices_t *choices_return) {
    clear(); // on nettoie l'écran
    int c; // la touche pressée
    int curr = highlight; // le choix actuel
    init_menu(title, text, 0); // Initialisation du menu

    // On affiche les choix
    for (int i = 0; i < nbChoices; i++) {
        if (i == curr) {
            attron(A_REVERSE);
            mvprintw(5 + i, 8, "%s", choices[i]);
        } 
        else {
            mvprintw(5 + i, 8, "%s", choices[i]);
        }
        attroff(A_REVERSE);
    }

    // On gère la navigation dans le menu
    while(1) {
        c = getch();
        switch(c) {
            case KEY_UP:
                if (curr > 0) curr--;
                break;
            case KEY_DOWN:
                if (curr < nbChoices - 1) curr++;
                break;
            case 10:
                return choices_return[curr];
            break;
        }
        for (int i = 0; i < nbChoices; i++) {
            if (i == curr) attron(A_REVERSE);
            mvprintw(5 + i, 8, "%s", choices[i]);
            attroff(A_REVERSE);
        }
    }
    refresh();
}

/**********************************************************************************************************************/
/*                                           Public functions                                                         */
/**********************************************************************************************************************/

/**
 * \fn void init_ncurses()
 * \brief Initialisation de ncurses et de la fenêtre
 * \details Cette fonction s'occupe de l'initialisation de ncurses, des couleurs, de la fenêtre et des bordures
 */
void init_ncurses() {
    // Initialisation de la fenêtre
    initscr(); // Initialisation de ncurses
    noecho(); // Désactivation de l'affichage des caractères saisis
    cbreak(); // Désactivation du buffering de ligne
    keypad(stdscr, TRUE); // Activation des touches spéciales
    curs_set(0); // Désactivation du curseur
    start_color(); // Activation des couleurs
    init_colors(); // Initialisation des couleurs
}

/**
 * \fn void exit_ncurses()
 * \brief Fin de ncurses
 * \details Cette fonction s'occupe de la fin de ncurses
 */
void exit_ncurses() {

    // Fin de la fenêtre
    endwin(); // Fin de ncurses
}

/**
 * \fn choices_t show_main_menu()
 * \brief Affichage du menu principal
 * \details Cette fonction affiche le menu principal et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see choices_t
 */
choices_t show_main_menu() {
    // On définit le titre du menu
    char title[] = "Pimusic Application";
    // On définit le texte du menu
    char text[] = "Welcome to Pimusic, please choose an option :";
    // On définit les choix du menu
    char *choices[] = {
        "1. Connect to retrieve music",
        "2. Create music",
        "3. Quit"
    };
    // On récupère le nombre de choix
    int n_choices = sizeof(choices) / sizeof(char *);
    // Les valeurs de retour de chaque choix
    choices_t choices_return[] = {
        CHOICE_CONECTION_MENU,
        CHOICE_SEQUENCER,
        CHOICE_QUITAPP,
    };

    // On affiche le menu
    return create_menu(title, text, choices, n_choices, 0, choices_return);

}

/**
 * \fn choices_t show_connection_menu
 * \brief Affichage du menu de connexion
 * \details Cette fonction affiche le menu de connexion et gère la navigation dans le menu
 * \return Le choix de l'utilisateur
 * \see choices_t
 */
choices_t show_connection_menu() {
    // Affichage du menu
}


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
choices_t show_create_music_menu(music_t *music, int connected) {
    init_menu("Create music", "", 1);
    music->bpm = 120;
    gettimeofday(&music->date, NULL);
    if(connected == 0) {
        attron(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
        mvprintw(3, 4, "%s", "Warning : You're in offline mode, you can't save your music !");
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_WARNING) | A_BOLD);
    }
    attron(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    mvprintw(5, 4, "%s", "Creation date : ");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    mvprintw(5, 20, "%ld", music->date.tv_sec);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));

    attron(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    mvprintw(7, 4, "%s", "Select the bpm by pressing the arrows : ");
    
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    mvprintw(7, 45, "%3d", music->bpm);
    attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));

    // Option pour créer ou retourner au menu principal en bas de l'écran
    attron(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    mvprintw(RPI_LINES - 3, 4, "%s", "[BTN0] Return to main menu");
    mvprintw(RPI_LINES - 3, RPI_COLS-22, "%s", "[BTN1] Create music");
    attroff(COLOR_PAIR(COLOR_PAIR_MENU | A_BOLD));
    refresh();
    // On gère la navigation dans le menu
    int c;
    while(1) {
        c = getch();
        switch(c) {
            case KEY_UP:
                if (music->bpm < 300) music->bpm++;
                break;
            case KEY_DOWN:
                if (music->bpm > 0) music->bpm--;
                break;
            case 10:
                return CHOICE_SEQUENCER;
            
            case 27:
                return CHOICE_MAIN_MENU;

            break;
        }
        attron(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
        mvprintw(7, 45, "%3d", music->bpm);
        attroff(COLOR_PAIR(COLOR_PAIR_MENU_PROMPT));
    }
    refresh();
}
