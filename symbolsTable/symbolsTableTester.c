#include "symbolsTableTester.h"
/* _getIndexWhereLabel returns the index of the symbol array where label equals givel label
 * simple scan just as a test helping function
*/
int _getIndexWhereLabel(char array[TEST_MAX_SIZE][STRING_TEST_SIZE], char * label) {
	int i;
	int strcmpValue;

	for (i=0; i<TEST_MAX_SIZE; i++) {
		strcmpValue = strcmp(array[i],label);
		if ( strcmpValue == 0)  {
			return i;
		}
	}
	return -1;
}

int symbols_table_test() {
	char labels[TEST_MAX_SIZE][STRING_TEST_SIZE];
	int values[TEST_MAX_SIZE];
	int isEntries[TEST_MAX_SIZE];
	int isExternals[TEST_MAX_SIZE];
	int i;
	int externalsCount = 0;
	SymbolTable * testSymbolTable;
	Symbol * symbolTemp;
	Symbol ** symbolsList;
	int returnedSymbolsListLength;
	int arrayIndex;

	info_print("Starting symbols table tests");

	/* generate random values **/
	debug_print("Generating %d random symbols", TEST_MAX_SIZE);
	srand(time(NULL));
	for (i=0; i<TEST_MAX_SIZE; i++) {
		rand_string(labels[i],STRING_TEST_SIZE);
		values[i] = rand();
		isEntries[i] = rand() % 2;
		isExternals[i] = rand() % 2;
		if (isExternals[i]) externalsCount ++;
	}

	info_print("Creating new symbols table and inserting symbols into it");
	testSymbolTable = create_symbols_table();

	if (testSymbolTable==NULL) {
		debug_print("ERROR: Something went wrong while creating symbols table");
		return 0;
	}

	for (i=0; i<TEST_MAX_SIZE; i++) {
		debug_print("Inserting symbol \"%s\" into the table.", labels[i]);
		symbols_table_set_symbol(testSymbolTable,labels[i],values[i],isEntries[i],isExternals[i], 0);
		debug_print("Getting symbol \"%s\" from the table.", labels[i]);
		symbolTemp = symbols_table_get_symbol(testSymbolTable,labels[i]);
		if (!(
				symbolTemp != NULL &&
				symbolTemp->isEntry == isEntries[i] &&
				symbolTemp->isExtern == isExternals[i] &&
				symbolTemp->value == values[i] &&
				strcmp(symbolTemp->label,labels[i]) == 0

				)) {
			debug_print("ERROR: something went wrong while retrieving the symbol");
			return 0;
		} else {
			debug_print("Got symbol correctly");
		}
	}

	info_print("Getting back symbols list");
	returnedSymbolsListLength = symbols_table_get_symbols(testSymbolTable, &symbolsList ,0,0, 0);
	if (returnedSymbolsListLength != TEST_MAX_SIZE) {
		debug_print("ERROR: Didn't get enough symbols back");
		return 0;
	}

	for (i=0; i<TEST_MAX_SIZE; i++) {
		symbolTemp=symbolsList[i];

		arrayIndex = _getIndexWhereLabel(labels,symbolTemp->label);
		if (arrayIndex<0) {
			debug_print("ERROR: Didn't find the string in my test array");
			return 0;
		}
		if (!(
				symbolTemp != NULL &&
				symbolTemp->isEntry == isEntries[arrayIndex] &&
				symbolTemp->isExtern == isExternals[arrayIndex] &&
				symbolTemp->value == values[arrayIndex] &&
				strcmp(symbolTemp->label,labels[arrayIndex]) == 0
				)) {
			debug_print("ERROR: something went wrong while comparing retrieved symbol");
						return 0;

		} else {
			debug_print("Retrieved symbol %s successfully.", symbolTemp->label);
		}

	}
	free(symbolsList);


	info_print("Testing for only externals");
	returnedSymbolsListLength = symbols_table_get_symbols(testSymbolTable, &symbolsList ,0,1, 0);
	if (returnedSymbolsListLength != externalsCount) {
		debug_print("ERROR: Didn't get enough symbols back");
		return 0;
	}

	for (i=0; i<returnedSymbolsListLength; i++) {
		symbolTemp=symbolsList[i];
		arrayIndex = _getIndexWhereLabel(labels,symbolTemp->label);
		if (arrayIndex<0) {
			debug_print("ERROR: Didn't find the string in my test array");
			return 0;
		}
		if (!(
				symbolTemp != NULL &&
				symbolTemp->isExtern == isExternals[arrayIndex]
				)) {
			debug_print("ERROR: something went wrong while comparing retrieved symbol");
						return 0;

		} else {
			debug_print("Retrieved symbol #%d: %s successfully.", arrayIndex, symbolTemp->label);
		}

	}

	free(symbolsList);
	destroy_symbols_table(testSymbolTable);


	results_print("Symbols Table test completed successfully");
	return 1;
}
