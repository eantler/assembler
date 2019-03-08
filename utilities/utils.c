/*
 * utils.c
 *
 * Contains helping functions that will be used across files
 *
 */

#include "utils.h"
#include <stdlib.h>
#include <time.h>

void debug_print(const char* format, ...) {
	va_list arglist;
	if (DEBUG) {
		printf("[DEBUG]: ");
		  va_start( arglist, format );
		  vprintf( format, arglist );
		  va_end( arglist );
		  printf("\n");
	} else {
		return;
	}
}


void info_print(const char* format, ...) {
	va_list arglist;
	if (DEBUG || TEST) {
		printf("[INFO]: ");
		  va_start( arglist, format );
		  vprintf( format, arglist );
		  va_end( arglist );
		  printf("\n");
	} else {
		return;
	}
}

void results_print(const char* format, ...) {
	va_list arglist;
	if (DEBUG || TEST) {
		printf("[RESULTS]: ");
		  va_start( arglist, format );
		  vprintf( format, arglist );
		  va_end( arglist );
		  printf("\n");
	} else {
		return;
	}
}

char *rand_string(char *str, size_t size) {
	/* this test function is taken from https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c */
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}
