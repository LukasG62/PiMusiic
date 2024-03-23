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
#include <sys/time.h>
#include <stdint.h>

/* ------------------------------------------------------------------------ */
/*              C O N S T A N T E S     S Y M B O L I Q U E S               */
/* ------------------------------------------------------------------------ */
#define BUTTON_ROW {BUTTON_ROW1,BUTTON_ROW2,BUTTON_ROW3,BUTTON_ROW4}
#define BUTTON_COL {BUTTON_COL2,BUTTON_COL3,BUTTON_COL4}

#define RFID_SPEED 1000L /*!< Vitesse de communication avec le lecteur RFID */
#define RFID_GPIO 22 /*!< GPIO du lecteur RFID */
#define MICRO2SEC 1000000L /*!< Conversion de microsecondes en secondes */

#define SEVEN_SEGMENT_DIGITS {SEVEN_SEGMENT_0,SEVEN_SEGMENT_1,SEVEN_SEGMENT_2,SEVEN_SEGMENT_3,SEVEN_SEGMENT_4,SEVEN_SEGMENT_5,SEVEN_SEGMENT_6,SEVEN_SEGMENT_7,SEVEN_SEGMENT_8,SEVEN_SEGMENT_9}
#define	SEVEN_SEGMENT_SDA 8 /*!< Valeur de la pin pour communiquer avec le 7 segments*/ 
#define SEVEN_SEGMENT_SCL 9/*!<Valeur de la pin de clock pour l’i2c*/
#define SEVEN_SEGMENT_ADDR 0x70 /*!<Adresse I2C du 7 segments*/

#define SEVEN_SEGMENT_D1_ADDR 0x8 /*!<Registre pour le chiffre des unités*/
#define SEVEN_SEGMENT_D2_ADDR 0x6 /*!<Registre pour le chiffre des dizaines*/
#define SEVEN_SEGMENT_D3_ADDR 0x2 /*!<Registre pour le chiffre des centaines*/
#define SEVEN_SEGMENT_D4_ADDR 0x0 /*!<Registre pour le chiffre des milliers*/
#define SEVEN_SEGMENT_DOT_ADDR 0x4 /*!<Registre pour les deux points*/



/* ------------------------------------------------------------------------ */
/*                    M A C R O    F O N C T I O N S                        */
/* ------------------------------------------------------------------------ */
#define IS_BUTTON_PRESSED(bitmap, mask) (bitmap) & (mask)

/* ------------------------------------------------------------------------ */
/*              D É F I N I T I O N S   D E   T Y P E S                     */
/* ------------------------------------------------------------------------ */

/**
 * @enum buttons_bitmap_t
 * @brief Enumeration des boutons
 * 
 */
typedef enum {
	
	BUTTON_UP= (1 << 0),/*!<Valeur WP_Pin pour la flèche directionnelle haut */ //1
	BUTTON_DOWN = (1 << 1),/*!< Valeur WP_Pin pour la flèche directionnelle bas*/ //2
	BUTTON_LEFT =(1 << 2), /*!< Valeur WP_Pin pour la flèche directionnelle gauche*/ //3
	BUTTON_RIGHT = (1 << 3),/*!< Valeur WP_Pin pour la flèche directionnelle droite*/ //4
	BUTTON_CH1NSAVE = (1 << 4),/*!< Valeur button pour CH1 ou pour sauvegarder*/ //5
	BUTTON_CH2NQUIT = (1 << 5),/*!< Valeur button pour CH2 ou pour quitter*/ //6
	BUTTON_CH3NPLAY = (1 << 6),/*!< Valeur button pour CH3 ou pour jouer*/ //7
	BUTTON_CHANGEMODE = (1 << 7), /*!< Valeur WP_Pin pour le bouton de changement de mode  */ //8
	
	BUTTON_ROW1 = 2, /*!< Valeur WP_Pin pour la première rangée de boutons*/
	BUTTON_ROW2 = 3, /*!< Valeur WP_Pin pour la deuxième rangée de boutons*/
	BUTTON_ROW3 = 21, /*!< Valeur WP_Pin pour la troisième rangée de boutons*/
	BUTTON_ROW4 = 22, /*!< Valeur WP_Pin pour la quatrième rangée de boutons*/
	
	BUTTON_COL2 = 25, /*!< Valeur WP_Pin pour la deuxième colonne de boutons*/
	BUTTON_COL3 = 24, /*!< Valeur WP_Pin pour la troisième colonne de boutons*/
	BUTTON_COL4 = 23, /*!< Valeur WP_Pin pour la quatrième colonne de boutons*/
	
}buttons_bitmap_t;

/**
 * @enum seven_segment_t
 * @brief Valeur des pins pour le 7 segments
 */



/**
 * @enum seven_segment_digit_t
*/
typedef enum {
	SEVEN_SEGMENT_0 = 0x3F, /*!< Valeur à écrire pour afficher le chiffre 0*/
	SEVEN_SEGMENT_1 = 0x06, /*!< Valeur à écrire pour afficher le chiffre 1*/
	SEVEN_SEGMENT_2 = 0x5B, /*!< Valeur à écrire pour afficher le chiffre 2*/
	SEVEN_SEGMENT_3 = 0x4F, /*!< Valeur à écrire pour afficher le chiffre 3*/
	SEVEN_SEGMENT_4 = 0x66, /*!< Valeur à écrire pour afficher le chiffre 4*/
	SEVEN_SEGMENT_5 = 0x6D, /*!< Valeur à écrire pour afficher le chiffre 5*/
	SEVEN_SEGMENT_6 = 0x7D, /*!< Valeur à écrire pour afficher le chiffre 6*/
	SEVEN_SEGMENT_7 = 0x07, /*!< Valeur à écrire pour afficher le chiffre 7*/
	SEVEN_SEGMENT_8 = 0x7F, /*!< Valeur à écrire pour afficher le chiffre 8*/
	SEVEN_SEGMENT_9 = 0x6F, /*!< Valeur à écrire pour afficher le chiffre 9*/
} seven_segment_digit_t;

/**
 * \enum rfid_reader_t
 * \brief pin du lecteur rfid
 */
typedef enum {
	RFID_MOSI = 12, /*!< Master Out Slave In*/ 
	RFID_CS=10,/*!<Device state*/ 
	RFID_MISO=13,/*!<Master In Slave Out */
	RFID_SCLK = 14,/*<CLOCK FOR SPI */                                                                                                                                                                                                                    
}rfid_reader_t;

/**
 * \enum sensor_proximity_t
 * \brief pin du capteur de proximité
 */
typedef enum {
	ULTRASONIC_TRIG = 27,/*<pin sur laquelle il faut envoyer une impulsion de 5v pendant au moins 10µs */
	ULTRASONIC_ECHO = 26, /*<	Retourne une impulsion proportionelle à la distance de l’obstacle*/
}sensor_proximity_t;

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
unsigned char is_button_pressed();

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
 * @fn int read_proximity_sensor()
 * @brief Lit la distance mesurée par le capteur de proximité
 * @return int La distance mesurée en centimètres
 */
int read_proximity_sensor();

/**
 * @fn void init_rfid()
 * @brief Fonction d'initialisation du RFID
 * Cette fonction a été repris d'un groupe précédent
 * GRP1_ATOME (2023) : Urban Prevost & Antoine Jedrezak
 */
void init_rfid();

#endif
