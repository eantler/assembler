/*
 * assember.c
 *
 * Implementation of final exercise 14 in course 20465
 * Student: 201194495
 *
 * This program implements an assembler based on the specifications provided for this excersize.
 *
 * Usage after make: assembler file.as
 *
 */

#include "assembler.h"

typedef struct {
	char * a;
	int b;
} Test;

int main() {

	printf("Hello world!\n");
	debug_print("DONEEEE#@(&$#HKJLHFD");
	if (TEST) {
		info_print("***************** Starting testing ***********");
		linked_list_test();
		string_hash_table_test();
		/*symbols_table_test();*/
		info_print("***************** End testing ***********");
	}
	char testText[] = "  	test: 			   			bne   	 3   			, @r4  		\n";
	char emptyText[] = "		 ";
	tokenizeLine(testText);
	tokenizeLine(emptyText);
	Test test[] = {{"hello",3},{"hey",2}};
	for (int i = 0; i<2; i++)
	info_print("Test string = %s and int = %d",test[i].a,test[i].b);
	return 1;

}
