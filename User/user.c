#include "user.h"

// Global list of areas
node_t *area_list = NULL;
node_t *my_bookings = NULL;

// Global mailbox
int mailbox;

int main(int argc, char *argv[]) {
    char is_admin = 0; // Remind if the user is an admin
	key_t msg_key = MESSAGE_KEY; // key of the message queue
	int msg_flag = IPC_CREAT | 0666; // flag of the message queue

	// Creating message queue
	if ((mailbox = msgget(msg_key,msg_flag)) == -1) {
		perror("msgget");
		exit(1);
	}

	printf("Message queue created with id %d\n", mailbox);

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

/**
 * @brief Remove a node from the list
 * @param head: head of the list
 * @param name: name to remove
 * @param success: bool to indicate if the node was removed
 * @return new head of the list
 */
node_t * remove_node(node_t * head, char* name, bool* success) {
	node_t * temp = head;
	node_t * prev = NULL;
	area_t * area;


	while(temp != NULL) {
		// Attaching shared memory
		area = (area_t *) shmat(temp->data, NULL, 0) ;
		if( area == (area_t *)-1) {
			perror("shmat");
			*success = false;
		}

		// Comparing names of areas and if found, detaching shared memory and returning shared memory segment
		if(strcmp(area->name,name)==0) {
			if(prev == NULL) {
				head = temp->next;
				free(temp);
				*success = true;
				return head;
			}
			else if(prev == NULL && temp->next == NULL){
				free(temp);
				*success = true;
				return NULL;
			}
			else {
				prev->next = temp->next;
				free(temp);
				*success = true;
				return head;
			}
		}
		prev = temp;
		temp = temp->next;
	}
	*success = false;
	return head;
}

void parse_areas(char *serialized_areas) {
	// The data is in this form: "segmid:segmid:segmid:..."
	// And all segmid should be added in the linked list area_list (being cleared before)
	int segmid;
	bool success = false;

	// Clearing the list
	free_list(area_list);
	area_list = NULL;

	// Parsing the data
	char *token = strtok(serialized_areas, ":");
	while(token != NULL) {
		segmid = atoi(token);
		area_list = add_node(area_list, segmid, &success);
		token = strtok(NULL, ":");
	}
}

void update_areas() {
	struct message msg_send, msg_rcv;

	// Serializing the message
	msg_send.code = ASK_AREAS;

	// Sending the message
	if (msgsnd(mailbox, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}

	// Waiting for the answer
	if (msgrcv(mailbox, &msg_rcv, sizeof(struct message)-sizeof(long), getpid(), 0) == -1) {
		perror("msgrcv");
		exit(1);
	}
	if (msg_rcv.code == LIST_AREAS) {
		// Parsing the list of areas
		parse_areas(msg_rcv.data);
	}
	else
		printf("Erreur lors de la récupération des emplacements\n");
}

void show_areas() {
	node_t * temp = area_list;
	area_t * area;

	printf("Liste des emplacements :\n");

	// Updating the list of areas
	update_areas();

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

/**
 * @brief Returns an area if it exists in the list
 * @param name: name of the area to search
 * @param head: head of the list
 * @return node_t: the area if it exists, NULL otherwise
 */
node_t * search_for_area(char* name, node_t * head) {
	node_t * temp = head;
	area_t * area;

	while(temp != NULL) {
		// Attaching shared memory
		area = (area_t *) shmat(temp->data, NULL, 0);
		if(area == (area_t *)-1) {
			perror("shmat");
			exit(1);
		}

		// Comparing names of areas and if found, detaching shared memory and returning 1
		if(strcmp(area->name, name) == 0) {
			if(shmdt(area) == -1) {
				perror("shmdt");
				exit(1);
			}
			return temp;
		}

		// Detaching shared memory and moving to the next node
		if(shmdt(area) == -1) {
			perror("shmdt");
			exit(1);
		}
		temp = temp->next;
	}
	return NULL;
}

void book_area() {
	char name[LENGTH_NAME_AREA];
	node_t * temp = area_list;
	area_t * area;
	bool success = false;

	printf("Quel emplacement voulez-vous réserver ? ");
	scanf("%s", name);

	// Searching for the area
	temp = search_for_area(name, area_list);
	if(temp == NULL) {
		printf("Cet emplacement n'existe pas\n");
		return;
	}

	// Attaching shared memory
	area = (area_t *) shmat(temp->data, NULL, 0);
	if(area == (area_t *)-1) {
		perror("shmat");
		exit(1);
	}

	// Checking if the area is already reserved
	if(area->shared_memory != 0) {
		printf("Cet emplacement est déjà réservé\n");
		return;
	}
	// Reserving the area
	else {
		area->shared_memory = getpid();
		// Adding to my bookings
		area_list = add_node(my_bookings, temp->data, &success);
		printf("Emplacement réservé avec succès\n");
	}

	// Detaching shared memory
	if(shmdt(area) == -1) {
		perror("shmdt");
		exit(1);
	}
}

void return_area() {
	char name[LENGTH_NAME_AREA];
	node_t * temp = my_bookings;
	area_t * area;
	bool success = false;

	printf("Quel emplacement voulez-vous rendre ? ");
	scanf("%s", name);

	// Searching for the area
	temp = search_for_area(name, my_bookings);
	if(temp == NULL) {
		printf("Cet emplacement n'existe pas\n");
		return;
	}

	// Attaching shared memory
	area = (area_t *) shmat(temp->data, NULL, 0);
	if(area == (area_t *)-1) {
		perror("shmat");
		exit(1);
	}

	// Checking if the area is already reserved
	if(area->shared_memory != getpid()) {
		printf("Cet emplacement n'est pas réservé par vous\n");
		return;
	}
	// Returning the area
	else {
		area->shared_memory = 0;
		// Removing from my bookings
		area_list = remove_node(my_bookings, name, &success);
		printf("Emplacement rendu avec succès\n");
	}

	// Detaching shared memory
	if(shmdt(area) == -1) {
		perror("shmdt");
		exit(1);
	}
}

void show_my_bookings() {
	node_t * temp = my_bookings;
	area_t * area;

	printf("Liste de mes réservations :\n");

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

		// Detaching shared memory and moving to the next node
		if(shmdt(area) == -1) {
			perror("shmdt");
			exit(1);
		}
		temp = temp->next;
	}
}

void add_area() {
	char name[LENGTH_NAME_AREA];
	char type;
	struct message msg_send, msg_rcv;

	// Asking for the name and the type of the area
	printf("Nom de l'emplacement : ");
	scanf("%s", name);
	getchar();
	printf("Type de l'emplacement (0 pour bureau, 1 pour salle de réunion) : ");
	scanf("%c", &type);

	// Serializing the area
	msg_send.mtype = 1;
	msg_send.sender = getpid();
	msg_send.code = CREATE_AREA;
	sprintf(msg_send.data, "%c:%s", type, name);

	// Sending the message
	printf("%c\n", type);
	if (msgsnd(mailbox, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}

	// Waiting for the answer
	if (msgrcv(mailbox, &msg_rcv, sizeof(struct message)-sizeof(long), getpid(), 0) == -1) {
		perror("msgrcv");
		exit(1);
	}
	if (msg_rcv.code == OK)
		printf("Emplacement ajouté avec succès\n");
	else
		printf("Erreur lors de l'ajout de l'emplacement\n");
}

void delete_area() {
    printf("Supprimer un earea\n");
}

void end() {
    exit(0);
}
