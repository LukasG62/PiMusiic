/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "wiringseq.h"
/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*                  C O D E    D E S    F O N C T I O N S                   */
/* ------------------------------------------------------------------------ */

/**
 * \fn init_wiringpi();
 * \brief initialiser tout les ports gpio de joy pi
 * \param
 */
void init_wiringpi(){
	wiringPiSetup();
	
	//on initialise le SPI ( channel , frequency ) 
	rfid_reader_t rfid = RFID_CS;
	wiringPiSPISetup (rfid, 13560000);
	
	//on initialise I2C ( nb de device ) 
	 wiringPiI2CSetup (1) ;
	 
	//on configure les boutons
	buttons_keymap_t buttons = BUTTON_CHANGEMODE;
	pinMode (buttons, INPUT);
	buttons = BUTTON_UP;
	pinMode (buttons, INPUT);
	buttons = BUTTON_DOWN;
	pinMode (buttons, INPUT);
	buttons = BUTTON_LEFT;
	pinMode (buttons, INPUT);
	buttons = BUTTON_RIGHT;
	pinMode (buttons, INPUT);
	buttons = BUTTON_CH1NSAVE;
	pinMode (buttons, INPUT);
	buttons = BUTTON_CH2NQUIT;
	pinMode (buttons, INPUT);
	buttons =BUTTON_CH3NPlay;
	pinMode (buttons, INPUT);
	
	
	//on configure les step du SM
	stepper_motor_pins_t sm = SM_STEP1;
	pinMode (sm, OUTPUT);
	sm=SM_STEP2;
	pinMode (sm, OUTPUT);
	sm=SM_STEP3;
	pinMode (sm, OUTPUT);
	sm=SM_STEP4;
	pinMode (sm, OUTPUT);
	
	
	//on configure le 7 segment_t
	seven_segment_t sevenSeg = SEVEN_SEGMENT_SDA;
	pinMode (sevenSeg, OUTPUT);
	sevenSeg=SEVEN_SEGMENT_SCL;
	pinMode (sevenSeg, GPIO_CLOCK);
	
	
	//on configure le rfid
	rfid = RFID_MOSI;
	pinMode (rfid, OUTPUT);
	rfid = RFID_MISO;
	pinMode(rfid,INPUT);
	rfid = RFID_SCLK;
	pinMode(rfid,GPIO_CLOCK);
	rfid = RFID_CS;
	pinMode(rfid,OUTPUT);
	
	//on configure le capteur de proximité
	captor_proximity_t captProx = ULTRASONIC_TRIG;
	pinMode (captProx, OUTPUT);
	captProx= ULTRASONIC_ECHO;
	pinMode(captProx,INPUT);
	

}

/**
 * \fn is_button_pressed(buttons_keymap_t button);
 * \brief tester si le bouton est pressé
 * \param button le bouton a tester
 */
int is_button_pressed(buttons_keymap_t button){
	int level = digitalRead(button);
	return (level==HIGH);
}

/**
 * \fn  sm_play_note(stepper_motor_pins_t step);
 * \brief jouer une note sur le step motor
 * \param delay delai pour changer de pas ( faire x 4 pour un tour )
 */
void sm_play_note(int delay){
	stepper_motor_pins_t in1 = SM_STEP1;
	stepper_motor_pins_t in2 = SM_STEP2;
	stepper_motor_pins_t in3 = SM_STEP3;
	stepper_motor_pins_t in4 = SM_STEP4;

	digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    digitalWrite(in3, LOW); 
    digitalWrite(in4, HIGH);
    sleep(delay);

    digitalWrite(in1, HIGH); 
    digitalWrite(in2, HIGH); 
    digitalWrite(in3, LOW); 
    digitalWrite(in4, LOW);
    sleep(delay);

    digitalWrite(in1, LOW); 
    digitalWrite(in2, HIGH); 
    digitalWrite(in3, HIGH); 
    digitalWrite(in4, LOW);
    sleep(delay);

    digitalWrite(in1, LOW); 
    digitalWrite(in2, LOW); 
    digitalWrite(in3, HIGH); 
    digitalWrite(in4, HIGH);
    sleep(delay);
}

/**
 * \fn display_bpm(int bpm);
 * \brief afficher le bpm sur le 7 segments
 * \param bpm chiffre à afficher
 */
void display_bpm(int bpm){
	int address = 0x70;
	wiringPiI2CWrite(address,0);
	wiringPiI2CWrite(address,bpm);
}

/**
 * \fn read_rfid();
 * \brief lis la valeur du badge rfid
 */
char * read_rfid(char * tagRfid){
 // aucune idée ? j'ai pas trouvé de fonction pour lire en SPI ?
}

/**
 * \fn read_proximity_captor();
 * \brief  lis la distance mesurée par le capteur
 */
int read_proximity_captor(){
	
	//on envoie un pwm 
	captor_proximity_t captProx = ULTRASONIC_TRIG;
	pwmWrite(captProx,660); // rapport cyclique conseillé  : 0.67 environ
	
	//on lit la valeur en retour
	captProx = ULTRASONIC_ECHO;
	int distance = round(analogRead(captProx)*17165/1000000);
}


