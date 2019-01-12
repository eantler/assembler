#include "linkedList.h"
#include "linkedListTester.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define info_print(fmt, ...) \
        do { fprintf(stdout, " %s:%d:%s() [INFO]: " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__ ); \
		} while (0)
#define results_print(fmt, ...) \
        do { fprintf(stdout, " %s:%d:%s() [RESULT]: " fmt, __FILE__, \
                                __LINE__, __func__, ##__VA_ARGS__ ); \
		} while (0)

int linked_list_test() {
	int flag = 1;
	info_print("Start testing LinkedList.\n");

	info_print("Test #1 - Create linkedList\n");
	LinkedList * testList1;
	testList1 = create_linked_list();
	if (testList1) {
		results_print("TEST #1 - SUCCESS\n");
	} else {
		results_print("TEST #1 - FAILED\n");
		flag = -1;
	}


	info_print("Test #2 - Setting %d keys then getting them one by the other.\n",TEST_MAX_SIZE);
		srandom(time(NULL));

		char keys[TEST_MAX_SIZE][40];
		int values[TEST_MAX_SIZE];
		int * get_value_results;
		int test_2_flag = 1;

		for (int i=0; i < TEST_MAX_SIZE ;i++) {
			values[i] = random();
			sprintf(keys[i],"key%d",i);
			linked_list_set(testList1,keys[i],(void *)&values[i], sizeof(int));
		}

		for (int j=TEST_MAX_SIZE-1; j>=0; j--) {
			get_value_results = linked_list_get(testList1,keys[j]);
			if (!(*get_value_results && *get_value_results==values[j])) {
				info_print("Test #2 - failed at key %s.\n",keys[j]);
				flag = -1;
				test_2_flag= -1;
				results_print("Test #2 - FAILED\n");
			}
		}

		if (test_2_flag==1) {
			results_print("Test #2 - SUCCESS\n");
		}

	info_print("Test #3 - Seeting up 100 keys with objects then getting them one by the other.\n");

	destroy_linked_list(testList1);
	results_print("Destroyed linked lists SUCCESS\n");
	return flag;

}
