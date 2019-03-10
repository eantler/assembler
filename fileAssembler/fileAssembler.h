/*
 * fileAssembler.h
 *
 *  Created on: Mar 9, 2019
 *      Author: parallels
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "../utilities/utils.h"
#include "../symbolsTable/symbolsTable.h"
#include "../tokenizer/tokenizer.h"

#ifndef FILEASSEMBLER_FILEASSEMBLER_H_

#define START_CODE_ROW 100
/**
 * Assemble file
 *
 * This is the main function that recieve a file then assembles it
 * and created an ob, ext, and ent files.
 *
 * Returns non-zero on failure
 */

int assembleFile(FILE * file, char * path);

/*
 * Computes the first code word binary representation and returns number of code words that would be consumed by this sentence
 */
int computeBinarySentence(Sentence * sentence, unsigned int * binaryWord);

/*
 * HELPING FUNCTIONS
 */

/* prints binary 12 first words */
void debug_binary_print(unsigned int word);

/* prints binary word to file */
void print_binary_word_to_file (FILE * f, unsigned int * word);

/* returns the miun type based on operand */
int inferMiunFromOperand (Operand o);



#define FILEASSEMBLER_FILEASSEMBLER_H_



#endif /* FILEASSEMBLER_FILEASSEMBLER_H_ */
