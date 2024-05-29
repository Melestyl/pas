#include "server.h"

area_t* create_area(enum type type, char* name) {
	static unsigned shared_memory_id = 0;
	char shared_memory_prefix[100], project_id_str[100], shared_memory_path[120];
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