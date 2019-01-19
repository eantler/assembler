/*
 * StringHashTable.h
 *
 *  StringHashTable provides a simple interface for a string key value store
 *  based on hash table.
 *
 *  Keys needs to be strings (char *).
 *  Data can be any (void *) but size should be provided.
 */

#ifndef STRINGHASHTABLE_STRINGHASHTABLE_H_
#define STRINGHASHTABLE_STRINGHASHTABLE_H_

#define MAX_KEY_SIZE 31
#define HASH_TABLE_SIZE 120
#include <string.h>
 /* Hash table is using the linked list for its nodes */
#include "../LinkedList/LinkedList.h"
#include "../utils.h"

typedef struct {
	LinkedList * hashTable[HASH_TABLE_SIZE];
	int length;
} StringHashTable;


/*
 * StringHashTable *create_string_hash_table()
 *
 * Function creates and initiate a new string hash table.
 *
 * @return
 * Pointer to newly created StringHashTable;
 */

StringHashTable *create_string_hash_table();



/*
 * void destroy_string_hash_table
 *
 * function frees all the memory used by hash table.
 *
 */
void destroy_string_hash_table(StringHashTable * table);

/*
 * int string_hash_table_set(StringHashTable table, char * key, void * data, size_t data_length)
 *
 * function sets a key in the hash table to be equal to data.
 *
 * @params
 * table - StringHashTable instance to be set into
 * key - the key we wish to set
 * data - pointer to the data we're going to store
 * data_length - size of data
 *
 * @returns
 * 1 upon success and -1 else
 */

int string_hash_table_set(StringHashTable * table, char * key, void * data, size_t data_length);


/*
 * void * string_hash_table_get(StringHashTable table, char * key);
 *
 * function retrieves a key from the StringHashTable instance.
 *
 * @params
 * table - StringHashTable instace
 * key - the key to be retrieved
 *
 * @return
 * pointer to data if key exists
 * else returns NULL pointer.
 */
void * string_hash_table_get(StringHashTable * table, char * key);

/*
 * int string_hash_table_get_keys(StringHashTable table, char ** keys);
 *
 * Function returnes all keys hosted in Hash table
 *
 * @params
 * table - StringHashTable instance
 * char ** keys - pointer that will be set to point at and array of keys.
 *
 * @returns
 * keys will be changed to point to a new array (needs to be freed)
 * returns the number of keys in hashtable.
 *
 */

int string_hash_table_get_keys(StringHashTable * table, char *** keys);


#endif /* STRINGHASHTABLE_STRINGHASHTABLE_H_ */
