#include "areas.h"

#define STR_LARGE 100
#define STR_SMALL 50



/**
 * @brief Create a new node
 * @param data: data to store in the node
 * @return new node
 */
node_t* create_node(int data) {
	node_t* new_node = (node_t *) malloc(sizeof(node_t));
	if(new_node == NULL) {
		printf("Error creating a new node.\n");
		exit(0);
	}
	new_node->data = data;
	new_node->next = NULL;

	return new_node;
}

/**
 * @brief Add a new node to the list
 * @param head: head of the list
 * @param data: data to store in the node
 * @return new head of the list
 */
node_t * add_node(node_t *head, int data) {
	node_t * new_node = create_node(data);
	node_t * temp = head;

	if(head == NULL)
		head = new_node;
	else {
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}

	return head;
}

/**
 * @brief Delete an area
 * @param shared_memory: shared memory segment of the area
 */
void delete_area(int shared_memory) {
	if (shmctl(shared_memory, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	}
}

/**
 * @brief Remove a node from the list
 * @param head: head of the list
 * @param data: data to remove
 * @return new head of the list
 */
node_t *remove_node(node_t * head, int data) {
	node_t * temp = head;
	node_t * prev = NULL;

	while(temp != NULL) {
		if(temp->data == data) {
			if(prev == NULL) {
				head = temp->next;
				delete_area(temp->data);
				free(temp);
				return head;
			}
			else if(prev == NULL && temp->next == NULL){
				delete_area(temp->data);
				free(temp);
				return NULL;
			}
			else {
				prev->next = temp->next;
				delete_area(temp->data);
				free(temp);
				return head;
			}
		}
		prev = temp;
		temp = temp->next;
	}

	return head;
}

/**
 * @brief Print the list
 * @param head: head of the list
 */
void print_list(node_t * head) {
	node_t * temp = head;

	while(temp != NULL) {
		printf("%d -> ", temp->data);
		temp = temp->next;
	}

	printf("NULL\n");
}

/**
 * @brief Free the list
 * @param head: head of the list
 */
void free_list(node_t * head) {
	node_t * temp;

	while(head != NULL) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

/**
 * @brief Create a new area
 * @param type: type of the area
 * @param name: name of the area
 * @return shared memory segment of the area
 */
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

/**
 * @brief Verify if an area exists in the list
 * @param name: name of the area to search
 * @param head: head of the list
 * @return int: 1 if the area exists, 0 otherwise
 */
int area_exists(char name[LENGTH_NAME_AREA], node_t * head) {
	node_t * temp = head;
	area_t * area;

	while(temp != NULL) {
		// Attaching shared memory
		if( area = (area_t *) shmat(temp->data, NULL, 0) == -1) {
			perror("shmat");
			exit(1);
		}

		// Comparing names of areas and if found, detaching shared memory and returning 1
		if(strcmp(area->name, name) == 0) {
			if(shmdt(area) == -1) {
				perror("shmdt");
				exit(1);
			}
			return 1;
		}

		// Detaching shared memory and moving to the next node
		if(shmdt(area) == -1) {
			perror("shmdt");
			exit(1);
		}
		temp = temp->next;
	}
	return 0;
}

/**
 * @brief Add an area to the list of known areas
 * @param head: head of the list
 * @param shared_memory: shared memory segment of the area to add
 */
node_t * add_area_to_list(node_t * head, int shared_memory) {
	if (head == NULL)
		return create_node(shared_memory);
	else
		return add_node(head, shared_memory);
}

/**
 * @brief Remove an area from the list of known areas
 * @param head: head of the list
 * @param shared_memory: shared memory segment of the area to remove
 */
node_t *remove_area_from_list(node_t * head, int shared_memory) {
	return remove_node(head, shared_memory);
}