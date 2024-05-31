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
		return NULL;
	}
	new_node->data = data;
	new_node->next = NULL;

	return new_node;
}

/**
 * @brief Add a new node to the list
 * @param head: head of the list
 * @param data: data to store in the node
 * @param success: bool to indicate if the node was added
 * @return new head of the list
 */
node_t * add_node(node_t *head, int data, bool* success) {
	node_t * new_node = create_node(data);
	if(new_node == NULL) {
		printf("Error creating a new node.\n");
		*success = false;
		return head;
	}
	node_t * temp = head;

	if(head == NULL)
		head = new_node;
	else {
		while(temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
	*success = true;
	return head;
}

/**
 * @brief Delete an area
 * @param shared_memory: shared memory segment of the area
 * @return bool: true if the area was deleted, false otherwise
 */
bool delete_area(int shared_memory) {
	if (shmctl(shared_memory, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		return false;
	}
	return true;
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
				if(!delete_area(temp->data))
					*success = false;
				free(temp);
				*success = true;
				return head;
			}
			else if(prev == NULL && temp->next == NULL){
				if(!delete_area(temp->data))
					*success = false;
				free(temp);
				*success = true;
				return NULL;
			}
			else {
				prev->next = temp->next;
				if(!delete_area(temp->data))
					*success = false;
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
 * @return -1 if an error occurred
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
	if(shared_memory_segment == -1) {
		perror("shmget");
		return -1;
	}

	// Attaching shared memory
	area = (area_t*) shmat(shared_memory_segment, NULL, 0);
	if(area == (area_t *)-1) {
		perror("shmat");
		return -1;
	}

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
 * @return bool: true if the area exists, false otherwise
 */
bool area_exists(char* name, node_t * head) {
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
			return true;
		}

		// Detaching shared memory and moving to the next node
		if(shmdt(area) == -1) {
			perror("shmdt");
			exit(1);
		}
		temp = temp->next;
	}
	return false;
}

/**
 * @brief Add an area to the list of known areas
 * @param head: head of the list
 * @param shared_memory: shared memory segment of the area to add
 * @param success: bool to indicate if the node was added
 * @return new head of the list
 */
node_t * add_area_to_list(node_t * head, int shared_memory, bool* success) {
	return add_node(head, shared_memory, success);
}

/**
 * @brief Remove an area from the list of known areas
 * @param head: head of the list
 * @param name: name of the area to remove
 * @param success: bool to indicate if the node was removed
 * @return new head of the list
 */
node_t *remove_area_from_list(node_t * head, char* name, bool* success) {
	return remove_node(head, name, success);
}