#include "server.h"
#include "areas.h"

#define STR_LARGE 100
#define STR_SMALL 50

// Global areas list
node_t *areas_list = NULL;

int main(int argc, char* argv[]) {
	int num;
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
				send_nok(msg.sender, num);
				break;
			case NOK:
				printf("\"NOK\" the server does not receive this code : send NOK\n");
				send_nok(msg.sender, num);
				break;
			case ASK_AREAS:
				printf("Message code ASK_AREAS\n");
				ask_areas();
				break;
			case LIST_AREAS:
				printf("\"LIST_AREAS\" the server does not receive this code : send NOK\n");
				send_nok(msg.sender, num);
				break;
			case CREATE_AREA:
				printf("Message code CREATE_AREA\n");
				
				// Reading type and name of the message
				scanf("%c:%s", &type, name);

				//convert char type to enum type
				if (type == 0)
					enum_type = DESK;
				else
					enum_type = MEETING_ROOM;
				
				create_area(enum_type, name);
				break;
			case DEL_AREA:
				printf("Message code DEL_AREA\n");
				del_area();
				break;
			default:
				printf("Unknown message code : send NOK\n");
				send_nok(msg.sender, num);
				break;
		}
	}

}

void send_nok(pid_t sender, int num) {
	struct message msg_send;

	//init message
	msg_send.mtype = sender;
	msg_send.sender = getpid();
	msg_send.code = NOK;

	//send message
	if (msgsnd(num, &msg_send, sizeof(struct message)-sizeof(long), 0) == -1) {
		perror("msgsnd");
		exit(1);
	}
	printf("NOK message envoyé à %d\n", sender);
}

void ask_areas() {
// TODO
}
void del_area() {
// TODO
}