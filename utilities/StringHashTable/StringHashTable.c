/*
 * stringHashTable.c
 *
 *  Implements Hash Table of Strings
 *
 */

#include "StringHashTable.h"


/*
 * String hash function is based on djb2 as published by Dan Bernsteing.
 * Turns out 33 is a magic number. Anyhow, this is published as best practice to use.
 */
unsigned long
hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    c = *str++;
    while (c) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = *str++;
    }

    return hash % HASH_TABLE_SIZE;
}

StringHashTable *create_string_hash_table() {
	StringHashTable * newTable;
	newTable = (StringHashTable *) malloc(sizeof(StringHashTable));
	if (!newTable) {
		/* memory allocation failed */
		debug_print("Memory allocation failed for new linked list.");
		return NULL;
	}

	newTable->length = 0;
	memset(newTable->hashTable, 0, sizeof(LinkedList*) * HASH_TABLE_SIZE);
	return newTable;
}

void destroy_string_hash_table(StringHashTable * table) {

	LinkedList * list;
	int i;

	for (i=0; i<HASH_TABLE_SIZE; i++) {
		list = table->hashTable[i];
		if (list!=NULL) {
			destroy_linked_list(list);
		}

	}

	free(table);
}

int string_hash_table_set(StringHashTable * table, char * key, void * data, size_t data_length) {
	unsigned long hashValue;
	hashValue = hash(key);

	if (table->hashTable[hashValue] == NULL) {
		/* Linked list does not exists and needs to be created */
		table->hashTable[hashValue] = create_linked_list();
		if (table->hashTable[hashValue] == NULL) {
			/* allocation failed */
			return -1;
		}
	}
	table->length++;
	return linked_list_set(table->hashTable[hashValue], key, data, data_length);
}

void * string_hash_table_get(StringHashTable * table, char * key) {
	unsigned long hashValue;
	hashValue = hash(key);
	if (!table->hashTable[hashValue]) return NULL; /* linked list doesn't exist thus key doesn't either */
	return linked_list_get(table->hashTable[hashValue], key);
}


int string_hash_table_get_keys(StringHashTable * table, char *** keys) {
	char ** keysArray;
	char ** tempArray;
	LinkedList * list;
	int length;
	int i,j;
	int keysArrIndex = 0;

	keysArray = (char**) malloc(sizeof(char*) * table->length);

	for (i=0; i<HASH_TABLE_SIZE; i++) {
		list = table->hashTable[i];
		if (list != NULL) {
			length = linked_list_get_keys(list,&tempArray);
			for (j=0; j<length; j++) {
				keysArray[keysArrIndex]=tempArray[j];
				keysArrIndex++;
			}
			free(tempArray);
		}
	}

	*keys = keysArray;
	return table->length;
}


