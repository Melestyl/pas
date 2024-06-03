#include "user.h"

// Global list of areas
node_t *area_list = NULL;

int main(int argc, char *argv[]) {
    char is_admin = 0; // Remind if the user is an admin

    // Check if the user is admin
    if (argc == 2 && strcmp(argv[1], ADMIN_PASSWORD) == 0) {
        printf("Vous êtes un administrateur!\n");
        is_admin = 1;
    }

    while (1)
        menu(is_admin);
}

void menu(char is_admin) {
    int choice = -1; // User choice
    printf("\nQue voulez-vous faire ?\n"
            "\t0. Quitter\n"
            "\t1. Afficher la liste des emplacement (Bureaux et salles de réunion)\n"
            "\t2. Réserver un emplacement\n"
            "\t3. Rendre un emplacement\n"
            "\t4. Voir mes réservations\n");
    
    if(is_admin) {
            printf("\t5. Ajouter un emplacement\n"
            "\t6. Supprimer un emplacement\n");
    }

    printf("\nVotre choix : ");
    scanf("%d", &choice);
    switch (choice) {
        case 0:
            printf("Au revoir!\n");
            end(); //TODO: Verify if there is any reservation area and anything 
        case 1:
            show_areas();
            break;
        case 2:
            book_area();
            break;
        case 3:
            return_area();
            break;
        case 4:
            show_my_bookings();
            break;
        case 5:
            if(is_admin)
                add_area();
            else
                printf("Commande inconnue\n");
            break;
        case 6:
            if(is_admin)
                delete_area();
            else
                printf("Commande inconnue\n");
            break;
        default:
            printf("Commande inconnue\n");
            break;
    }
}

void show_areas() {
	node_t * temp = area_list;
	area_t * area;

	printf("Liste des emplacements :\n")

	while(temp != NULL) {
		// Attaching shared memory
		area = (area_t *) shmat(temp->data, NULL, 0);
		if(area == (area_t *)-1) {
			perror("shmat");
			exit(1);
		}

		// Printing infos
		printf("Nom : %s\n", area->name);
		area->type == DESK ? printf("Type : Bureau\n") : printf("Type : Salle de réunion\n");
		area->shared_memory == 0 ? printf("Statut : Libre\n") : printf("Statut : Réservé\n");

		// Detaching shared memory and moving to the next node
		if(shmdt(area) == -1) {
			perror("shmdt");
			exit(1);
		}
		temp = temp->next;
	}
}

void book_area() {
    printf("Réserver un earea\n");
}

void return_area() {
    printf("Rendre un earea\n");
}

void show_my_bookings() {
    printf("Voir mes réservations\n");
}

void add_area() {
    printf("Ajouter un earea\n");
}

void delete_area() {
    printf("Supprimer un earea\n");
}

void end() {
    exit(0);
}
