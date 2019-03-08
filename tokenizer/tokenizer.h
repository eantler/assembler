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
#define MAX_LABEL_SIZE 30
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
	char *label;
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
	Operand operands[];
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

#endif /* TOKENIZER_H_ */
