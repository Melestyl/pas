#include "user.h"

int main(int argc, char *argv[]) {
    char is_admin = 0; // Remind if the user is an admin

    // Verify that the user is an admin
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
            "\t1. Afficher la liste des espaces (Bureaux et salles de réunion)\n"
            "\t2. Réserver un espace\n"
            "\t3. Rendre un espace\n"
            "\t4. Voir mes réservations\n");
    
    if(is_admin) {
        printf("\t5. Ajouter un espace\n"
                "\t6. Supprimer un espace\n");
    }

    printf("\nVotre choix : \n");
    scanf("%d", &choice);
    switch (choice) {
        case 0:
            printf("Au revoir!\n");
            end(); //TODO: Verify if there is any reservation area and anything 
        case 1:
            show_spaces();
            break;
        case 2:
            book_space();
            break;
        case 3:
            return_space();
            break;
        case 4:
            show_my_bookings();
            break;
        case 5:
            if(is_admin)
                add_space();
            else
                printf("Commande inconnue\n");
            break;
        case 6:
            if(is_admin)
                delete_space();
            else
                printf("Commande inconnue\n");
            break;
        default:
            printf("Commande inconnue\n");
            break;
    }
}