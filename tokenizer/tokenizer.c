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
	char * stringStartPtr;
	char * parseCharPtr;
	int stringLength;
	int integerValue;
	int tokensIndex;
	int errFlag = 0;

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
	returnedSentence->operands = NULL;
	returnedSentence->operandsCount = 0;
	returnedSentence->isLabeled = 0;
	returnedSentence->commandType = 0;
	returnedSentence->type = 0;

	/* Checking if we have a label */

	if (tokens[0]->separator == ':') {
		if (!(isValidLabel(tokens[0]->string))) {
			printf("ERROR: token \"%s\" is invalid!", tokens[0]->string);
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			free(returnedSentence);
			return NULL;
		}
		if (tokensCount<2) {
			/* if we have label, we need to make sure line isn't empty */
			returnedSentence->type = EMPTY;
			printf("ERROR: label \"%s\" proceeds an empty line.\n", tokens[0]->string);
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			free(returnedSentence);
			return NULL;
		}
		returnedSentence->isLabeled = 1;
		returnedSentence->label = (char *) malloc(sizeof(char)*strlen(tokens[0]->string));
		strcpy(returnedSentence->label,tokens[0]->string);
		firstTokenIndicator = 1;
	} else {
		returnedSentence->isLabeled = 0;
	}

	/* figuring out which command is that*/

	sentenceTypeInferred = inferSentenceTypeFromToken(tokens[0+firstTokenIndicator]->string);
	if (sentenceTypeInferred == 0) {
		printf("ERROR: Invalid command type \"%s\".\n",0+tokens[0+firstTokenIndicator]->string);
		freeTokens(tokens, tokensCount);
		free(returnedSentence);
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

	/* VERIFYING SEPARATORS */
		for (i=0+firstTokenIndicator; i < tokensCount-1; i++) {
			if (!(
					(tokens[i]->separator == ',' || tokens[i]->separator == '\0')
					||
					(tokens[i]->separator == ' ' && i==0+firstTokenIndicator)
					)) {
				debug_print("TOKEN [%d] separator is %c, which is invlid. token is: %s",i,tokens[i]->separator, tokens[i]->string);
				printf("ERROR: invalid separators format\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}
		}

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
				free(returnedSentence);
			}
			if (!(isValidLabel(tokens[firstTokenIndicator+1]->string))) {
				printf("ERROR: string \"%s\" is not a valid label.\n",tokens[firstTokenIndicator+1]->string);
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}

			returnedSentence->operandsCount=1;
			returnedSentence->operands = (Operand **) malloc(sizeof(Operand *));
			if (returnedSentence->operands == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}
			returnedSentence->operands[0] = (Operand *) malloc(sizeof(Operand));
			if (returnedSentence->operands[0] == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}
			returnedSentence->operands[0]->type = label;
			returnedSentence->operands[0]->label = (char *) malloc(sizeof(char)*strlen(tokens[firstTokenIndicator+1]->string));
			if (returnedSentence->operands[0] == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}
			strcpy(returnedSentence->operands[0]->label,tokens[firstTokenIndicator+1]->string);
			break;





		case STRING:
			if (!isValidString(lineCopy,tokens[firstTokenIndicator+1]->startOffset,tokens[tokensCount-1]->endOffset)) {
				printf("Error: String is invalid. Remember I do not support escaping '\"'.\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}



			stringStartPtr = lineCopy+tokens[firstTokenIndicator+1]->startOffset+1;
			stringLength = tokens[tokensCount-1]->endOffset - tokens[firstTokenIndicator+1]->startOffset-2;

			returnedSentence->operandsCount = stringLength;
			returnedSentence->operands = (Operand **) malloc(sizeof(Operand *)*(stringLength+1));
			if (returnedSentence->operands == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}

			for (i = 0; i < stringLength; i++) {
				returnedSentence->operands[i] = (Operand *) malloc(sizeof(Operand));
				if (returnedSentence->operands[i] == NULL) {
					if (returnedSentence->operands[i] == NULL) {
						printf("Memory allocation failed while tokenizing\n");
						freeTokens(tokens, tokensCount);
						free(lineCopy);
						free(returnedSentence);
						return NULL;
					}
				}

				returnedSentence->operands[i]->type = character;
				returnedSentence->operands[i]->character = *(stringStartPtr+i);
			}

			returnedSentence->operands[stringLength] = (Operand *) malloc(sizeof(Operand));
			if (returnedSentence->operands[stringLength] == NULL) {
				printf("Memory allocation failed while tokenizing\n");
				freeTokens(tokens, tokensCount);
				free(lineCopy);
				free(returnedSentence);
				return NULL;
			}
			returnedSentence->operands[stringLength]->type = character;
			returnedSentence->operands[stringLength]->character = '\0';

			break;






		case DATA:
				/* allocate space needed for array*/

				returnedSentence->operandsCount = tokensCount-firstTokenIndicator-1;
				returnedSentence->operands = (Operand **) malloc(sizeof(Operand*)*returnedSentence->operandsCount+1);
				if (returnedSentence->operands == NULL) {
					printf("Memory allocation failed while tokenizing\n");
					freeTokens(tokens, tokensCount);

					free(returnedSentence);
					return NULL;
				}

				/* allocating space needed for operands */
				 for (i=0; i<returnedSentence->operandsCount; i++) {
					 returnedSentence->operands[i] = (Operand *) malloc(sizeof(Operand));
					if (returnedSentence->operands[i] == NULL) {
						printf("Memory allocation failed while tokenizing\n");
						freeTokens(tokens, tokensCount);
						free(returnedSentence);
						return NULL;
					}
				 }

				/* adding operands as we go */
				for (i=0;i<returnedSentence->operandsCount; i++) {
					tokensIndex = i+firstTokenIndicator+1;
					debug_print("Tring to parse %s",tokens[tokensIndex]->string);
					errno = 0;
					parseCharPtr=0;
					integerValue = (int) strtol(tokens[tokensIndex]->string, &parseCharPtr, 10);
					if (parseCharPtr != (tokens[tokensIndex]->string+strlen(tokens[tokensIndex]->string))) {
						errno = ENOEXEC;
					}

					if (errno != 0) {
						printf("Error while trying to parse \"%s\" as integer: %s.\n", tokens[tokensIndex]->string,strerror(errno));
						freeTokens(tokens, tokensCount);
						freeOperands(returnedSentence->operands, returnedSentence->operandsCount);
						/*
						free(returnedSentence); */
						return NULL;
					}

					debug_print("Parsed first number: %d", integerValue);
					debug_print("Setting returned operand %d to type %d of value %d",i,constant,integerValue);
					returnedSentence->operands[i]->type = constant;
					returnedSentence->operands[i]->value = integerValue;

				}
			break;





		case COMMAND:
			if (returnedSentence->commandType==0) {
				printf("ERROR: Invalid command type. Command type is unknown while command is the type.\n");
				break;
			}
				errFlag = 0;

				if (returnedSentence->commandType >=1 && returnedSentence->commandType<=5) {
					/* first type of commands group */

					if (tokensCount-firstTokenIndicator-1 != 2) {
						printf("Command type %d requires exactly two operands, %d were given.",returnedSentence->commandType,tokensCount-firstTokenIndicator-1);
						printf("Something went wrong while parsing command.\n");
						freeTokens(tokens, tokensCount);
						free(lineCopy);
						free(returnedSentence);
						return NULL;
					}

					returnedSentence->operandsCount=2;
					returnedSentence->operands = (Operand **) malloc(sizeof(Operand *)*2);
					if (returnedSentence->operands == NULL) {
						printf("Couldn't allocate memory while parsing command.\n");
						freeTokens(tokens, tokensCount);
						free(lineCopy);
						free(returnedSentence);
						return NULL;
					}

					returnedSentence->operands[0] = createOperandFromToken(tokens[firstTokenIndicator+1]->string);
					returnedSentence->operands[1] = createOperandFromToken(tokens[firstTokenIndicator+2]->string);
					if (returnedSentence->operands[0] == NULL || returnedSentence->operands[1] == NULL ) {
						errFlag = 1;
					}

				} else if (returnedSentence->commandType >=6 && returnedSentence->commandType <=14) {
					/* second type of command group */

					if (tokensCount-firstTokenIndicator-1 != 1) {
						printf("Command type %d requires exactly one operands, %d were given.",returnedSentence->commandType,tokensCount-firstTokenIndicator-1);
						printf("Something went wrong while parsing command.\n");
						freeTokens(tokens, tokensCount);
						free(lineCopy);
						free(returnedSentence);
						return NULL;
					}

					returnedSentence->operandsCount=1;
					returnedSentence->operands = (Operand **) malloc(sizeof(Operand *));
					if (returnedSentence->operands == NULL) {
						printf("Couldn't allocate memory while parsing command.\n");
						freeTokens(tokens, tokensCount);
						free(lineCopy);
						free(returnedSentence);
						return NULL;
					}

					returnedSentence->operands[0] = createOperandFromToken(tokens[firstTokenIndicator+1]->string);
					if (returnedSentence->operands[0] == NULL) {
						errFlag = 1;
					}


				} else if (returnedSentence->commandType >=15 && returnedSentence->commandType <=16) {
					/* third group of commands group */

					if (tokensCount-firstTokenIndicator-1 != 0) {
						printf("Command type %d requires cannot accepts operands, %d were given.",returnedSentence->commandType,tokensCount-firstTokenIndicator-1);
						printf("Something went wrong while parsing command.\n");

						freeTokens(tokens, tokensCount);
						free(lineCopy);
						free(returnedSentence);
						return NULL;
					}
					returnedSentence->operandsCount=0;
				}
				if (errFlag) {
					printf("Something went wrong while parsing command.\n");
					freeTokens(tokens, tokensCount);
					free(lineCopy);
					free(returnedSentence);
					return NULL;

				}
			break;
		default:
			printf("Something went terribly wrong, returnedSentence Type can't be handled\n");
			freeTokens(tokens, tokensCount);
			free(lineCopy);
			free(returnedSentence);
			return NULL;
	}


	freeTokens(tokens, tokensCount);
	free(lineCopy);
	debug_print_sentence(returnedSentence);
	return returnedSentence;
}

void destorySentence(Sentence * sentence) {
	int i;
	for (i = 0; i < sentence->operandsCount;i++) {
		if(sentence->operands[i]->type == label) {
			free(sentence->operands[i]->label);
		}
		free(sentence->operands[i]);
	}
	if (sentence->operandsCount>0) free(sentence->operands);
	if (sentence->isLabeled) free(sentence->label);
	free(sentence);
	return;
}

