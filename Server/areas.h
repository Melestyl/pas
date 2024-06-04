#ifndef PAS_AREAS_H
#define PAS_AREAS_H

#include <stdlib.h>
#include <stdio.h>
#include "../Common/common.h"

/**
 * @brief Create a new area
 * @param type: type of the area
 * @param name: name of the area
 * @return shared memory segment of the area
 * @return -1 if an error occurred
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
 * @brief Delete an area
 * @param shared_memory: shared memory segment of the area
 * @return bool: true if the area was deleted, false otherwise
 */
bool delete_area(int shared_memory);

/**
 * @brief Add an area to the list of known areas
 * @param head: head of the list
 * @param shared_memory: shared memory segment of the area to add
 * @param success: bool to indicate if the node was added
 * @return new head of the list
 */
node_t * add_area_to_list(node_t * head, int shared_memory, bool* success);

/**
 * @brief Remove an area from the list of known areas
 * @param head: head of the list
 * @param name: name of the area to remove
 * @param success: bool to indicate if the node was removed
 * @return new head of the list
 */
node_t *remove_area_from_list(node_t * head, char* name, bool* success);

#endif //PAS_AREAS_H
