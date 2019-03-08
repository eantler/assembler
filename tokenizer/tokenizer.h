/*
 * tokenizer.h
 *
 * Provides methods for parsing a line of test and returns a structured command object
 * The actual reading of the strings is done elsewhere.
 *
 */


#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "../utilities/utils.h"
#include <stdio.h>
#include <string.h>

/* Restrictions */
#define MAX_LINE_SIZE 80
#define MAX_LABEL_SIZE 31
#define DELIMS " :,\r\t\n"
#define MAX_TOKENS_PER_LINE 100
#define MAX_TOKEN_SIZE 80
/*Operand structure represents an operand in the command */
enum OperandType {registerNumber, label, constant, character};

typedef struct {
	char string[MAX_TOKEN_SIZE];
	int startOffset;
	int endOffset;
	char separator;
} Token;

typedef struct {
	enum OperandType type;
	char * label;
	int value;
} Operand;

/*
 * Sentence structure represents a single command made in code
 * if the sentence type has special fields they are also encoded in
 *
 */
enum SentenceType {unknownSentence, EMPTY,COMMAND,DATA,STRING,ENTRY,EXTERNAL};
enum CommandType {unknownCommand, mov,cmp,add,sub,not,clr,lea,inc,dec,jmp,bne,red,prn,jsr,rts,stop};

typedef struct {
	enum SentenceType type;
	int isLabeled;
	char * label;
	enum CommandType commandType;
	int operandsCount;
	Operand ** operands;
} Sentence;

/*
 * Sentence * tokenizeLine(char * line)
 *
 *  receives a sentence line as input
 * prints warning if any
 *
 * @Returns null if tokenization failed.
 * Otherwise returns a pointer to a tokenized Sentence
 */
Sentence * tokenizeLine(char * line);

/*
 * void destorySentence(Sentence * sentance)
 * Used to destoroy sentences.
 */
void destorySentence(Sentence * sentance);

/*
 * INTERNAL HELPING FUNCTIONS
 */

/* helping function
 * skip blanks move the pointer after blank spaces. Sets pointer to null if line is empty
*/
void skipBlanks(char ** ptr);

/* helping function that looks for the separator between tokens.
 * if more than 1 separator then prints an error and return '\0'
 */
char getSeparator(char * string, int startOffSet, int endOffSet);

/* helping function that frees a list of tokens */
void freeTokens(Token ** list, int length);

/* helping function that returns sentence type from token */
enum SentenceType inferSentenceTypeFromToken(char * token);

/* helping function that returns command type from token */
enum CommandType inferCommandTypeFromToken(char * token);

/* helping function that checks if a token is a valid label */
int isValidLabel(char * token);

/* helping function that checks if we look at valid strings */
int isValidString(char * line, int start, int end);

#endif /* TOKENIZER_H_ */
