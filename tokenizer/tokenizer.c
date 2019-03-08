/*
 * tokenizer.c
 *
 *  Created on: Feb 2, 2019
 *      Author: parallels
 */

#include "tokenizer.h"



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
	int i;



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

	for (i=0; i<tokensCount-1; i++) {
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
		if (!(isValidLabel(tokens[0]->string))) {
			printf("ERROR: token \"%s\" is invalid!", tokens[0]->string);
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			return NULL;
		}
		if (tokensCount<2) {
			/* if we have label, we need to make sure line isn't empty */
			returnedSentence->type = EMPTY;
			printf("ERROR: label \"%s\" proceeds an empty line.\n", tokens[0]->string);
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			return NULL;
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

	/**
	 *  HANDLING TOKENS BY COMMAND
	 */

	switch (returnedSentence->type)
	{
		case ENTRY:
		case EXTERNAL:
			if (!(tokensCount=firstTokenIndicator+2)) {
				printf("ERROR: Too many tokens command.\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
			}
			if (!(isValidLabel(tokens[firstTokenIndicator+1]->string))) {
				printf("ERROR: string \"%s\" is not a valid label.\n",tokens[firstTokenIndicator+1]->string);
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				return NULL;
			}

			returnedSentence->operandsCount=1;
			returnedSentence->operands = (Operand **) malloc(sizeof(Operand *));
			if (returnedSentence->operands == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				return NULL;
			}
			returnedSentence->operands[0] = (Operand *) malloc(sizeof(Operand));
			if (returnedSentence->operands[0] == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				return NULL;
			}
			returnedSentence->operands[0]->type = label;
			returnedSentence->operands[0]->label = (char *) malloc(sizeof(char)*strlen(tokens[firstTokenIndicator+1]->string));
			if (returnedSentence->operands[0] == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				return NULL;
			}
			strcpy(returnedSentence->operands[0]->label,tokens[firstTokenIndicator+1]->string);
			break;

		case STRING:
			if (!isValidString(lineCopy,tokens[firstTokenIndicator+1]->startOffset,tokens[tokensCount]->endOffset)) {
				printf("Error: String is invalid. Remember to escape \" using \\ and only have one string in one line.\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				return NULL;
			}

			  debug_print("GOT STRING: \"(%.*s)\"", tokens[tokensCount]->endOffset - tokens[firstTokenIndicator+1]->startOffset, tokens[firstTokenIndicator+1]->startOffset);

			break;
		default:
			printf("Something went terribly wrong, returnedSentence Type can't be handled\n");
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			return NULL;
	}
	/*enum SentenceType {unknownSentence, EMPTY,COMMAND,DATA,STRING,ENTRY*,EXTERNAL*};
	enum CommandType {unknownCommand, mov,cmp,add,sub,not,clr,lea,inc,dec,jmp,bne,red,prn,jsr,rts,stop};
	*/

	freeTokens(tokens, tokensCount);
	free(lineCopy);
	return returnedSentence;
}

void destorySentence(Sentence * sentance) {
	return;
}

