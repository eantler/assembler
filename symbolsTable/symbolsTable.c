/*
 * symbolsTable.c
 *
 *  See header file for details.
 *
 */


#include "symbolsTable.h"
/*
typedef struct {
	char * label;
	int value;
	int isExtern;
	int isEntry;
} Symbol;


typedef struct {
	StringHashTable hashTable;
	int length;
	int lengthExternals;
	int LengthEntries;
} SymbolTable;

*/

SymbolTable * create_symbols_table() {
	debug_print("Starting to create a symbols table");
	SymbolTable * returnedTable;
	returnedTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	if (!returnedTable) {
		/* allocation failed */
		debug_print("Allocation failed.");
		return NULL;
	}

	returnedTable->length = 0;
	returnedTable->lengthEntries = 0;
	returnedTable->lengthExternals = 0;

	returnedTable->hashTable = create_string_hash_table();

	debug_print("Created symbols table successfully");
	return returnedTable;
}

void destroy_symbols_table(SymbolTable * st) {
	destroy_string_hash_table(st->hashTable);
	free(st);
}

Symbol create_symbol();

/*
 * int symbols_table_declare_external(SymbolTable * st, char * label)
 *
 * Function declares an external symbol
 *
 * @params
 * st - SymbolTable instance
 * label - the symbol to be declared
 *
 * @returns
 * 1 if successful, else 0
 */
int symbols_table_declare_external(SymbolTable * st, char * label) {
	return 0;
}

/*
 * int symbols_table_declare_entry(SymbolTable * st, char * label)
 *
 * Function declares an entry symbol
 *
 * @params
 * st - SymbolTable instance
 * label - the symbol to be declared
 *
 * @returns
 * 1 if successful, else 0
 */
int symbols_table_declare_entry(SymbolTable * st, char * label) {
	return 0;
}

/*
 * int symbols_table_set_symbol(SymbolTable * st, char * label, int value)
 *
 * Function sets a single symbol by some value
 *
 * @params
 * st - SymbolTable instance
 * label - the symbol label
 * value - value to be stored
 *
 * @returns
 * pointer to symbol if successful else NULL
 *
 */
Symbol * symbols_table_set_symbol(SymbolTable * st, char * label, int value){
	return NULL;
}

/*
 * Symbol * symbols_table_get_symbol(SymbolTable * st, char * label)
 *
 * Function gets a singly symbol by some value
 *
 * @params
 * st - SymbolTable instance
 * label - the symbol label
 *
 * @returns
 * pointer to the Symbol if successful else NULL;
 *
 */
Symbol * symbols_table_get_symbol(SymbolTable * st, char * label){
	return NULL;
};

/*
 * int symbols_table_get_symbols(SymbolTable * st, char * label, Symbol ** array)
 *
 * Function gets all the symbols in the table
 *
 * @params
 * st - symbol table
 * array - pointer to an array that will be changed to hold the array of symbols
 *
 * @return
 * count of number of symbols in array, if allocation error than -1
 */
int symbols_table_get_symbols(SymbolTable * st, char * label, Symbol ** array) {
	return 0;
}




