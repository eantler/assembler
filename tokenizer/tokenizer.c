/*
 * tokenizer.c
 *
 *  Created on: Feb 2, 2019
 *      Author: parallels
 */

#include "tokenizer.h"

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

Sentence * tokenizeLine(char * line) {

	char * tokenStr;
	char * lineStart;
	char * lineCopy;
	Sentence * returnedSentence;
	enum SentenceType sentenceTypeInferred;
	Token * token;
	Token * tokens[MAX_TOKENS_PER_LINE];
	int tokensCount = 0;
	int firstTokenIndicator = 0;



	debug_print("Tokenizer read line: \"%s\"", line);

	/* Trimming blanks at start of line */
	lineStart = line;
	skipBlanks(&lineStart);
	debug_print("Line after skipping blanks at beginning: \"%s\"", lineStart);


	/* handle empty line */
	if (lineStart == NULL) {
		debug_print("Empty line recognized, returning blankSentence");
		returnedSentence = (Sentence *) malloc(sizeof(Sentence));
		if (returnedSentence == NULL) {
			printf("Failed to allocate memory while tokenizing line.\n");
			return NULL;
		}
		returnedSentence->type = EMPTY;
		return returnedSentence;
	}

	/* copying the line because strtok alters it */
	lineCopy = (char *) malloc(sizeof(char)*strlen(lineStart));
	if (lineCopy==NULL) {
		printf("Failed to allocate memory while tokenizing line.\n");
		return NULL;
	}
	strcpy(lineCopy,lineStart);

	/* Reading tokens one after the other. If we reach MAX TOKENS PER LINE return NULL */
	tokenStr = strtok(lineStart,DELIMS);
	while (tokenStr != NULL) {
		/* Verifying token length */
		if (strlen(tokenStr)>MAX_TOKEN_SIZE) {
			printf("Found a token too long! Maximum token size is: %d.\n Token was: \"%s\"",MAX_TOKEN_SIZE,tokenStr);
			free(lineCopy);
			return NULL;
		}
		/* allocating space for new token and pushing it in to the list */
		token = (Token *) malloc(sizeof(Token));
		if (token == NULL ) {
			printf("Failed to allocate memory while tokenizing line.\n");
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			return NULL;
		}
		strcpy(token->string,tokenStr);
		token->startOffset = tokenStr - lineStart;
		token->endOffset = tokenStr - lineStart + strlen(tokenStr);
		tokens[tokensCount] = token;
		tokensCount++;
		/* Handle too many tokens */
		if (tokensCount>MAX_TOKENS_PER_LINE) {
			printf("Maximum tokens per line is %d. Too many tokens for line: %s\n", MAX_TOKENS_PER_LINE,lineStart);
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			return NULL;
		}
		tokenStr = strtok(NULL,DELIMS);
	}

	for (int i=0; i<tokensCount-1; i++) {
		/* adding separators for tokens */
		tokens[i]->separator = getSeparator(lineCopy, tokens[i]->endOffset, tokens[i+1]->startOffset); /* using copy because original string is altered */
		if (tokens[i]->separator == '\0') {
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			return NULL;
		}
	}

	/*
	 *  Processing returned Sentence according to logic
	 */

	returnedSentence = (Sentence *) malloc(sizeof(Sentence));
	if (returnedSentence == NULL ) {
		printf("Failed to allocate memory while tokenizing line.\n");
		freeTokens(tokens, tokensCount);
		free(lineCopy);
		return NULL;

	}

	/* Checking if we have a label */

	if (tokens[0]->separator == ':') {
		debug_print("separator: %c",tokens[0]->separator);
		if (tokensCount<2) {
			/* if we have label, we need to make sure line isn't empty */
			returnedSentence->type = EMPTY;
			printf("WARNING: label \"%s\" proceeds an empty line.\n", tokens[0]->string);
			free(lineCopy);
			return returnedSentence;
		}
		returnedSentence->isLabeled = 1;
		returnedSentence->label = (char *) malloc(sizeof(char)*strlen(tokens[0]->string));
		strcpy(returnedSentence->label,tokens[0]->string);
		firstTokenIndicator = 1;
	} else {
		returnedSentence->isLabeled = 0;
		freeTokens(tokens, tokensCount);
		free(lineCopy);
		return NULL;
	}

	/* figuring out which command is that*/

	sentenceTypeInferred = inferSentenceTypeFromToken(tokens[0+firstTokenIndicator]->string);
	if (sentenceTypeInferred == 0) {
		printf("ERROR: Invalid command type \"%s\".\n",0+tokens[0+firstTokenIndicator]->string);
		freeTokens(tokens, tokensCount);
		free(lineCopy);
		return NULL;
	}

	returnedSentence->type = sentenceTypeInferred;
	if (sentenceTypeInferred == COMMAND) {
		returnedSentence->commandType = inferCommandTypeFromToken(tokens[0+firstTokenIndicator]->string);
	} else {
		returnedSentence->commandType = 0;
	}

	debug_print("Sentence type %d, command type %d", returnedSentence->type,returnedSentence->commandType);

	/* making sure we have enough tokens for that command */

	//enum SentenceType {unknownSentence, EMPTY,COMMAND,DATA,STRING,ENTRY,EXTERNAL};
	//enum CommandType {unknownCommand, mov,cmp,add,sub,not,clr,lea,inc,dec,jmp,bne,red,prn,jsr,rts,stop};

	freeTokens(tokens, tokensCount);
	free(lineCopy);
	return returnedSentence;
}

