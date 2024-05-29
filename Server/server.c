#include "server.h"

#define STR_LARGE 100
#define STR_SMALL 50


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
				send_nok();
				break;
			case NOK:
				printf("\"NOK\" the server does not receive this code : send NOK\n");
				send_nok();
				break;
			case ASK_AREAS:
				printf("Message code ASK_AREAS\n");
				ask_areas();
				break;
			case LIST_AREAS:
				printf("\"LIST_AREAS\" the server does not receive this code : send NOK\n");
				send_nok();
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
				send_nok();
				break;
		}
	}

}


area_t* create_area(enum type type, char* name) {
	static unsigned shared_memory_id = 0;
	char shared_memory_prefix[STR_SMALL], project_id_str[STR_SMALL], shared_memory_path[STR_LARGE];
	int project_id;
	key_t token;

	// Allocating memory for area
	area_t* area = (area_t*) malloc(sizeof(area_t));
	if (area == NULL) {
		perror("malloc");
		exit(1);
	}

	// Setting type
	area->type = type;

	// Setting name
	strcpy(area->name, name);

	// Retrieving shared memory path and project id
	read_config("../Common/config.txt", "SHARED_MEMORY_PREFIX", shared_memory_prefix);
	project_id = atoi(read_config("../Common/config.txt", "PROJECT_ID", project_id_str));

	// Initializing shared memory
	sprintf(shared_memory_path, "%s%d", shared_memory_prefix, shared_memory_id++);
	token = ftok(shared_memory_path, project_id);
	area->shared_memory = shmget(token, sizeof(int), IPC_CREAT | 0666);
	if (area->shared_memory == -1) {
		perror("shmget");
		exit(1);
	}
	strcpy(area->path, shared_memory_path);

	// Initializing mutex
	pthread_mutex_init(&area->mutex, NULL);

	// Returning area's pointer
	return area;
}

int main() {
	return 0;
}