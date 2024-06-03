#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

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

