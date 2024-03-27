/**
 * \file pimusiic.c
 * \details Application principale
*/
#include "wiringseq.h"
#include "mysyscall.h"
sem_t * sem_login;
sem_t * sem_tag;
void clean_up() {
    // On arrête la bibliothèque graphique
	unlink_named_sem("SEM_LOGIN");
	unlink_named_sem("SEM_TAG");
    exit(EXIT_SUCCESS);
}

int main() {
	sem_unlink("SEM_LOGIN");
	sem_unlink("SEM_TAG");
	atexit(clean_up);
	install_signal_handler(SIGINT, clean_up, 0);
	sem_login = create_named_sem("SEM_LOGIN", 0);
	sem_tag =  create_named_sem("SEM_TAG", 0);
	
	
	while(1){
		fprintf(stderr,"On attend la ressource\n");
		wait_sem(sem_login);
		fprintf(stderr,"On prend la sémaphore\n");
    	char rfid[20] = "";
    	init_wiringpi();
   		read_rfid(rfid);
   		FILE *fichier;
    	fichier = fopen("ressources/tag", "w"); // Ouvre le fichier en mode écriture ("w")

    	if (fichier != NULL) { // Vérifie si le fichier a été ouvert avec succès
        	// Écrit la valeur RFID dans le fichier
        	fprintf(fichier, "%s\n", rfid);
        
        	// Ferme le fichier
        	fclose(fichier);
    	} else {
        	// En cas d'erreur lors de l'ouverture du fichier
        	printf("Erreur lors de l'ouverture du fichier.\n"); // Quitte le programme avec un code d'erreur
    	}
    	fprintf(stderr,"On écrit le tag et on post\n");
    	post_sem(sem_tag);
    }

}


