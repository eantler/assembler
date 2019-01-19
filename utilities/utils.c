/*
 * utils.c
 *
 * Contains helping functions that will be used across files
 *
 */

#include "utils.h"

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
