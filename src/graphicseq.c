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
void init_colors();

/**
 * \fn void create_custom_colors(int color_id, const char *color)
 * \brief Création de couleurs personnalisées pour ncurses en passant par des codes hexadécimaux
 * \param color_id L'identifiant de la couleur
 * \param color La couleur en hexadécimal
 * \return La couleur personnalisée
 */
int create_custom_colors(int color_id, const char *color);

/**
 * \fn void init_menu(const char *title, const char *text)
 * \brief Initialisation du menu ncurses
 * \details Cette fonction initialise le menu en affichant les bordures, le titre et le texte
 * \param title Le titre du menu
 * \param text Le texte du menu
 * \param centered Centre le texte verticalement si différent de 0
 * \note Elle ne gère pas la navigation dans le menu
 */
void init_menu(const char *title, const char *text, int centered);

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
choices_t create_menu(const char *title, const char *text, char **choices, int nbChoices, int highlight, choices_t *choices_return);

/**
 * \fn show_sequencer_info(WINDOW *win, int mode) 
 * \brief Affichage des informations du séquenceur
 * \details Cette fonction affiche les informations du séquenceur
 * \param win La fenêtre où afficher les informations
 * \param music La musique à afficher
 * \param mode Le mode des boutons (0 pour le mode NAVIGATION, 1 pour le mode EDITION)
 */
void show_sequencer_info(WINDOW *win, music_t *music, int mode);

/**
 * \fn show_sequencer_help(WINDOW *win)
 * \brief Affichage de l'aide du séquenceur
 * \details Cette fonction affiche l'aide du séquenceur
 * \param win La fenêtre où afficher l'aide
 */
void show_sequencer_help(WINDOW *win);

/**
 * \fn init_sequencer_channels(WINDOW *ch1, WINDOW *ch2, WINDOW *ch3, music_t *music)
 * \brief Initialisation des channels du séquenceur
 * \details Cette fonction initialise les channels du séquenceur
 * \param ch1 La fenêtre du channel 1
 * \param ch2 La fenêtre du channel 2
 * \param ch3 La fenêtre du channel 3
 * \param music La musique à afficher
*/
void init_sequencer_channels(WINDOW *ch1, WINDOW *ch2, WINDOW *ch3, music_t *music);

/**
 * \fn void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isCurrLine)
 * \brief Affichage d'une ligne de notes du séquenceur
 * \details Cette fonction affiche une ligne de notes du séquenceur pour un unique channel
 * \param win La fenêtre où afficher les notes
 * \param note La note à afficher
 * \param ch L'identifiant du channel
 * \param line La ligne où afficher la note
 * \param seqNav La structure de navigation dans le séquenceur afin de pouvoir afficher un curseur
 * \param isSelected Si la ligne est la ligne selectionnée
 */
void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isSelected);

/**
 * \fn void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav)
 * \brief Affichage des notes du séquenceur
 * \param win La fenêtre où afficher les notes
 * \param channelId L'identifiant du channel
 * \param seqNav La structure de navigation dans le séquenceur
 * \param music La musique à afficher
 * \see sequencer_nav_t
 */
void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav);

/**
 * \fn void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * \brief Modification d'une note du séquenceur en fonction de la colonne actuel et de la direction de la modification
 * \param note La note à modifier
 * \param col La colonne actuelle
 * \param scale La gamme des notes
 * \param isUp La direction de la modification (0 pour le bas, 1 pour le haut)
 * \see scale_t
*/
void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp);

