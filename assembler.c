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
	int i;
	char testText[] = "  	test: 			   			.string   	 \"Hola\"   			\n";
	char emptyText[] = "		 \n";
	Sentence * s;
	s = tokenizeLine(testText);
	if (s != NULL) debug_print("s type: %d,",s->type);
			else debug_print("s is null");
	tokenizeLine(emptyText);
	Test test[] = {{"hello",3},{"hey",2}};
	for (i = 0; i<2; i++)
	info_print("Test string = %s and int = %d",test[i].a,test[i].b);
	return 1;

}
