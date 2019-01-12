/*
 * linkedList.c
 *
 *  See header
 *  
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

#define DEBUG 1
#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__ ); \
		} while (0)


//typedef struct Node {
//	char * key;
//	void * data;
//	struct node * next;
//} Node;
//
//typedef struct {
//	Node *first_node;
//	Node *last_node;
//	int length;
//} LinkedList;

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

void destory_linked_list(LinkedList * ls) {
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

	debug_print("Setting key %s:\n",key);
	exists_data = linked_list_get(ls,key);
	if (exists_data) {
		debug_print("Key %s already exists in the list, seeting it to new value.\n");
		memcpy(exists_data, data, data_size);
	} else {
		debug_print("Key %s does not exists. Allocating memory for a new node.\n");
		key_length = strlen(key);
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
 return NULL;
}