/**********************************************************************************************************************/
/*                                           Public Fonction Definitions                                              */
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
choices_t show_sequencer(music_t *music, int connected) {
    clear(); // on nettoie l'écran
    WINDOW *seqInfo = newwin(SEQUENCER_INFO_LINES, SEQUENCER_INFO_COLS, SEQUENCER_INFO_Y0, SEQUENCER_INFO_X0);
    WINDOW *seqHelp = newwin(SEQUENCER_HELP_LINES, SEQUENCER_HELP_COLS, SEQUENCER_HELP_Y0, SEQUENCER_HELP_X0);
    WINDOW *seqBody = newwin(SEQUENCER_BODY_LINES, SEQUENCER_BODY_COLS, SEQUENCER_BODY_Y0, SEQUENCER_BODY_X0);
    WINDOW *channelWin[3];

    channelWin[0] = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, SEQUENCER_CH_Y0, SEQUENCER_CH1_X0);
    channelWin[1] = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, SEQUENCER_CH_Y0, SEQUENCER_CH2_X0);
    channelWin[2] = newwin(SEQUENCER_CH_LINES, SEQUENCER_CH_COLS, SEQUENCER_CH_Y0, SEQUENCER_CH3_X0);

    // Des variables pour la navigation dans le séquenceur
    sequencer_nav_t seqNav = create_sequencer_nav();
    scale_t scale = init_scale(); // Initialisation de la gammes
    // On dessine chaque fenêtre
    show_sequencer_info(seqInfo, music, 0);
    show_sequencer_help(seqHelp);
    box(seqBody, 0, 0);
    wrefresh(seqBody);
    mvwprintw(seqBody, 0, 1, "%s", "SEQUENCER");
    wrefresh(seqBody);
    init_sequencer_channels(channelWin[0], channelWin[1], channelWin[2], music);

    // Navigation dans le séquenceur
    // activation des touches spéciales
    keypad(seqBody, TRUE);
    while (1) {
        int c = wgetch(seqBody);
        switch(c) {
            case KEY_UP:
                if(seqNav.col == SEQUENCER_NAV_COL_LINE) {
                    if (seqNav.line > 0) seqNav.line--;
                    if (seqNav.line < seqNav.start[seqNav.ch]) seqNav.start[seqNav.ch] = seqNav.start[seqNav.ch] - SEQUENCER_CH_LINES + 3; // On défile vers le haut
                }
                // Sinon modification de la note
                else {
                    note_t *note = &(music->channels[seqNav.ch].notes[seqNav.line]);
                    change_sequencer_note(note, seqNav.col, scale, 1);
                }
                break;
            case KEY_DOWN:
                if(seqNav.col == SEQUENCER_NAV_COL_LINE) {
                    if (seqNav.line >= seqNav.start[seqNav.ch] + SEQUENCER_CH_LINES - 4) seqNav.start[seqNav.ch] = seqNav.start[seqNav.ch] + SEQUENCER_CH_LINES - 3; // On défile vers le bas
                    if (seqNav.line < CHANNEL_MAX_NOTES - 1) seqNav.line++;
                }
                // Sinon modification de la note
                else {
                    note_t *note = &(music->channels[seqNav.ch].notes[seqNav.line]);
                    change_sequencer_note(note, seqNav.col, scale, 0);
                }
                break;
            case KEY_LEFT:
                // Si on est sur la colonne de la ligne on change de channel 
                if (seqNav.col == SEQUENCER_NAV_COL_LINE) {
                    // on change de channel
                    int newChannel = (seqNav.ch - 1) != -1 ? seqNav.ch-1 : SEQUENCER_NAV_CH_MAX - 1;
                    seqNav.start[newChannel] = seqNav.start[seqNav.ch]; // on switch de channel mais on garde la ligne à la même position
                    seqNav.ch = newChannel;
                    seqNav.col = SEQUENCER_NAV_COL_TIME; // on revient à la colonne de la durée
                    break;
                }
                // Sinon on change de colonne
                else if (seqNav.col > 0) seqNav.col--;

                break;
            case KEY_RIGHT:
                // Si on est sur la colonne de la durée on change de channel
                if (seqNav.col == SEQUENCER_NAV_COL_TIME) {
                    // on change de channel
                    int newChannel = (seqNav.ch + 1) % SEQUENCER_NAV_CH_MAX;
                    seqNav.start[newChannel] = seqNav.start[seqNav.ch]; // on switch de channel mais on garde la ligne à la même position
                    seqNav.ch = newChannel;
                    seqNav.col = SEQUENCER_NAV_COL_LINE; // on revient à la colonne de la ligne
                    break;
                }
                if (seqNav.col < SEQUENCER_NAV_COL_MAX - 1) seqNav.col++;
                break;
        }
        // On rafraichit les fenêtres
        show_sequencer_info(seqInfo, music, 0);
        print_sequencer_lines(channelWin[0], 0, music, &seqNav);
        print_sequencer_lines(channelWin[1], 1, music, &seqNav);
        print_sequencer_lines(channelWin[2], 2, music, &seqNav);
        mvwprintw(seqBody, 0, 1, "%d, %d, %d %d", seqNav.start[0], seqNav.start[1], seqNav.start[2], seqNav.line);

    }
}

/**
 * \fn void create_sequencer_nav()
 * \brief Création de la structure de navigation dans le séquenceur
 * \return La structure de navigation
*/
sequencer_nav_t create_sequencer_nav() {
    sequencer_nav_t nav;
    nav.col = SEQUENCER_NAV_COL_LINE;
    nav.ch = SEQUENCER_NAV_CH1;
    int i;
    for (i = 0; i < SEQUENCER_NAV_COL_MAX; i++) nav.start[i] = 0;
    nav.line = 0;
    return nav;
}

/**********************************************************************************************************************/
/*                                           Private Fonction Definitions                                             */
/**********************************************************************************************************************/

/**
 * \fn show_sequencer_info(WINDOW *win, int mode) 
 * \brief Affichage des informations du séquenceur
 * \details Cette fonction affiche les informations du séquenceur
 * \param win La fenêtre où afficher les informations
 * \param music La musique à afficher
 * \param mode Le mode des boutons (0 pour le mode NAVIGATION, 1 pour le mode EDITION)
 */
