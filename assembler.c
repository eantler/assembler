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

int main(int argc, char ** argv) {
	FILE * fp;
	char cwd[1024];
	char path[1024];
	char * fileName = "test";
	int i;

	if (TEST) {
		info_print("***************** Starting testing ***********");
		linked_list_test();
		string_hash_table_test();
		/*symbols_table_test();*/
		info_print("***************** End testing ***********");
	}

	if (getcwd(cwd, sizeof(cwd)) == NULL) {
		printf("ERROR: failed at getting current work directory");
	};


	if (argc==1 && TEST) {



	strcpy(path,cwd);
	strcat(path,"/");
	strcat(path,fileName);
	strcat(path,".as");

	fp = fopen(path, "r");
	if (fp == NULL) {
		printf("ERROR: failed to open file \"%s\": %s\n",path,strerror(errno));
	}
		assembleFile(fp, path);

	fclose(fp);

	}

	for (i=1; i< argc; i++) {
		strcpy(path,cwd);
		strcat(path,"/");
		strcat(path,argv[i]);
		strcat(path,".as");

		fp = fopen(path, "r");
		if (fp == NULL) {
			printf("ERROR: failed to open file \"%s\": %s\n",path,strerror(errno));
		} else {
			assembleFile(fp, path);

			fclose(fp);
		}

	}





	/*
	int i;
	char testText[] = "  	test: 			   			.data 1,2,3,-677,78,143,12,5666,342,1235343,2\n";
	char emptyText[] = "	not C	 \n";
	Sentence * s;
	s = tokenizeLine(testText);
	if (s != NULL) debug_print("s type: %d,",s->type);
			else debug_print("s is null");

	s = tokenizeLine(emptyText);
		if (s != NULL) debug_print("s type: %d,",s->type);
				else debug_print("s is null");
	Test test[] = {{"hello",3},{"hey",2}};
	for (i = 0; i<2; i++)
	info_print("Test string = %s and int = %d",test[i].a,test[i].b);
	return 1;
	*/
	return 0;
}
