
#include "tokenizer.h"


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
	while (**ptr == ' ' || **ptr == '\r' || **ptr== '\t' || **ptr== '\n') {
		(*ptr)++; /* skip blanks */
		continue;
	}
	if (**ptr=='\0') {
		*ptr = NULL;
	}
}

int isValidLabel(char * token) {
	int tokenLength;
	enum CommandType inferCommandType;

	tokenLength = strlen(token);
	if (tokenLength==0 || tokenLength>MAX_LABEL_SIZE)
		return 0;

	/* remove commands names */
	inferCommandType = inferCommandTypeFromToken(token);
	if (inferCommandType !=0 )
		return 0;

	/* remove memory reserved names */
	if (token[0]=='r' && tokenLength > 1 && token[1] >= '0' && token[1]<='9')
		return 0;

	/* startswith A-Za-Z*/
	if (!((token[0]>='a' && token[0]<='z') || (token[0]>='A' && token[0]<='Z')))
		return 0;

	while (*token != '\0') {
		/* A-Za-z0-9 */
		if (!((token[0]>='a' && token[0]<='z') ||
			  (token[0]>='A' && token[0]<='Z') ||
			  (token[0]>='0' && token[0]<='9')))
			return 0;
		token++;
	}
	return 1;

}

int isValidString(char * line, int start, int end) {
	int escapeFlag;


	escapeFlag = 0;
	line+=start+1;
	debug_print("inside isValid String GOT STRING: \"(%.*s)\"", end-start, line);
	if (!(*line=='"' && *(line+end-start)=='"')) return 0;
	while (line < line+end-start) {
		if (*line=='"' && !escapeFlag) return 0;
		escapeFlag=0;
		if (*line=='\\') escapeFlag = 1;
	}
	return 1;
}
