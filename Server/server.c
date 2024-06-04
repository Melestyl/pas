#include "server.h"
#include "areas.h"

#define STR_LARGE 100
#define STR_SMALL 50

// Global areas list
node_t *areas_list = NULL;

// Global message queue id
int mailbox;

int main(int argc, char* argv[]) {
	int nb_areas = 0;
	int area_segm_id;
	bool success;
	key_t msg_key = MESSAGE_KEY; // key of the message queue
	int msg_flag = IPC_CREAT | 0666; // flag of the message queue
	enum type enum_type; // type of the area
	char type; // equivalent of enum type in char
	char name[LENGTH_NAME_AREA]; // name of the area
	struct sigaction act;

	// Handling SIGINT using sigaction
	act.sa_handler = sigint_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);

	// Creating message queue
    if ((mailbox = msgget(msg_key,msg_flag)) == -1)
    {
		perror("msgget");
		exit(1);
    }

	printf("Message queue created with id %d\n", mailbox);

	while(1){
		struct message msg;

		// Receiving message
		if (msgrcv(mailbox, &msg, sizeof(struct message)-sizeof(long), 1, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		printf("Message received by %d with the code %d\n", msg.sender, msg.code);

		// Processing message
		switch (msg.code) {
			case OK:
				printf("\"OK\" the server does not receive this code : send NOK\n");
				send_nok(msg.sender, mailbox);
				break;
			case NOK:
				printf("\"NOK\" the server does not receive this code : send NOK\n");
				send_nok(msg.sender, mailbox);
				break;
			case ASK_AREAS:
				printf("Message code ASK_AREAS\n");
				list_areas(msg.sender, mailbox);
				break;
			case LIST_AREAS:
				printf("\"LIST_AREAS\" the server does not receive this code : send NOK\n");
				send_nok(msg.sender, mailbox);
				break;
			case CREATE_AREA:
				printf("Message code CREATE_AREA\n");

				// Check if the maximum number of areas is not reached
				if (nb_areas >= MAX_AREA) {
					printf("Too many areas\n");
					send_nok(msg.sender, mailbox);
					break;
				}

				// Reading type and name of the message
				sscanf(msg.data, "%c:%s", &type, name);

				// Check if the name is not already used
				if (area_exists(name, areas_list)) {
					printf("The name of the area already exists\n");
					send_nok(msg.sender, mailbox);
					break;
				}


				// Convert char type to enum type
				if (type == '0')
					enum_type = DESK;
				else if (type == '1')
					enum_type = MEETING_ROOM;
				else {
					printf("Unknown type\n");
					send_nok(msg.sender, mailbox);
					break;
				}
				
				// Create the area and get the shared memory segment id
				area_segm_id = create_area(enum_type, name);
				if(area_segm_id == -1) {
					printf("Error while creating the area\n");
					send_nok(msg.sender, mailbox);
					break;
				}

				nb_areas++;

				//add the area to the list
				printf("Adding the area to the list\n");
				areas_list=add_area_to_list(areas_list,area_segm_id,&success);
				if(!success) {
					printf("Error while adding the area to the list\n");
					send_nok(msg.sender, mailbox);
					break;
				}
				else {
					printf("Area added to the list\n");
					send_ok(msg.sender, mailbox);
				}
				
				break;
			case DEL_AREA:
				printf("Message code DEL_AREA\n");

				// Reading segm id received
				sscanf(msg.data,"%s", name);

				//delete area
				del_area(name, &success);
				if(!success) {
					printf("Error while deleting the area\n");
					send_nok(msg.sender, mailbox);
					break;
				}
				nb_areas--;

				//send ok
				send_ok(msg.sender, mailbox);
				break;
			default:
				printf("Unknown message code : send NOK\n");
				send_nok(msg.sender, mailbox);
				break;
		}
	}

}

void sigint_handler(int sig) {
	node_t * temp = areas_list;

	printf("SIGINT received\n");

	// Removing the message queue
	if (msgctl(mailbox, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}

	// Removing the shared memory segments
	while(temp != NULL) {
		if (shmctl(temp->data, IPC_RMID, NULL) == -1) {
			perror("shmctl");
			exit(1);
		}
		temp = temp->next;
	}

	// Exiting the server
	exit(0);
}

void send_nok(pid_t sender, int mailbox) {
	struct message msg_send;

	//init message
	msg_send.mtype = sender;
	msg_send.sender = getpid();
	msg_send.code = NOK;

	//send message
	if (msgsnd(mailbox, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}
	printf("NOK message envoyé à %d\n", sender);
}

void send_ok(pid_t sender, int mailbox) {
	struct message msg_send;

	//init message
	msg_send.mtype = sender;
	msg_send.sender = getpid();
	msg_send.code = OK;

	//send message
	if (msgsnd(mailbox, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}
	printf("OK message envoyé à %d\n", sender);
}

void list_areas(pid_t sender, int mailbox) {
	char data[MAX_LENGTH_DATA], tmp[LENGTH_ID_SEGM];
	node_t * temp = areas_list;
	struct message msg_send;

	//init data
	strcpy(data, "");
	strcpy(msg_send.data, "");

	//read the list of areas and put it in data
	while(temp != NULL) {
		sprintf(tmp, "%d:", temp->data);
		strcat(msg_send.data, tmp);
		temp = temp->next;
	}
	msg_send.data[strlen(msg_send.data)-1] = '\0'; //removing the last ':' and replacing it by '\0'

	//init message
	msg_send.mtype = sender;
	msg_send.sender = getpid();
	msg_send.code = LIST_AREAS;

	//send message
	if (msgsnd(mailbox, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
				send_nok(sender, mailbox);
	}
}

void del_area(char* name,bool* success) {
	areas_list=remove_area_from_list(areas_list,name, success);
}