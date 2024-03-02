/**
 * \file wiringseq.h
 * \details Couche d'abstraction de la librairie WiringPI
 * Permet de manipuler les périphiques connectées aux ports GPIO
*/
#ifndef WIRING_SEQ_H
#define WIRING_SEQ_H
/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include <wiringPi.h>
#include <math.h>
#include <unistd.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <stdio.h>
/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */



/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */
/**
 * \enum buttons_keymap_t
 * \brief enumeration des boutons
 *
 */
typedef enum {
	
	BUTTON_UP=37,/*!<Valeur WP_Pin pour la flèche directionnelle haut */ //TODO trouver la bonne valeur 
	BUTTON_DOWN = 1,/*!< Valeur WP_Pin pour la flèche directionnelle bas*/ //TODO trouver la bonne valeur 
	BUTTON_LEFT =15, /*!< Valeur WP_Pin pour la flèche directionnelle gauche*/
	BUTTON_RIGHT = 2,/*!< Valeur WP_Pin pour la flèche directionnelle droite*/ //TODO trouver la bonne valeur 
	BUTTON_CH1NSAVE = 90,/*!< Valeur button pour CH1 ou pour sauvegarder*/
	BUTTON_CH2NQUIT = 91,/*!< Valeur button pour CH2 ou pour quitter*/
	BUTTON_CH3NPlay = 92,/*!< Valeur button pour CH3 ou pour jouer*/
	BUTTON_CHANGEMODE = 93, /*!< Valeur WP_Pin pour le bouton de changement de mode  */
	BUTTON_ROW1 = 13,/*!< Valeur pin de la ligne 1 de la matrice de bouton*/
	BUTTON_ROW2 = 15,/*!< Valeur pin de la ligne 2 de la matrice de bouton*/
	BUTTON_COL3 = 37,/*!< Valeur pin de la colonne 3 de la matrice de bouton*/
	BUTTON_COL4 = 22,/*!< Valeur pin de la colonne 4 de la matrice de bouton*/
}buttons_keymap_t;


/**
 * \enum stepper_motor_pins_t
 * \brief step du step motor
 *
 */
typedef enum {
	SM_STEP1 = 29, /*!< Valeur WP_Pin pour le STEP1 */ 
	SM_STEP2=31,/*!<Valeur WP_Pin pour le STEP2*/ 
	SM_STEP3 = 33,/*!< Valeur WP_Pin pour le STEP3*/ 
	SM_STEP4 =35, /*!< Valeur WP_Pin pour le STEP4*/
}stepper_motor_pins_t;

/**
 * \enum seven_segment_t
 * \brief pin du 7 seg
 *
 */
typedef enum {
	SEVEN_SEGMENT_SDA = 3, /*!< Valeur de la pin pour communiquer avec le 7 segments*/ 
	SEVEN_SEGMENT_SCL=5,/*!<Valeur de la pin de clock pour l’i2c*/ 
}seven_segment_t;

/**
 * \enum rfid_reader_t
 * \brief pin du lecteur rfid
 *
 */
typedef enum {
	RFID_MOSI = 19, /*!< Master Out Slave In*/ 
	RFID_CS=24,/*!<Device state*/ 
	RFID_MISO=29,/*!<Master In Slave Out */
	RFID_SCLK = 28,/*<CLOCK FOR SPI */
                                                                                                                                                                                                                                                 
}rfid_reader_t;

/**
 * \enum captor_proximity_t
 * \brief pin du capteur de proximité
 *
 */
typedef enum {
	ULTRASONIC_TRIG = 36,/*<pin sur laquelle il faut envoyer une impulsion de 5v pendant au moins 10µs */
	ULTRASONIC_ECHO = 32, /*<	Retourne une impulsion proportionelle à la distance de l’obstacle*/
}captor_proximity_t;



/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */
/**
 * \fn init_wiringpi();
 * \brief initialiser tout les ports gpio de joy pi
 * \param
 */
void init_wiringpi();

/**
 * \fn is_button_pressed(buttons_keymap_t button);
 * \brief tester si le bouton est pressé
 * \param button le bouton a tester
 */
int is_button_pressed(buttons_keymap_t button);

/**
 * \fn  sm_play_note(stepper_motor_pins_t step);
 * \brief jouer une note sur le step motor
 * \param delay delai pour changer de pas ( faire x 4 pour un tour )
 */
void sm_play_note(int delay);

/**
 * \fn display_bpm(int bpm);
 * \brief afficher le bpm sur le 7 segments
 * \param bpm chiffre à afficher
 */
void display_bpm(int bpm);

/**
 * \fn read_rfid();
 * \brief lis la valeur du badge rfid
 */
char* read_rfid(char * tagRfid);

/**
 * \fn read_proximity_captor();
 * \brief  lis la distance mesurée par le capteur
 */
int read_proximity_captor();























#endif
