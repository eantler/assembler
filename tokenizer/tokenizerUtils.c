
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
	debug_print("getSeparator called with string \"%s\" startOffSet = %d and endOffSet = %d",string,startOffSet,endOffSet);
	while (pointer < string+endOffSet && *pointer != '\0') {
		if (*pointer == ' ' || *pointer == '\r' || *pointer== '\t' ) {
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
	debug_print("Got separator string \"%.*s\". Returned separator: %c, pointer = %c", endOffSet-startOffSet, string + startOffSet, separator, *pointer);

	return separator;
}

/* frees a list of tokens */
void freeTokens(Token ** list, int length) {
	int i;
	for (i=0;i<length;i++) {
		free(list[i]);
	}

}

void freeOperands(Operand ** list, int length) {
	int i;
	for (i=0;i<length;i++) {
		if (list[i]->type != label ) {
			free(list[i]);
		} else {
			free(list[i]->label);
			free(list[i]);
		}

	}

}

/* move ptr past blanks */
void skipBlanks(char ** ptr) {
	while (**ptr == ' ' || **ptr == '\r' || **ptr== '\t' || **ptr== '\n') {
		(*ptr)++; /* skip blanks */
		continue;
	}
	if (**ptr=='\0') {
		*ptr = NULL;
	}
}

/* checks a token for if its a valid label */
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

/* checks a segment of line to see if that's a valid string */
int isValidString(char * line, int start, int end) {
	int escapeFlag;
	int length;
	char * linePtr;
	escapeFlag = 0;
	linePtr=line+start+1;
	length=end-start-2;
	debug_print("inside isValid String GOT STRING: \"%.*s\"", length, linePtr);
	debug_print("first char: %c, last char: %c", *(linePtr-1), *(linePtr+length));
	if (!(*(linePtr-1)=='"' && *(linePtr+length)=='"')) return 0;
	line+=start+1;
	while (line < linePtr+length) {
		debug_print("Checking pointer at: %p, char is %c",line,*line);
		if (*line=='"' && !escapeFlag) return 0;
		escapeFlag=0;
		if (*line=='\\') escapeFlag = 1;
		line++;
	}
	debug_print("Returning\n");
	return 1;
}

Operand * createOperandFromToken(char * token) {
	/* character is set manually by tokenizer, so can't be infered from token */
	Operand * returnedOperand;
	int integerValue;
	char * charPtr;

	returnedOperand  = (Operand *) malloc(sizeof(Operand));
	if (returnedOperand == NULL ) {
		printf("ERROR: couldn't allocate memory to create new operand.\n");
		return NULL;
	}

	/* valid registerNumber */
	if (token[0] == '@' && strlen(token) == 3) {
		if (token[1] == 'r' && isdigit(token[2]) && token[2] < ('0'+NUM_OF_REGISTERES)) {
			returnedOperand->type = registerNumber;
			returnedOperand->value = (int) (token[2] - '0');
			return returnedOperand;
		}
	}
	/* check if it's a valid label */
	if (isValidLabel(token)) {
		returnedOperand->type = label;
		returnedOperand->label = (char *) malloc(sizeof(char)*strlen(token)+1);
		if (returnedOperand == NULL) {
			printf("ERROR: couldn't allocate memory to create new operand.\n");
			return NULL;
		}
		strcpy(returnedOperand->label,token);
		return returnedOperand;
	}

	/* valid number */
	errno = 0;
	integerValue = strtol(token, &charPtr, 10);
	debug_print("Integer value == %d",integerValue);
	debug_print("errno == %d",errno);
	debug_print("charPtr at end == %d",charPtr-token==strlen(token));
	if (charPtr-token==strlen(token)) {
		returnedOperand->type = constant;
		returnedOperand->value = integerValue;
		return returnedOperand;
	}

	printf("ERROR: couldn't resolve \"%s\" into a valid operand.\n",token);
	free(returnedOperand);
	return NULL;
}

void debug_print_sentence(Sentence * sen) {
	int i;
	debug_print("Sentence Type: %d, Command Type: %d",sen->type,sen->commandType);
	if (sen->isLabeled) {
		debug_print("A labeled sentence with label %s",sen->label);
	}
	debug_print("It had %d operands:", sen->operandsCount);
	for (i=0;i<sen->operandsCount;i++) {
		debug_print("Operand #%d of type %d",i,sen->operands[i]->type);
		if (sen->operands[i]->type == registerNumber) {
			debug_print("register number #%d",sen->operands[i]->value);
		}
		if (sen->operands[i]->type == character) {
			debug_print("char '%c'",sen->operands[i]->character);
		}
		if (sen->operands[i]->type == constant) {
			debug_print("constant = %d",sen->operands[i]->value);
		}
		if (sen->operands[i]->type == label) {
			debug_print("label: %s",sen->operands[i]->label);
		}

	}
	debug_print("Sentence End");
}



