/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "wiringseq.h"
#include "rfid/rfid.h"
#include "rfid/rc522.h"
#include "rfid/config.h"
#include "rfid/main.h"

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
	
	//on configure le capteur de proximité
	pinMode (ULTRASONIC_TRIG, OUTPUT);
	pinMode(ULTRASONIC_ECHO,INPUT);
	
	int i;
	int button_row[] = BUTTON_ROW;
	
	for(i=0;i<4;i++){
		pinMode(button_row[i],INPUT);
		pullUpDnControl (button_row[i], PUD_UP) ;
	}
	
	int button_col[] = BUTTON_COL;
	for(i=0;i<3;i++){
		pinMode(button_col[i],OUTPUT);
		digitalWrite(button_col[i],HIGH);
	}

	//init_rfid();
	
}

/**
 * \fn is_button_pressed(buttons_keymap_t button);
 * \brief tester si le bouton est pressé
 * \param button le bouton a tester
 */
unsigned char is_button_pressed(){
	unsigned char bitmap = 0;
	int level = 0;
	
	//col2
	digitalWrite(BUTTON_COL2, LOW);
	//button left
	level=!digitalRead(BUTTON_ROW2);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_LEFT;
	}
	//button play
	level=!digitalRead(BUTTON_ROW4);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_CH1NSAVE;
	}
	
	digitalWrite(BUTTON_COL2,HIGH);
	//col3
	digitalWrite(BUTTON_COL3, LOW);
	//button up
	level=!digitalRead(BUTTON_ROW1);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_UP;
	}
	//button changemode
	level=!digitalRead(BUTTON_ROW2);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_CHANGEMODE;
	}
	//button down
	level=!digitalRead(BUTTON_ROW3);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_DOWN;
	}
	//button save
	level=!digitalRead(BUTTON_ROW4);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_CH2NQUIT;
	}
	digitalWrite(BUTTON_COL3,HIGH);
	//col4
	digitalWrite(BUTTON_COL4,LOW);
	//button right
	level=!digitalRead(BUTTON_ROW2);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_RIGHT;
	}
	//button quit
	level=!digitalRead(BUTTON_ROW4);
	if(level == HIGH){
		bitmap = bitmap | BUTTON_CH3NPLAY;
	}
	digitalWrite(BUTTON_COL4,HIGH);

	return bitmap;
}

/**
 * \fn display_bpm(int bpm);
 * \brief afficher le bpm sur le 7 segments
 * \param bpm chiffre à afficher
 */
void display_bpm(int bpm){
	int digits [] = SEVEN_SEGMENT_DIGITS;

	//on initialise I2C
	int fd = wiringPiI2CSetup (SEVEN_SEGMENT_ADDR) ;
	//on allume l'horloge
	wiringPiI2CWriteReg16(fd,0x2,0x01);
	//on configure int/row
	wiringPiI2CWriteReg16(fd,0xA,0x00);
	//on allume l'écran 
	wiringPiI2CWriteReg16(fd,0x8,0x01);
	//on configure l'intensité
	wiringPiI2CWriteReg16(fd,0xE,0x01);
	//X---
	wiringPiI2CWriteReg16(fd,SEVEN_SEGMENT_D4_ADDR,(digits[bpm/1000]));//premier digit
	bpm = bpm%1000;
	//-X--
	wiringPiI2CWriteReg16(fd,SEVEN_SEGMENT_D3_ADDR,(digits[bpm/100]));//deuxième digit
	bpm =bpm%100;

	wiringPiI2CWriteReg16(fd,SEVEN_SEGMENT_DOT_ADDR,0x00);
	//--X-
	wiringPiI2CWriteReg16(fd,SEVEN_SEGMENT_D2_ADDR,(digits[bpm/10]));//troisème digit
	bpm =bpm%10;
	//---X
	wiringPiI2CWriteReg16(fd,SEVEN_SEGMENT_D1_ADDR,(digits[bpm/1]));
	close(fd);
}

/**
 * \fn read_rfid();
 * \brief lis la valeur du badge rfid
 */
char * read_rfid(char * tagRfid){
	int 	tmp;
	char    *p, sn_str[23];         // to hold [serial] as a string
	init_rfid();
	
	// wait for card
	while (get_card_info() != TAG_OK) usleep(5000);
    p=sn_str;
    for (tmp=0;tmp<SN_len;tmp++) {  // add serial number
		sprintf(p,"%02x",SN[tmp]);
		p+=2;
	}
    strcpy(tagRfid,sn_str);

	//close_out(0);
	return tagRfid;
	
}


/**
 * @fn int read_proximity_sensor()
 * @brief Lit la distance mesurée par le capteur de proximité
 * @return int si un obstacle est détecté
 */
int read_proximity_sensor(){
	
	struct timeval dateDebut;
	struct timeval dateFin;
	
	digitalWrite(ULTRASONIC_TRIG, LOW);
	
	digitalWrite(ULTRASONIC_TRIG, HIGH);
	usleep(10);
	digitalWrite(ULTRASONIC_TRIG, LOW);
	while(digitalRead(ULTRASONIC_ECHO)==LOW){
		gettimeofday(&dateDebut, NULL);
	}
	while(digitalRead(ULTRASONIC_ECHO)==HIGH){
		gettimeofday(&dateFin, NULL);
	}
	
	uint64_t delta_us = (dateFin.tv_sec - dateDebut.tv_sec) * 1000000 + (dateFin.tv_usec - dateDebut.tv_usec) / 1000;
	int distance = round(delta_us*17150*100.0)/100.0;
	
	if( distance == 0 )
		return 1;
	else 
		return 0;
	
}

/**
 * @fn void init_rfid()
 * @brief Fonction d'initialisation du RFID
 * Cette fonction a été reprise d'un groupe précédent
 * GRP1_ATOME (2023) : Urban Prevost & Antoine Jedrezak
 */
void init_rfid() {
    // catch signals
    //set_signals();

    /* read /etc/rc522.conf */
    if (get_config_file()) exit(1);

    /* set BCM2835 Pins correct */
    if (HW_init(RFID_SPEED,RFID_GPIO)) close_out(1);

    /* initialise the RC522 */
    InitRc522();

    /* read & set GID and UID from config file */
    if (read_conf_uid()!= 0) close_out(1);
}



