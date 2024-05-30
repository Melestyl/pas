#include "server.h"
#include "areas.h"

#define STR_LARGE 100
#define STR_SMALL 50

// Global areas list
node_t *areas_list = NULL;

int main(int argc, char* argv[]) {
	int nb_areas = 0;
	int num, area_segm_id;
    key_t msg_key = MESSAGE_KEY; // key of the message queue
    int msg_flag = IPC_CREAT | IPC_EXCL | 0666; // flag of the message queue
	enum type enum_type; // type of the area
	char type; // equivalent of enum type in char
	char name[LENGTH_NAME_AREA]; // name of the area

	// Creating message queue
    if ((num = msgget(msg_key,msg_flag)) == -1)
    {
		perror("msgget");
		exit(1);
    }

	printf("Message queue created with id %d\n", num);

	while(1){
		struct message msg;

		// Receiving message
		if (msgrcv(num, &msg, sizeof(struct message)-sizeof(long), 1, 0) == -1) {
			perror("msgrcv");
			exit(1);
		}
		printf("Message received by %d with the code %d\n", msg.sender, msg.code);

		// Processing message
		switch (msg.code) {
			case OK:
				printf("\"OK\" the server does not receive this code : send NOK\n");
				send_nok();
				break;
			case NOK:
				printf("\"NOK\" the server does not receive this code : send NOK\n");
				send_nok();
				break;
			case ASK_AREAS:
				printf("Message code ASK_AREAS\n");
				ask_areas(msg.sender, num);
				break;
			case LIST_AREAS:
				printf("\"LIST_AREAS\" the server does not receive this code : send NOK\n");
				send_nok();
				break;
			case CREATE_AREA:
				printf("Message code CREATE_AREA\n");

				// Check if the maximum number of areas is not reached
				if (nb_areas > MAX_AREA) {
					printf("Too many areas\n");
					send_nok();
					break;
				}

				// Reading type and name of the message
				sscanf(msg.data, "%c:%s", &type, name);

				//check if the name is not already used
				if (area_exists(name, areas_list)) {
					printf("The name of the area already exists\n");
					send_nok();
					break;
				}


				//convert char type to enum type
				if (type == 0)
					enum_type = DESK;
				else
					enum_type = MEETING_ROOM;
				
				area_segm_id = create_area(enum_type, name);
				nb_areas++;

				//add the area to the list
				areas_list=(areas_list,area_segm_id);
				
				break;
			case DEL_AREA:
				printf("Message code DEL_AREA\n");

				// Reading segm id received
				sscanf(msg.data,"%s", area_segm_id);

				//delete area
				del_area(area_segm_id); //TODO: anticipate the case of a deletion error
				
				//send ok
				send_ok(msg.sender, num);


				break;
			default:
				printf("Unknown message code : send NOK\n");
				send_nok();
				break;
		}
	}

}

void send_nok() {
	// TODO
}

void send_ok(pid_t sender, int num) {
	struct message msg_send;

	//init message
	msg_send.mtype = sender;
	msg_send.sender = getpid();
	msg_send.code = OK;

	//send message
	if (msgsnd(num, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}
	printf("OK message envoyé à %d\n", sender);
}

void ask_areas(pid_t sender, int num) {
	char data[MAX_LENGTH_DATA];
	node_t * temp = areas_list;
	struct message msg_send;

	//read the list of areas and put it in data
	while(temp != NULL) {
		sprintf(data, "%d:", temp->data);
		temp = temp->next;
	}
	data[strlen(data)-1] = '\0'; //removing the last ':' and replacing it by '\0'

	//init message
	msg_send.mtype = sender;
	msg_send.sender = getpid();
	msg_send.code = LIST_AREAS;
	strcpy(msg_send.data, data);

	//send message
	if (msgsnd(num, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}
}

void del_area(int area_segm_id) {
	areas_list=(areas_list,area_segm_id);
}