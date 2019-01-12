#include "linkedList.h"
#include "linkedListTester.h"

#include <stdio.h>

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



	char * a = "test";
	int data = 4;

	linked_list_set(testList1,a,(void *)&data,sizeof(data));
	return flag;

}
