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
#include "../LinkedList/LinkedList.h" // Hash table is using the linked list for its nodes

typedef struct {
	LinkedList table[HASH_TABLE_SIZE];
	int length;
	int interatorPointer;
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
void destroy_string_hash_table(StringHashTable table);

/*
 * int string_hash_table_set(StringHashTable table, char * key, void * data, size_t data_length)
 *
 */
int string_hash_table_set(StringHashTable table, char * key, void * data, size_t data_length);

void * string_hash_table_get(StringHashTable table, char * key);

int string_hash_table_get_keys(StringHashTable table, char ** keys);


#endif /* STRINGHASHTABLE_STRINGHASHTABLE_H_ */
