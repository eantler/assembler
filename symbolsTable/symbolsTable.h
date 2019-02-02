/*
 * symbolsTable.h
 *
 * Symbols tables gives a clear and simple symbol management interface (in-memory).
 * Included both table and Symbol API itself
 *
 */

#ifndef SYMBOLSTABLE_H_
#define SYMBOLSTABLE_H_

/* A single Symbol represents a location. It can be entry, external or none of them */
#include "../utilities/utils.h"
#include "../utilities/StringHashTable/StringHashTable.h"

typedef struct Symbol {
	char * label;
	int value;
	int isExtern; /* =1 if true */
	int isEntry; /* =1 if true */
} Symbol;


typedef struct SymbolTable {
	StringHashTable * hashTable;
	int length;
	int lengthExternals;
	int lengthEntries;
} SymbolTable;


/*
 * SymbolTable * create_symbols_table()
 *
 * Function creates and initiate a new SymbolTable
 *
 * @reutrns
 * Pointer to new SymbolTable
 */
SymbolTable * create_symbols_table();

/*
 * void destroy_symbols_table()
 *
 * Function frees all memory related to a symbol table
 *
 */
void destroy_symbols_table(SymbolTable * st);

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
int symbols_table_declare_external(SymbolTable * st, char * label);

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
int symbols_table_declare_entry(SymbolTable * st, char * label);

/*
 * int symbols_table_set_symbol(SymbolTable * st, char * label, int value,int isEntry, int isExtern)
 *
 * Function sets a single symbol by some value
 *
 * @params
 * st - SymbolTable instance
 * label - the symbol label
 * value - value to be stored (-1 if stay with existing)
 * isEntry/isExtern - o or 1 to set a value, -1 to stay with existing
 *
 * @returns
 * pointer to symbol if successful else NULL
 *
 */
Symbol * symbols_table_set_symbol(SymbolTable * st, char * label, int value, int isEntry, int isExtern);

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
Symbol * symbols_table_get_symbol(SymbolTable * st, char * label);

/*
 * int symbols_table_get_symbols(SymbolTable * st, char * label, Symbol ** array)
 *
 * Function gets all the symbols in the table
 *
 * @params
 * st - symbol table
 * array - pointer to an array that will be changed to hold the array of symbols (need to be fried)
 * entriesOnly/externalsOnly - equals 1 if need to retrieve only entries/externals
 *
 * @return
 * count of number of symbols in array, if allocation error than -1
 */

int symbols_table_get_symbols(SymbolTable * st, char * label, Symbol *** array, int entriesOnly, int externalsOnly);


#endif /* SYMBOLSTABLE_H_ */
