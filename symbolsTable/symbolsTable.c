/*
 * symbolsTable.c
 *
 *  See header file for details.
 *
 */


#include "symbolsTable.h"

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

Symbol * symbols_table_get_symbol(SymbolTable * st, char * label){
	Symbol * existingSymbol;
	existingSymbol = (Symbol *) string_hash_table_get(st->hashTable,label);
	if (existingSymbol) return  existingSymbol;

	return NULL;
};


Symbol * symbols_table_set_symbol(SymbolTable * st, char * label, int value, int isEntry, int isExtern){
	Symbol * existingSymbol;
	Symbol newSymbol;
	int setResult;

	if (isEntry > 1 || isEntry < -1) {
		debug_print("Error: isEntry of invalid value for symbol %s", label);
		return NULL;
	}

	if (isExtern > 1 || isExtern < -1) {
		debug_print("Error: isExtern of invalid value for symbol %s", label);
			return NULL;
	}

	if (value < -1 ) {
		debug_print("Error: value parameter of invalid value for symbol %s", label);
		return NULL;
	}
	existingSymbol = symbols_table_get_symbol(st,label);
	if (existingSymbol) {
		existingSymbol->value = value == -1 ? existingSymbol->value : value;
		existingSymbol->isEntry = isEntry == -1 ? existingSymbol->isEntry : isEntry;
		existingSymbol->isExtern = isExtern == -1 ? existingSymbol->isExtern : isExtern;
		if (existingSymbol->isEntry==1 && isEntry==1) st->lengthEntries++;
		if (existingSymbol->isExtern==1 && isExtern==1) st->lengthExternals++;
		return existingSymbol;
	} else {
		newSymbol.isEntry = isEntry == -1 ? 0 : isEntry;
		newSymbol.isExtern = isExtern == -1 ? 0 : isExtern;
		newSymbol.value = value == -1 ? 0 : value;
		newSymbol.label = label;

		setResult = string_hash_table_set(st->hashTable,label,&newSymbol,sizeof(newSymbol));

		if (setResult) {
			st->length++;
			if (isEntry==1) st->lengthEntries++;
			if (isExtern==1) st->lengthExternals++;
			return string_hash_table_get(st->hashTable,label);
		}
	}
	return NULL;
}




/*
 * int symbols_table_get_symbols(SymbolTable * st, Symbol ** array)
 *
 * Function gets all the symbols in the table
 *
 * @params
 * st - symbol table
 * array - pointer to an array pointer that will be changed to hold the array of symbols (need to be fried)
 *
 * @return
 * count of number of symbols in array, if allocation error than -1
 */
int symbols_table_get_symbols(SymbolTable * st, Symbol *** array, int entriesOnly, int externalsOnly ) {
	Symbol ** newArray;
	Symbol * tempSymbol;
	char ** keysArray;
	int hashResValue;
	int allocationLength;
	int i;
	int externalIndex = 0;

	if (entriesOnly == 1 && externalsOnly==1) {
		allocationLength = (st->lengthEntries > st->lengthExternals ? st->lengthEntries : st->lengthExternals);
	} else if (entriesOnly == 1) {
		allocationLength = st->lengthEntries;
	} else if (externalsOnly == 1) {
		allocationLength = st->lengthExternals;
	} else {
		allocationLength = st->length;
	}

	newArray = (Symbol **) malloc(sizeof(Symbol *)*allocationLength);

	if (newArray == NULL ) {
		debug_print("Memory allocation for returned symbols array failed.");
		free(newArray);
		free(keysArray);
		return -1;
	}


	hashResValue = string_hash_table_get_keys(st->hashTable, &keysArray);
	if (hashResValue<0) {
		debug_print("Something went wrong while reading keys from hash table");
		free(newArray);
		free(keysArray);
		return -1;
	}

	for (i=0; i < allocationLength; i++) {
		tempSymbol = (Symbol *) string_hash_table_get(st->hashTable,keysArray[externalIndex]);
		if (tempSymbol == NULL) {
			debug_print("Something went wrong while trying to read key: %s",keysArray[i]);
			free(newArray);
			free(keysArray);
			return -1;
		}

		if (entriesOnly) {
			while (tempSymbol->isEntry != 1) {
				externalIndex++;
				tempSymbol = (Symbol *) string_hash_table_get(st->hashTable,keysArray[externalIndex]);
				if (tempSymbol == NULL) {
					debug_print("Something went wrong while trying to read key: %s",keysArray[i]);
					free(newArray);
					free(keysArray);
					return -1;
				}
			}
		}
		if (externalsOnly) {
			while (tempSymbol->isExtern != 1) {
				externalIndex++;
				tempSymbol = (Symbol *) string_hash_table_get(st->hashTable,keysArray[externalIndex]);
				if (tempSymbol == NULL) {
					debug_print("Something went wrong while trying to read key: %s",keysArray[i]);
					free(newArray);
					free(keysArray);
					return -1;
				}
			}
		}

		newArray[i] = tempSymbol;
		externalIndex++;
		if (newArray[i] == NULL) {
			debug_print("Something went wrong while trying to read key: %s",keysArray[i]);
			free(newArray);
			free(keysArray);
			return -1;
		}
	}
	*array = newArray;
	free(keysArray);
	return allocationLength;
}