void destorySentence(Sentence * sentance) {
	return;
}

/*
 * INTERNAL HELPING FUNCTIONS
 */

/* helping function that returns sentence type from token */
enum SentenceType inferSentenceTypeFromToken(char * token) {
	enum CommandType commandInferred;
	commandInferred = inferCommandTypeFromToken(token);
	if (commandInferred != 0) {
		return COMMAND;
	} else if (strcmp(token, ".data") == 0) {
		  return DATA;
	} else if (strcmp(token, ".string") == 0) {
		  return STRING;
	} else if (strcmp(token, ".entry") == 0) {
		  return ENTRY;
	} else if (strcmp(token, ".extern") == 0) {
		  return EXTERNAL;
	}

	return 0;
}

/* helping function that returns command type from token */
enum CommandType inferCommandTypeFromToken(char * token) {

	if (strlen(token) > 5) return 0;

	if (strcmp(token, "mov") == 0)
		{
		  return mov;
		}
	else if (strcmp(token, "cmp") == 0)
		{
		  return cmp;
		}

	else if (strcmp(token, "add") == 0)
		{
		  return add;
		}
	else if (strcmp(token, "sub") == 0)
		{
		  return sub;
		}
	else if (strcmp(token, "not") == 0)
		{
		  return not;
		}
	else if (strcmp(token, "clr") == 0)
		{
		  return clr;
		}
	else if (strcmp(token, "lea") == 0)
		{
		  return lea;
		}
	else if (strcmp(token, "inc") == 0)
		{
		  return inc;
		}
	else if (strcmp(token, "dec") == 0)
		{
		  return dec;
		}
	else if (strcmp(token, "jmp") == 0)
		{
		  return jmp;
		}
	else if (strcmp(token, "bne") == 0)
		{
		  return bne;
		}
	else if (strcmp(token, "red") == 0)
		{
		  return red;
		}
	else if (strcmp(token, "prn") == 0)
		{
		  return prn;
		}
	else if (strcmp(token, "jsr") == 0)
		{
		  return jsr;
		}
	else if (strcmp(token, "rts") == 0)
		{
		  return rts;
		}
	else if (strcmp(token, "stop") == 0)
		{
		  return stop;
		}
	return 0;

}


/* helping function that looks for the separator between tokens.
 * if more than 1 separator then throws an error and return '\0'
 */
char getSeparator(char * string, int startOffSet, int endOffSet) {
	char * pointer;
	char separator = ' ';
	pointer  = string + startOffSet;
	while (pointer < string+endOffSet) {
		if (*pointer == ' ' || *pointer == '\r' || *pointer== '\t') {
			pointer++; /* skip blanks */
			continue;
		}
		if (separator != ' ') {
			printf("Too many separators! Saw unexpected %c but already had %c",*pointer,separator);
			return '\0';
		} else {
			separator = *pointer;
		}
		pointer++;
	}

	return separator;
}

void freeTokens(Token ** list, int length) {
	int i;
	for (i=0;i<length;i++) {
		free(list[i]);
	}

}

void skipBlanks(char ** ptr) {
	while (**ptr == ' ' || **ptr == '\r' || **ptr== '\t') {
		(*ptr)++; /* skip blanks */
		continue;
	}
	if (**ptr=='\0') {
		*ptr = NULL;
	}
}