void show_sequencer_info(WINDOW *win, music_t *music, int mode) {
    // On efface la fenêtre
    werase(win);
    // On crée les bordures
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "%s", "INFO");
    // On affiche les informations
    mvwprintw(win, 1, 1, "Created : %ld", music->date.tv_sec);
    mvwprintw(win, 2, 1, "BPM : %d", music->bpm);
    mvwprintw(win, 3, 1, "Mode : %s", mode == 0 ? "NAVIGATION" : "EDITION");
    mvwprintw(win, 4, 1, "[BTN1] Save        [BTN2] Play       [BTN3] Quit ");
    wrefresh(win);
}

/**
 * \fn show_sequencer_help(WINDOW *win)
 * \brief Affichage de l'aide du séquenceur
 * \details Cette fonction affiche l'aide du séquenceur
 * \param win La fenêtre où afficher l'aide
 */
void show_sequencer_help(WINDOW *win) {
    // On efface la fenêtre
    werase(win);
    // On crée les bordures
    box(win, 0, 0);
    mvwprintw(win, 0, 1, "%s", "HELP");
    // Affichage des contrôles (TODO : eventuellement créer des macros pour les positions)
    mvwprintw(win, 1, 1, "%s", " / : Change note/octave/instrument/shift");
    mvwaddch(win, 1, 1, ACS_DARROW);
    mvwaddch(win, 1, 3, ACS_UARROW);

    mvwprintw(win, 2, 1, "%s", " / : Change column in the channel");
    mvwaddch(win, 2, 1, ACS_LARROW);
    mvwaddch(win, 2, 3, ACS_RARROW);

    mvwprintw(win, 3, 1, "%s", "[BTN4] : Change button mode");
    // On rafraichit la fenêtre
    wrefresh(win);
}

/**
 * \fn init_sequencer_channels(WINDOW *ch1, WINDOW *ch2, WINDOW *ch3, music_t *music)
 * \brief Initialisation des channels du séquenceur
 * \details Cette fonction initialise les channels du séquenceur
 * \param ch1 La fenêtre du channel 1
 * \param ch2 La fenêtre du channel 2
 * \param ch3 La fenêtre du channel 3
 * \param music La musique à afficher
*/
void init_sequencer_channels(WINDOW *ch1, WINDOW *ch2, WINDOW *ch3, music_t *music) {
    sequencer_nav_t seqNav = create_sequencer_nav();
    // On efface les fenêtres
    werase(ch1);
    werase(ch2);
    werase(ch3);
    // On crée les bordures
    box(ch1, 0, 0);
    box(ch2, 0, 0);
    box(ch3, 0, 0);
    // On affiche les entêtes
    mvwprintw(ch1, 0, 1, "%s", "CHANNEL 1");
    mvwprintw(ch2, 0, 1, "%s", "CHANNEL 2");
    mvwprintw(ch3, 0, 1, "%s", "CHANNEL 3");
    // On affiche l'entête d'information
    mvwprintw(ch1, 1, 1, "%s", "LINE|NOTE|OCTA|INST|SHFT");
    mvwprintw(ch2, 1, 1, "%s", "LINE|NOTE|OCTA|INST|SHFT");
    mvwprintw(ch3, 1, 1, "%s", "LINE|NOTE|OCTA|INST|SHFT");
    // On affiche les informations
    print_sequencer_lines(ch1, 0, music, &seqNav);
    print_sequencer_lines(ch2, 1, music, &seqNav);
    print_sequencer_lines(ch3, 2, music, &seqNav);
    // On rafraichit les fenêtres
    wrefresh(ch1);
    wrefresh(ch2);
    wrefresh(ch3);
}

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
    init_pair(COLOR_PAIR_SEQ_OCTAVE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_NOTE, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_INSTRUMENT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_PAIR_SEQ_SHIFT, COLOR_CYAN, COLOR_BLACK);
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
    int i;
    int curr = highlight; // le choix actuel
    init_menu(title, text, 0); // Initialisation du menu

    // On affiche les choix
    for (i = 0; i < nbChoices; i++) {
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
        for (i = 0; i < nbChoices; i++) {
            if (i == curr) attron(A_REVERSE);
            mvprintw(5 + i, 8, "%s", choices[i]);
            attroff(A_REVERSE);
        }
    }
    refresh();
}


/**
 * \fn void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav)
 * \brief Affichage des notes du séquenceur
 * \details Cette fonction affiche les notes du séquenceur
 * \param win La fenêtre où afficher les notes
 * \param channelId L'identifiant du channel
 * \param seqNav La structure de navigation dans le séquenceur
 * \param music La musique à afficher
 * \see sequencer_nav_t
 */
