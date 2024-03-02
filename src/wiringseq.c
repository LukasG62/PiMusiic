/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "wiringseq.h"
/* ------------------------------------------------------------------------ */
/*            P R O T O T Y P E S    D E    F O N C T I O N S               */
/* ------------------------------------------------------------------------ */
/**
 * \fn is_button_pressed(buttons_keymap_t button);
 * \brief tester si le bouton est pressé dans la matrice
 * \param button le bouton a tester
 */
int is_button_pressed_matrix(buttons_keymap_t button);
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
	wiringPiSPISetup (RFID_CS, 13560000);
	
	//on initialise I2C ( nb de device ) 
	 wiringPiI2CSetup (1) ;
	 
	//on configure les boutons
	
	pinMode (BUTTON_UP, INPUT);
	pinMode (BUTTON_DOWN, INPUT);
	pinMode (BUTTON_LEFT, INPUT);
	pinMode (BUTTON_RIGHT, INPUT);
	pinMode (BUTTON_COL4, OUTPUT);
	pinMode (BUTTON_COL3, OUTPUT);
	pinMode (BUTTON_ROW2, INPUT);
	pinMode (BUTTON_ROW1, INPUT);
	
	
	//on configure les step du SM
	pinMode (SM_STEP1, OUTPUT);
	pinMode (SM_STEP2, OUTPUT);
	pinMode (SM_STEP3, OUTPUT);
	pinMode (SM_STEP4, OUTPUT);
	
	
	//on configure le 7 segment_t
	pinMode (SEVEN_SEGMENT_SDA, OUTPUT);
	pinMode (SEVEN_SEGMENT_SCL, GPIO_CLOCK);
	
	
	//on configure le rfid
	pinMode (RFID_MOSI, OUTPUT);
	pinMode(RFID_MISO,INPUT);
	pinMode(RFID_SCLK,GPIO_CLOCK);
	pinMode( RFID_CS,OUTPUT);
	
	//on configure le capteur de proximité
	pinMode (ULTRASONIC_TRIG, OUTPUT);
	pinMode(ULTRASONIC_ECHO,INPUT);
	

}

/**
 * \fn is_button_pressed(buttons_keymap_t button);
 * \brief tester si le bouton est pressé
 * \param button le bouton a tester
 */
int is_button_pressed(buttons_keymap_t button){

	if(button>=90)
		return is_button_pressed_matrix(button);
	else{	
		int level = digitalRead(button);
		return (level==HIGH);
	}
}
/**
 * \fn is_button_pressed(buttons_keymap_t button);
 * \brief tester si le bouton est pressé dans la matrice
 * \param button le bouton a tester
 */
int is_button_pressed_matrix(buttons_keymap_t button){
	int level=0;
	switch(button)
	{
		case BUTTON_CH1NSAVE:
			digitalWrite(BUTTON_COL3,HIGH);
			level = is_button_pressed(BUTTON_ROW1);
			digitalWrite(BUTTON_COL3,LOW);
			
		break;
		case BUTTON_CH2NQUIT:
			digitalWrite(BUTTON_COL3,HIGH);
			level = is_button_pressed(BUTTON_ROW2);
			digitalWrite(BUTTON_COL3,LOW);
			
		break;
		case BUTTON_CH3NPlay :
			digitalWrite(BUTTON_COL4,HIGH);
			level = is_button_pressed(BUTTON_ROW1);
			digitalWrite(BUTTON_COL4,LOW);
			
		break;
		case BUTTON_CHANGEMODE:
			digitalWrite(BUTTON_COL4,HIGH);
			level = is_button_pressed(BUTTON_ROW2);
			digitalWrite(BUTTON_COL4,LOW);
			
		break;
		
		default:
			printf("Le bouton %d n'existe pas !",button); 
			
	}
	return level;

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
	pwmWrite(captProx,0);
	
	//on lit la valeur en retour
	captProx = ULTRASONIC_ECHO;
	int distance = round(analogRead(captProx)*17165/1000000);
}


