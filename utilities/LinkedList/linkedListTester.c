#include "linkedListTester.h"


int linked_list_test() {
	int flag = 1;
	LinkedList * testList1;
	char keys[TEST_MAX_SIZE][40];
	int values[TEST_MAX_SIZE];
	int * get_value_results;
	int test_2_flag = 1;
	int i,j;
	int length;
	char ** keysArr;

	info_print("Start testing LinkedList.");
	info_print("Test #1 - Create linkedList");
	testList1 = create_linked_list();
	if (testList1) {
		results_print("TEST #1 - SUCCESS");
	} else {
		results_print("TEST #1 - FAILED");
		flag = -1;
	}


	info_print("Test #2 - Setting %d keys then getting them one by the other.",TEST_MAX_SIZE);
	srand(time(NULL));

	for (i=0; i < TEST_MAX_SIZE ;i++) {
		values[i] = rand();
		sprintf(keys[i],"key%d",i);
		linked_list_set(testList1,keys[i],(void *)&values[i], sizeof(int));
	}

	for (j=TEST_MAX_SIZE-1; j>=0; j--) {
		get_value_results = linked_list_get(testList1,keys[j]);
		if (!(*get_value_results && *get_value_results==values[j])) {
			info_print("Test #2 - failed at key %s.",keys[j]);
			flag = -1;
			test_2_flag= -1;
			results_print("Test #2 - FAILED");
		}
	}

	if (test_2_flag==1) {
		results_print("Test #2 - SUCCESS");
	}

	info_print("Test #3 - Printing the keys");
	length = linked_list_get_keys(testList1, &keysArr);

	if (length<0) {
		debug_print("Failed to get keys");
	}
	debug_print("iterating through the list in order to print it");

	for (i=0;i < TEST_MAX_SIZE; i++) {
		debug_print("i=%d",i);
		debug_print("Key %d: %s",i,keysArr[i]);
	}
	free(keysArr);
	results_print("Print link success");
	destroy_linked_list(testList1);
	results_print("Destroyed linked lists SUCCESS");
	return flag;

}