void print_sequencer_lines(WINDOW *win, short channelId, music_t *music, sequencer_nav_t *seqNav) {
    // On affiche les informations
    int i;
    for(i = 0; i < SEQUENCER_CH_LINES - 3; i++) {
        note_t note = music->channels[channelId].notes[seqNav->start[channelId] + i];
        int isSelected = 0;
        if(seqNav->ch == channelId && seqNav->line == seqNav->start[channelId]+i) isSelected = 1;
        print_sequencer_note(win, note, channelId, i, seqNav, isSelected);
    }
    // On rafraichit la fenêtre
    wrefresh(win);
}

/**
 * \fn void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isCurrLine)
 * \brief Affichage d'une ligne de notes du séquenceur
 * \details Cette fonction affiche une ligne de notes du séquenceur pour un unique channel
 * \param win La fenêtre où afficher les notes
 * \param note La note à afficher
 * \param line La ligne où afficher la note
 * \param seqNav La structure de navigation dans le séquenceur afin de pouvoir afficher un curseur
 * \param isSelected Si la ligne est la ligne selectionnée
 */
void print_sequencer_note(WINDOW *win, note_t note, short ch, int line, sequencer_nav_t *seqNav, int isSelected) {
    //TODO : Rajouter position du curseur pour reverse la colonne sélectionnée sur la ligne
    //TODO : Rajouter si la ligne est jouée alors toute la ligne est inversée
    char instrumentName[5]; // Nom de l'instrument
    char noteName[3]; // Nom de la note
    note2str(note, noteName); // On récupère le nom de la note
    instrument2str(note.instrument, instrumentName); // On récupère le nom de l'instrument
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_LINE, isSelected));
        mvwprintw(win, 2+line, 1, "%04X", seqNav->start[ch] + line);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_REVERSE);
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    mvwprintw(win, 2+line, 5, "|");
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ) | A_REVERSE);
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_NOTE, isSelected));
    mvwprintw(win, 2+line, 6, " %-2s ", noteName);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_NOTE) | A_REVERSE);
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    mvwprintw(win, 2+line, 10, "|");
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_OCTAVE, isSelected));
    mvwprintw(win, 2+line, 11, " %02d ", note.octave);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_OCTAVE) | A_REVERSE);
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    mvwprintw(win, 2+line, 15, "|");
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_INSTRUMENT, isSelected));
    mvwprintw(win, 2+line, 16, "%s ", instrumentName);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_INSTRUMENT) |A_REVERSE);
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    mvwprintw(win, 2+line, 20, "|");
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ));
    
    wattron(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | REVERSE_IF_COL(seqNav->col, SEQUENCER_NAV_COL_TIME, isSelected));
    mvwprintw(win, 2+line, 21, " %02d ", note.time);
    wattroff(win, COLOR_PAIR(COLOR_PAIR_SEQ_SHIFT) | A_REVERSE);
}

/**
 * \fn void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp)
 * \brief Modification d'une note du séquenceur en fonction de la colonne actuel et de la direction de la modification
 * \param note La note à modifier
 * \param col La colonne actuelle
 * \param scale La gamme des notes
 * \param isUp La direction de la modification (0 pour le bas, 1 pour le haut)
 * \see scale_t
*/
void change_sequencer_note(note_t *note, short col, scale_t scale, int isUp) {
    switch(col) {
        case SEQUENCER_NAV_COL_NOTE:
            if (isUp) {
                get_next_note(note, &scale);
                // on assigne un instrument par défaut si l'utilisateur n'a pas choisi
                if (note->instrument == INSTRUMENT_NA) note->instrument = INSTRUMENT_SIN;
            }
            else { 
                get_previous_note(note, &scale);
            }
            break;
        case SEQUENCER_NAV_COL_OCTAVE:
            if (isUp) note->octave = note->octave + 1 > 8 ? 8 : note->octave + 1;
            else note->octave = note->octave - 1 < 0 ? 8 : note->octave - 1;
            break;
        case SEQUENCER_NAV_COL_INSTRUMENT:
            if (isUp) note->instrument = note->instrument + 1 == INSTRUMENT_NB ? 0 : note->instrument + 1;
            else note->instrument = (note->instrument - 1) == -1 ? INSTRUMENT_NB - 1 : note->instrument - 1;
            break;
        case SEQUENCER_NAV_COL_TIME:
            if (isUp) {
                note->time += ( (note->time == TIME_CROCHE_DOUBLE) ? 1 : 2);
                if (note->time >= TIME_END) note->time = TIME_CROCHE_DOUBLE;
            }
            else {
                note->time -=(note->time == TIME_CROCHE ? 1 : 2);
                if (note->time == -1) note->time = TIME_RONDE;
            }
            break;
    }
}