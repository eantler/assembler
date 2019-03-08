/*
 * linkedList.h
 *
 *  A basic key-value store implementation using linked list.
 *  Designed to be used with string hash map implementation.
 *
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdlib.h>
#define KEY_MAX_LENGTH 31


/*
 * Because input is not limited in length (might have many collisions in hash map) we need to have a dynamic data structure.
 * Most insertions always happen at the end of an array, so I choose to hold a pointer to that as well.
 *
 * Here we define the linked list structure and interface which will be used for that
 */

typedef struct Node {
	char * key;
	void * data;
	struct Node * next;
} Node;

typedef struct LinkedList {
	Node *first_node;
	Node *last_node;
	int length;
} LinkedList;

/*
 * linked_list *create_linked_list()
 * Allocated a new linked_list data structure and returns a pointer to it.
 * If memory allocation fails, returns NULL.
 */

LinkedList *create_linked_list();


/*
 * destory_linked_list(LinkedList * ls)
 * @param ls - the linked list pointer to be destroyed.
 *
 * This function expect ls to be an initiated LinkedList.
 * It frees all memory related to that LinkedList.
 */

void destroy_linked_list(LinkedList * ls);


/*
 * set(LinkedList * ls, int key, void * data)
 *
 * Function sets a key to a certain value in the store.
 * Returns -1 if failed.
 *
 * @params
 * LinkedList * ls - pointer to the linked list we're setting into
 * char * key - key to be set
 * void * data - pointer to data stored
 * size_t data_size - size of data stored
 *
 * @return
 * 1 if successful else -1
 *
 */

int linked_list_set(LinkedList * ls, char * key, void * data, size_t data_size);


/*
 * void * get(LinkedList * ls, char * key, void ** dataPointer)
 *
 * Function finds a key by a certain value in the store.
 * Returns NULL if none was found.
 * If it was found returning pointer to value
 *
 * @params
 * LinkedList * ls - pointer to the linked list we're getting from
 * char * key - key in store
 *
 */

void * linked_list_get(LinkedList * ls, char * key);

/*
 * int linked_list_get_keys(LinkedList * ls, char ** keys);
 *
 * Function set keys to point to an array of keys available.
 *
 * @params
 * ls - LinkedList instance
 * keys - pointer to keys array that the function will update
 */

int linked_list_get_keys(LinkedList * ls, char *** keys);

#endif /* LINKEDLIST_H_ */
