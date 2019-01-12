/*
 * linkedList.c
 *
 *  See header
 *  
 */


#include "LinkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 0
#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__ ); \
		} while (0)


LinkedList *create_linked_list() {

	LinkedList * returned_pointer;
	returned_pointer = (LinkedList *) malloc(sizeof(LinkedList));
	if (!returned_pointer) {
		//memory allocation failed
		debug_print("Memory allocation failed for new linked list.\n");
		return NULL;

	}
	returned_pointer->first_node = NULL;
	returned_pointer->last_node = NULL;
	returned_pointer->length = 0;
	return returned_pointer;
}


void _linked_list_destroy_node(Node * node) {
	free(node->data); // free the data space
	free(node->key); // free the key space
	free(node); // frees the node
}

void destroy_linked_list(LinkedList * ls) {
	/* iterating over the list freeing any unfree'd nodes */
	Node * next_node;
	Node * current_node;
	next_node = ls->first_node;
	debug_print("Starting to release linked list.\n");

	while (next_node != NULL) {
		current_node = next_node;
		next_node = next_node->next;
		debug_print("Freeing memory for node with key %s.\n",current_node->key);
		_linked_list_destroy_node(current_node);
	}
	debug_print("No more nodes left to free. Now freeing the list itself.\n");
	free(ls);
}


int linked_list_set(LinkedList * ls, char * key, void * data, size_t data_size) {
	void * exists_data;
	Node * new_node;
	size_t key_length;

	key_length = strlen(key);
	if (key_length > KEY_MAX_LENGTH) {
		// key is too long
		debug_print("Key is too long ! key provided: \n%s\n",key);
		fprintf(stderr, "Provided with a key of length %zd when the max size is %d.\n",key_length,KEY_MAX_LENGTH);
	}

	debug_print("Setting key %s:\n",key);
	exists_data = linked_list_get(ls,key);
	if (exists_data) {
		debug_print("Key %s already exists in the list, seeting it to new value.\n");
		memcpy(exists_data, data, data_size);
	} else {
		debug_print("Key %s does not exists. Allocating memory for a new node.\n");


		new_node = (Node *) malloc(sizeof(Node));
		new_node->data = (void *) malloc(data_size);
		new_node->key = (char *) malloc((sizeof(char)*key_length)+1);
		if (new_node->key==NULL || new_node->data==NULL) {
			debug_print("Memory allocation failed.\n");
			return -1;
		}
		debug_print("Data allocation successful for key %s.\n",key);
		memcpy(new_node->data,data,data_size);
		strcpy(new_node->key,key);
		debug_print("Mem and string copy successful.\n");

		new_node->next = NULL;

		if (ls->last_node==NULL) { // list is empty
			ls->first_node=new_node;
			ls->last_node=new_node;
		} else {
			ls->last_node->next = new_node;
			ls->last_node = new_node;
		}

		ls->length++;
		debug_print("Key %s added successfully.\n",key);

	}
	return 1;
}



void * linked_list_get(LinkedList * ls, char * key) {
	Node * current_node;
	debug_print("Trying to find key %s.\n",key);

	if (ls->first_node == NULL || ls->length==0) {
				//list is empty
		debug_print("List is empty, returning NULL.\n");
		return NULL;
	}

	current_node = ls->first_node;
	debug_print("Iterating through the list\n");
	while (current_node!=NULL) {
		debug_print("Comparing to key: %s\n",current_node->key);
		if (strcmp(key,current_node->key)==0) {
			debug_print("Key is found, returning.\n");
			return current_node->data;
		} else {
			current_node = current_node->next;
		}
	}
	debug_print("Reached end of the list, returning NULL.\n");
	return NULL;
}
