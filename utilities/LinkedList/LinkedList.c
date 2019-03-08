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
#include "../utils.h"


LinkedList *create_linked_list() {

	LinkedList * returned_pointer;
	returned_pointer = (LinkedList *) malloc(sizeof(LinkedList));
	if (!returned_pointer) {
		/* memory allocation failed */
		debug_print("Memory allocation failed for new linked list.");
		return NULL;

	}
	returned_pointer->first_node = NULL;
	returned_pointer->last_node = NULL;
	returned_pointer->length = 0;
	return returned_pointer;
}


void _linked_list_destroy_node(Node * node) {
	free(node->data); /* free the data space */
	free(node->key); /* free the key space */
	free(node); /* frees the node */
}

void destroy_linked_list(LinkedList * ls) {
	/* iterating over the list freeing any unfree'd nodes */
	Node * next_node;
	Node * current_node;
	next_node = ls->first_node;
	debug_print("Starting to release linked list.");

	while (next_node != NULL) {
		current_node = next_node;
		next_node = next_node->next;
		debug_print("Freeing memory for node with key %s.",current_node->key);
		_linked_list_destroy_node(current_node);
	}
	debug_print("No more nodes left to free. Now freeing the list itself.");
	free(ls);
}


int linked_list_set(LinkedList * ls, char * key, void * data, size_t data_size) {
	void * exists_data;
	Node * new_node;
	size_t key_length;

	key_length = strlen(key);
	if (key_length > KEY_MAX_LENGTH) {

		debug_print("Key is too long ! key provided: \n%s",key);
		fprintf(stderr, "Provided with a key which is too long ! max size is %d, key provided: \n%s\n",KEY_MAX_LENGTH,key);

	}

	debug_print("Setting key %s:",key);
	exists_data = linked_list_get(ls,key);
	if (exists_data) {
		debug_print("Key %s already exists in the list, seeting it to new value.", key);
		memcpy(exists_data, data, data_size);
	} else {
		debug_print("Key %s does not exists. Allocating memory for a new node.", key);


		new_node = (Node *) malloc(sizeof(Node));
		new_node->data = (void *) malloc(data_size);
		new_node->key = (char *) malloc((sizeof(char)*key_length)+1);
		if (new_node->key==NULL || new_node->data==NULL) {
			debug_print("Memory allocation failed.");
			return -1;
		}
		debug_print("Data allocation successful for key %s.",key);
		memcpy(new_node->data,data,data_size);
		strcpy(new_node->key,key);
		debug_print("Mem and string copy successful.");
		new_node->next = NULL;

		if (ls->last_node==NULL) { /* list is empty */
			ls->first_node=new_node;
			ls->last_node=new_node;
		} else {
			ls->last_node->next = new_node;
			ls->last_node = new_node;
		}

		ls->length++;
		debug_print("Key %s added successfully.",key);

	}
	return 1;
}



void * linked_list_get(LinkedList * ls, char * key) {
	Node * current_node;
	debug_print("Trying to find key %s.",key);

	if (ls->first_node == NULL || ls->length==0) {
		/* list is empty */
		debug_print("List is empty, returning NULL.");
		return NULL;
	}

	current_node = ls->first_node;
	debug_print("Iterating through the list");
	while (current_node!=NULL) {
		debug_print("Comparing to key: %s",current_node->key);
		if (strcmp(key,current_node->key)==0) {
			debug_print("Key is found, returning.");
			return current_node->data;
		} else {
			current_node = current_node->next;
		}
	}
	debug_print("Reached end of the list, returning NULL.");
	return NULL;
}

int linked_list_get_keys(LinkedList * ls, char *** keys) {
	Node * current_node;
	int i;
	char ** newKeys;

	debug_print("Allocating space for new keys of length %d. Altogether %d bytes", ls->length,(sizeof(char*)) * (ls->length));
	newKeys = (char**) malloc(sizeof(char*) * ls->length);

	if (newKeys==NULL)  {
		debug_print("Allocation failed");
		return -1;
	}
	debug_print("Allocation was successful newKeys=%d",*newKeys);


	if (  ls->first_node == NULL || ls->length==0) {
		debug_print("List is empty");
		return 0;
	}
	debug_print("Start iterating through the list");
	current_node = ls->first_node;
	i=0;
	while (current_node != NULL) {
		debug_print("Next key is %s, i=%d",current_node->key,i);
		debug_print("(current_node->key)= %d, newKeys[i]=%d",current_node->key,newKeys[i]);
		newKeys[i] = (current_node->key);

		debug_print("Setting key %d to be equal to %s. It is now set to %s", i, current_node->key, newKeys[i]);
		i++;
		current_node = current_node->next;
	}
	*keys = newKeys;
	debug_print("returning successfully now. length = %d",ls->length);
	return ls->length;

}
