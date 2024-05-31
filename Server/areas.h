#ifndef PAS_AREAS_H
#define PAS_AREAS_H

#include <stdlib.h>
#include <stdio.h>
#include "../Common/common.h"

/**
 * @struct node
 * @brief Node structure to create a linked list
 */
typedef struct node {
	int data;
	struct node* next;
} node_t;

/**
 * @brief Create a new area
 * @param type: type of the area
 * @param name: name of the area
 * @return shared memory segment of the area
 */
int create_area(enum type type, char* name);

/**
 * @brief Verify if an area exists in the list
 * @param name: name of the area to search
 * @param head: head of the list
 * @return bool: true if the area exists, false otherwise
 */
bool area_exists(char* name, node_t * head);

/**
 * @brief Add an area to the list of known areas
 * @param head: head of the list
 * @param shared_memory: shared memory segment of the area to add
 */
node_t *add_area_to_list(node_t * head, int shared_memory);

/**
 * @brief Remove an area from the list of known areas
 * @param head: head of the list
 * @param shared_memory: shared memory segment of the area to remove
 */
node_t *remove_area_from_list(node_t * head, int shared_memory);

/**
 * @brief Print the list
 * @param head: head of the list
 */
void print_list(node_t * head);

/**
 * @brief Free the list
 * @param head: head of the list
 */
void free_list(node_t * head);

#endif //PAS_AREAS_H
