/* ------------------------------------------------------------------------ */
/*                   E N T Ê T E S    S T A N D A R D S                     */
/* ------------------------------------------------------------------------ */
#include "wiringseq.h"
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
	
	/*
	//on configure le rfid
	pinMode (RFID_MOSI, OUTPUT);
	pinMode(RFID_MISO,INPUT);
	pinMode(RFID_SCLK,GPIO_CLOCK);
	pinMode( RFID_CS,OUTPUT);
	*/
	
	

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
	int digits [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

	
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
	
	
	//on écrit 2 à l'adresse 0
	wiringPiI2CWriteReg16(fd,0x0,(digits[bpm/1000]));//premier digit
	bpm = bpm%1000;
	
	wiringPiI2CWriteReg16(fd,0x2,(digits[bpm/100]));//deuxième digit
	bpm =bpm%100;
	
	//2points
	wiringPiI2CWriteReg16(fd,0x4,0x00);
	
	wiringPiI2CWriteReg16(fd,0x6,(digits[bpm/10]));//troisème digit
	bpm =bpm%10;
	
	wiringPiI2CWriteReg16(fd,0x8,(digits[bpm/1]));
	
	
	
	close(fd);
}

/**
 * \fn read_rfid();
 * \brief lis la valeur du badge rfid
 */
char * read_rfid(char * tagRfid){
	
	
	//on configure le rfid
	pinMode (RFID_MOSI, OUTPUT);
	pinMode(RFID_MISO,INPUT);
	//pinMode(RFID_SCLK,GPIO_CLOCK);
	//pinMode( RFID_CS,OUTPUT);
	
	//on initialise le SPI ( channel , frequency ) 
	//int fd = wiringPiSPISetup (RFID_CS, 13560000);
	
	//on met le champ CS à 1 
	//digitalWrite(RFID_CS, HIGH);
	 /*SPI_WRITE(((dev_cmd << 1) & 0x7e) | 0x80);
  SPI_FLUSH();
  SPI_READ(ret);*/
}

/**
 * \fn read_proximity_captor();
 * \brief  lis la distance mesurée par le capteur
 * \return 1 si proche 0 sinon
 */
int read_proximity_captor(){
	
	struct timeval dateDebut;
	struct timeval dateFin;
	
	digitalWrite(ULTRASONIC_TRIG, LOW);
	
	digitalWrite(ULTRASONIC_TRIG, HIGH);
	sleep(0.00001);
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
