#include "server.h"
#include "areas.h"

// Global areas list
node_t *areas_list = NULL;

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
	areas_list = add_area_to_list(areas_list, desk);
	int meeting_room = create_area(MEETING_ROOM, "Meeting-Room-001");
	areas_list = add_area_to_list(areas_list, meeting_room);

	// Printing shared memory info
	print_shared_memory_info(desk);
	print_shared_memory_info(meeting_room);

	// Listing areas
	print_list(areas_list);

	// Removing areas
	areas_list = remove_area_from_list(areas_list, desk);
	areas_list = remove_area_from_list(areas_list, meeting_room);

	return 0;
}