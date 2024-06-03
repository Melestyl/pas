#ifndef PAS_LINKED_LIST_H
#define PAS_LINKED_LIST_H

#include "common.h"
#define bool char

/**
 * @struct node
 * @brief Node structure to create a linked list
 */
typedef struct node {
	int data;
	struct node* next;
} node_t;

/**
 * @brief Create a new node
 * @param data: data to store in the node
 * @return new node
 */
node_t* create_node(int data);

/**
 * @brief Add a new node to the list
 * @param head: head of the list
 * @param data: data to store in the node
 * @param success: bool to indicate if the node was added
 * @return new head of the list
 */
node_t * add_node(node_t *head, int data, bool* success);

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

#endif //PAS_LINKED_LIST_H
