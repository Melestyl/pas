#include "server.h"

#define STR_LARGE 100
#define STR_SMALL 50

int create_area(enum type type, char* name) {
	static unsigned project_id = 1;
	char shared_memory_path[STR_LARGE];
	key_t token;
	int shared_memory_segment;
	area_t *area;

	// Retrieving shared memory path
	read_config("../Common/config.txt", "SHARED_MEMORY_PATH", shared_memory_path);

	// Initializing shared memory
	token = ftok(shared_memory_path, project_id++);
	shared_memory_segment = shmget(token, sizeof(area_t), IPC_CREAT | 0666);

	// Attaching shared memory
	area = (area_t*) shmat(shared_memory_segment, NULL, 0);

	// Setting type
	area->type = type;

	// Setting name
	strcpy(area->name, name);

	// Initializing mutex
	pthread_mutex_init(&area->mutex, NULL);

	// Returning shared memory segment
	return shared_memory_segment;
}

void print_shared_memory_info(int shared_memory) {
	struct shmid_ds shmid_ds;
	if (shmctl(shared_memory, IPC_STAT, &shmid_ds) == -1) {
		perror("shmctl");
		exit(1);
	}

	printf("Shared memory info:\n");
	printf("  - Segment ID: %d\n", shared_memory);
	printf("  - Size: %ld\n", shmid_ds.shm_segsz);
	printf("  - Last attach time: %ld\n", shmid_ds.shm_atime);
	printf("  - Last detach time: %ld\n", shmid_ds.shm_dtime);
	printf("  - Last change time: %ld\n", shmid_ds.shm_ctime);
	printf("  - Number of attaches: %ld\n", shmid_ds.shm_nattch);
}

int main() {
	// Creating areas
	int desk = create_area(DESK, "Desk-001");
	int meeting_room = create_area(MEETING_ROOM, "Meeting-Room-001");

	// Printing shared memory info
	print_shared_memory_info(desk);
	print_shared_memory_info(meeting_room);

	// Deleting shared memory
	if (shmctl(desk, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}
	if (shmctl(meeting_room, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}

	return 0;
}