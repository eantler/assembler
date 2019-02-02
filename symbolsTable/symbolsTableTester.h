/*
 * symbolsTabletester.h
 *
 *  Tests basic functionality of symbolsTable
 */

#ifndef SYMBOLSTABLETESTER_H_
#define SYMBOLSTABLETESTER_H_

#define TEST_MAX_SIZE 100 /* Maximum amount of symbols to test */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "symbolsTable.h"
#include "../utilities/utils.h"

/*
 * int symbols_table_test()
 *
 * Core function that runs the test.
 * Returns 1 if everything went smoothly and -1 else.
 *
 */
int symbols_table_test();


#endif /* SYMBOLSTABLETESTER_H_ */
