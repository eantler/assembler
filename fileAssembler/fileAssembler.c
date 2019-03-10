#include "fileAssembler.h"

int assembleFile(FILE * file, char * path) {

	int IC;
	int DC;
	int i;
	int LINE = 0;
	int readlineValue;
	int errorFlag = 0;
	int length;

	int getSymbolsValue;
	int fseekValue;
	void * address;
	char * line = NULL;
	char buffer[128];
	char buffer2[128];
	char ** keys;
	int twoRegistersFlag;
	FILE * writeFile;
	FILE * externalFile = NULL;
	size_t len = 0;
	SymbolTable * st;
	LinkedList * dataWords;
	LinkedList * commandsWords;
	Symbol * symbol;
	Symbol ** symbolsArray;
	Sentence * sentence;
	unsigned int binaryWord;
	unsigned int binaryWord2;
	unsigned int binaryWord3;


	st = create_symbols_table();
	dataWords = create_linked_list();
	commandsWords = create_linked_list();


	readlineValue = getline(&line, &len, file);

	/*
	 * enum SentenceType {unknownSentence, EMPTY,COMMAND,DATA,STRING,ENTRY,EXTERNAL};
	 */

	/**
	 *  FIRST ITERATION
	 */

	DC = 0;
	IC = 0;

	while (readlineValue != -1) {
		LINE++;
		if (line[0]==';') {
			readlineValue = getline(&line, &len, file);
			continue;
		}
		sentence = tokenizeLine(line);
		if (sentence == NULL ) {
			printf("ERROR: something went wrong while consuming line %d\n",LINE);
			errorFlag = 1;

		} else {

			switch (sentence->type) {
				case DATA:
				case STRING:
					if (sentence->isLabeled) {
						symbol = symbols_table_get_symbol(st,sentence->label);
						if (symbol != NULL) {
							printf("ERROR: symbol '%s' already exists in table\n",sentence->label);
							errorFlag=1;
						}
						symbol = symbols_table_set_symbol(st, sentence->label,DC, 0, 0, 1);
						if (symbol == NULL) {
							printf("ERROR: something went wrong while setting symbols table. line: %d\n", LINE);
							errorFlag=1;
						}
					}
					DC += sentence->operandsCount;
					for (i=0; i<sentence->operandsCount; i++) {
						debug_print("Sentence data:");
						debug_binary_print((unsigned int)sentence->operands[i]->value);
						if (sentence->type==STRING) {
							address = &sentence->operands[i]->character;
						} else {
							address = &sentence->operands[i]->value;
						}
						sprintf(buffer,"data%d%d",LINE,i);
						debug_print("WRITING DATA KEY %s with value %d",buffer,sentence->operands[i]->value);
						if (linked_list_set(dataWords,buffer, address,sizeof(unsigned int)) == -1) {
							printf("ERROR: something went wront while writing data words to memory.\n");
							errorFlag = 1;
						}

					}
					if (sentence->type==STRING) {
						i=0;
						DC++;
						sprintf(buffer,"data%dEndOfString",LINE);
							if (linked_list_set(dataWords,buffer, &i,sizeof(unsigned int)) == -1) {
								printf("ERROR: something went wront while writing data words to memory.\n");
								errorFlag = 1;
						}
					}
					break;
				case EXTERNAL:
				case ENTRY:
					if (sentence->isLabeled) {
						printf("WARNING: external/entry commands shouldn't have a label, it's meaningless.\n");
					}
					if (sentence->type == EXTERNAL) {
							symbol = symbols_table_get_symbol(st,sentence->operands[0]->label);
							if (symbol != NULL) {
								printf("ERROR: external symbol '%s' already exists in table\n",sentence->label);
								errorFlag=1;
							}
							symbol = symbols_table_set_symbol(st, sentence->operands[0]->label,IC+START_CODE_ROW, 0, 1, 0);
							if (symbol == NULL) {
								printf("ERROR: something went wrong while setting symbols table. line: %d\n", LINE);
								errorFlag=1;
							}
					}
					break;
				case COMMAND:
					if (sentence->isLabeled) {
						symbol = symbols_table_get_symbol(st,sentence->label);
						if (symbol != NULL) {
							printf("ERROR: symbol '%s' already exists in table\n",sentence->label);
							errorFlag=1;
						}
						symbol = symbols_table_set_symbol(st, sentence->label,IC, 0, 0, 0);
						if (symbol == NULL) {
							printf("ERROR: something went wrong while setting symbols table. line: %d\n", LINE);
							errorFlag=1;
						}
					}

					IC += computeBinarySentence(sentence, &binaryWord);
					debug_binary_print(binaryWord);
				break;
				case EMPTY:
					debug_print("Empty line, moving on. LINE: %d",LINE);
					break;
				case unknownSentence:
					debug_print("Unknown sentence line, something is wrong. LINE: %d",LINE);
					errorFlag = 1;
					break;
				default:
					break;

			}
		}

		if (errorFlag) {
			printf("** PLEASE FIX MISTAKES IN SOURCE FILE AND REASSEMBLE **\n");
			destroy_symbols_table(st);
			destroy_linked_list(dataWords);
			destroy_linked_list(commandsWords);
			return 0;
		}



		readlineValue = getline(&line, &len, file);
	}

	/* now updating data values */
	getSymbolsValue = symbols_table_get_symbols(st,&symbolsArray,0,0,1);
	for (i=0; i<getSymbolsValue; i++) {
		symbols_table_set_symbol(st,symbolsArray[i]->label,symbolsArray[i]->value+IC,-1,-1,-1);
	}

	free(symbolsArray);

	/**
	 *  SECOND ITERATION
	 */

	IC = 0;
	LINE = 0;
	fseekValue = fseek(file,0,SEEK_SET);

	if (fseekValue!=0) {
		printf("ERROR: something went wrong while reading from file.\n");
		destroy_symbols_table(st);
		destroy_linked_list(dataWords);
		destroy_linked_list(commandsWords);
		return 0;
	}

	readlineValue = getline(&line, &len, file);

	while (readlineValue != -1) {
		LINE++;
		if (line[0]==';') {
			readlineValue = getline(&line, &len, file);
			continue;
		}
		sentence = tokenizeLine(line);
			if (sentence == NULL ) {
				printf("ERROR: something went wrong while consuming line %d\n",LINE);
				errorFlag = 1;

			} else {

				switch (sentence->type) {
					case EXTERNAL:
					case STRING:
					case DATA:
						break;
					case ENTRY:
							if (sentence->operandsCount==0 || sentence->operands[0]->type != label) {
								printf("ERROR: Entry operand should be label");
								errorFlag = 1;
							}
							symbol = symbols_table_get_symbol(st,sentence->operands[0]->label);
							if (symbol == NULL ) {
								printf("ERROR: entry symbol \"%s\" wasn't found.\n",sentence->operands[0]->label);
								errorFlag = 1;
							} else {
								symbol = symbols_table_set_symbol(st,sentence->operands[0]->label,-1,1,-1,-1);
								if (symbol == NULL) {
									printf("ERROR: something went wrong while trying to mark label \"%s\" as symbol.\n",sentence->operands[0]->label);
									errorFlag = 1;
								}
							}
						break;
					case COMMAND:
						twoRegistersFlag = 0;
						IC += computeBinarySentence(sentence, &binaryWord);
						debug_binary_print(binaryWord);
						sprintf(buffer,"command%d%d",LINE,1);
						if (linked_list_set(commandsWords,buffer, &binaryWord,sizeof(unsigned int)) == -1) {
							printf("ERROR: something went wrong while writing code to memory.\n");
							errorFlag = 1;
						}
						if (sentence->operandsCount == 2) {
							/* handling case of registers */
							if (sentence->operands[0]->type == registerNumber) {
								binaryWord2 = 0;
								binaryWord2 |= (sentence->operands[0]->value << 7);
								if (sentence ->operands[1]->type == registerNumber) {
									binaryWord2 |= (sentence->operands[1]->value << 2);
									twoRegistersFlag = 1;
								}
							}

							/* handling first operand */
							if (sentence->operands[0]->type == constant) {
								binaryWord2 = 0;
								binaryWord2 = (sentence->operands[0]->value) << 2;
							}
							if (sentence->operands[0]->type == label) {
								symbol = symbols_table_get_symbol(st,sentence->operands[0]->label);
								if (symbol == NULL) {
									printf("ERROR: symbol \"%s\" is marked in line %d, but does not exists.",sentence->operands[0]->label, LINE);
									errorFlag = 1;
								} else {
									binaryWord2 = 0;
									binaryWord2 = ((symbol->value)+100) << 2;

									if (symbol->isExtern) {
											binaryWord2 = 1;
											debug_print("FOUND EXTERNAL SYMBOL %s with value %d at line %d IC = %d",symbol->label,symbol->value,LINE,IC);
											if (externalFile == NULL) {
												length = strlen(path);
												strcpy(buffer2,path);
												buffer2[length-2] = 'e';
												buffer2[length-1] = 'x';
												buffer2[length] = 't';
												buffer2[length+1] = '\0';
												externalFile = fopen(buffer2,"w+");
												debug_print("OPENING FILE: %s",buffer2);
												if (externalFile == NULL) {
													printf("Failed open file to write in path \"%s\".",buffer2);
													destroy_symbols_table(st);
													destroy_linked_list(dataWords);
													destroy_linked_list(commandsWords);
													return 0;
												}

											}

										fprintf(externalFile,"%s 	%d\n",symbol->label,IC+START_CODE_ROW-1);



										} else {
											binaryWord2 |= 2;
										}

								}
							}
							sprintf(buffer,"command%d%d",LINE,2);
							if (linked_list_set(commandsWords,buffer, &binaryWord2,sizeof(unsigned int)) == -1) {
								printf("ERROR: something went wrong while writing code to memory.\n");
								errorFlag = 1;
							}
							/* handling second operand */
							binaryWord3 = 0;
							if (sentence->operands[1]->type == registerNumber  && sentence->operands[0]->type != registerNumber && twoRegistersFlag == 0)
																binaryWord3 |= (sentence->operands[1]->value << 2);
							if (sentence->operands[1]->type == constant) {

									binaryWord3 = (sentence->operands[1]->value) << 2;
								}
								if (sentence->operands[1]->type == label) {
									symbol = symbols_table_get_symbol(st,sentence->operands[1]->label);
									if (symbol == NULL) {
										printf("ERROR: symbol \"%s\" is marked in line %d, but does not exists.",sentence->operands[1]->label, LINE);
										errorFlag = 1;
									} else {
										binaryWord3 = 0;
										binaryWord3 = ((symbol->value)+100) << 2;

										if (symbol->isExtern) {
												binaryWord3 = 1;
												debug_print("FOUND EXTERNAL SYMBOL %s with value %d at line %d IC = %d",symbol->label,symbol->value,LINE,IC);
												if (externalFile == NULL) {
													length = strlen(path);
													strcpy(buffer2,path);
													buffer2[length-2] = 'e';
													buffer2[length-1] = 'x';
													buffer2[length] = 't';
													buffer2[length+1] = '\0';
													externalFile = fopen(buffer2,"w+");
													debug_print("OPENING FILE: %s",buffer2);
													if (externalFile == NULL) {
														printf("Failed open file to write in path \"%s\".",buffer2);
														destroy_symbols_table(st);
														destroy_linked_list(dataWords);
														destroy_linked_list(commandsWords);
														return 0;
													}

												}

											fprintf(externalFile,"%s 	%d\n",symbol->label,IC+START_CODE_ROW-1);



											} else {
												binaryWord3 |= 2;
											}
									}
								}
								sprintf(buffer,"command%d%d",LINE, 3);
								if (linked_list_set(commandsWords,buffer, &binaryWord3,sizeof(unsigned int)) == -1) {
									printf("ERROR: something went wrong while writing code to memory.\n");
									errorFlag = 1;
								}
							}
							if (sentence->operandsCount == 1) {
								binaryWord2 = 0;
								/* handling case of registers */
								if (sentence->operands[0]->type == registerNumber) {
									binaryWord2 = 0;
									binaryWord2 |= (sentence->operands[0]->value << 2);

								}

								/* handling first operand */
								if (sentence->operands[0]->type == constant) {
									binaryWord2 = 0;
									binaryWord2 = (sentence->operands[0]->value) << 2;
								}
								if (sentence->operands[0]->type == label) {
									symbol = symbols_table_get_symbol(st,sentence->operands[0]->label);
									if (symbol == NULL) {
										printf("ERROR: symbol \"%s\" is marked in line %d, but does not exists.",sentence->operands[0]->label, LINE);
										errorFlag = 1;
									} else {
										binaryWord2 = 0;
										binaryWord2 = ((symbol->value)+100) << 2;

										if (symbol->isExtern) {
												binaryWord2 = 1;
												debug_print("FOUND EXTERNAL SYMBOL %s with value %d at line %d IC = %d",symbol->label,symbol->value,LINE,IC);
												if (externalFile == NULL) {
													length = strlen(path);
													strcpy(buffer2,path);
													buffer2[length-2] = 'e';
													buffer2[length-1] = 'x';
													buffer2[length] = 't';
													buffer2[length+1] = '\0';
													externalFile = fopen(buffer2,"w+");
													debug_print("OPENING FILE: %s",buffer2);
													if (externalFile == NULL) {
														printf("Failed open file to write in path \"%s\".",path);
														destroy_symbols_table(st);
														destroy_linked_list(dataWords);
														destroy_linked_list(commandsWords);
														return 0;
													}

												}

											fprintf(externalFile,"%s 	%d\n",symbol->label,IC+START_CODE_ROW-1);



											} else {
												binaryWord2 |= 2;
											}
									}
								}
								sprintf(buffer,"command%d%d",LINE,2);
								debug_print("Writing command %s for label %s with value %d ", buffer,sentence->operands[0]->label,binaryWord2);
								if (linked_list_set(commandsWords,buffer, &binaryWord2,sizeof(unsigned int)) == -1) {
									printf("ERROR: something went wrong while writing code to memory.\n");
									errorFlag = 1;
								}

							}



						break;
					default:
						debug_print("Unidentified behavior called in second iteration - non valid sentence type.");
						break;

				}
			}

			readlineValue = getline(&line, &len, file);
	}

	if (errorFlag) {
		printf("** PLEASE FIX MISTAKES IN SOURCE FILE AND REASSEMBLE **\n");
		destroy_symbols_table(st);
		destroy_linked_list(dataWords);
		destroy_linked_list(commandsWords);
		return 0;
	}
	/**
	 *  WRITE FILES
	 */

	/* writings ob file (/
	 */
	strcpy(buffer,path);
	length = strlen(path);
	buffer[length-2] = 'o';
	buffer[length-1] = 'b';

	writeFile = fopen(buffer,"w+");
	debug_print("OPENING FILE: %s",buffer);
	if (writeFile == NULL) {
		printf("Failed open file to write in path \"%s\".",path);
		destroy_symbols_table(st);
		destroy_linked_list(dataWords);
		destroy_linked_list(commandsWords);
		return 0;
	}

	fprintf(writeFile,"%d %d\n",IC, DC);
	length = linked_list_get_keys(commandsWords, &keys);
	for (i = 0; i < length; i ++ ) {
		debug_print("Printing key %d to file with label %s and value %d",i,keys[i],(unsigned int *) linked_list_get(commandsWords,keys[i]));
		print_binary_word_to_file(writeFile,(unsigned int *) linked_list_get(commandsWords,keys[i]));
	}
	free(keys);
	length = linked_list_get_keys(dataWords, &keys);
	for (i = 0; i < length; i ++ ) {
		debug_print("Printing key %d to file with label %s and value %d",i,keys[i],(unsigned int *) linked_list_get(dataWords,keys[i]));
		print_binary_word_to_file(writeFile,(unsigned int *) linked_list_get(dataWords,keys[i]));
	}
	free(keys);
	fclose(writeFile);

	/* Writing Entries */
	debug_print("GOT HERE");
	length = strlen(path);
	strcpy(buffer2,path);
	buffer2[length-2] = 'e';
	buffer2[length-1] = 'n';
	buffer2[length] = 't';
	buffer2[length+1] = '\0';

	writeFile = fopen(buffer2,"w+");
	debug_print("OPENING FILE: %s",buffer2);
	if (writeFile == NULL) {
		printf("Failed open file to write in path \"%s\".",path);
		destroy_symbols_table(st);
		destroy_linked_list(dataWords);
		destroy_linked_list(commandsWords);
		return 0;
	}

	symbols_table_get_symbols(st,&symbolsArray,0,0,0);
	for (i=0; i<st->length; i++) {
		if (symbolsArray[i]->isEntry) {
			fprintf(writeFile,"%s 	%d\n",symbolsArray[i]->label,symbolsArray[i]->value+START_CODE_ROW);
		}
	}


	free(symbolsArray);
	fclose(writeFile);
	if (externalFile != NULL) fclose(externalFile);

	destroy_symbols_table(st);
	destroy_linked_list(dataWords);
	destroy_linked_list(commandsWords);
	return 1;
}

int computeBinarySentence(Sentence * sentence, unsigned int * binaryWord) {
	unsigned int opCode;
	int wordsNeeded;
	*binaryWord = 0;
	wordsNeeded = 1;

	opCode = sentence->commandType-1;
	*binaryWord |= (opCode << 5);
	switch (sentence->operandsCount) {
		case 0:
			break;
		case 1:
			wordsNeeded++;
			*binaryWord |= (inferMiunFromOperand(*sentence->operands[0]) << 2);
			break;
		case 2:
			if (sentence->operands[0]->type == registerNumber && sentence->operands[1]->type == registerNumber) {
				wordsNeeded++;
			} else {
				wordsNeeded+=2;
			}
			*binaryWord |= (inferMiunFromOperand(*sentence->operands[0]) << 9);
			*binaryWord |= (inferMiunFromOperand(*sentence->operands[1]) << 2);
		break;
	default:
		break;
	}

	return wordsNeeded;
}

int inferMiunFromOperand (Operand o) {
	switch (o.type) {
		case registerNumber:
			return 5;
			break;
		case label:
			return 3;
			break;
		case constant:
		case character:
			return 1;
			break;
	}
	return 0;
}


void debug_binary_print(unsigned int word){
	/* found this !! trick (turn to true/false) using https://stackoverflow.com/questions/18327439/printing-binary-representation-of-a-char-in-c */
	int i;
	if (DEBUG) {
		for (i = 0; i < 12; i++) {
			printf("%d", !!((word << i ) & 0x800));
		}
		printf("\n");
	}
}

void print_binary_word_to_file (FILE * f, unsigned int * word) {
	static const unsigned char base64_table[65] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned int firstLetter;
	unsigned int secondLetter;

	firstLetter = 0;
	secondLetter = 0;
	firstLetter |= (*word & 0x3F);
	secondLetter |= (*word & 0xFC0) >> 6;
	fprintf(f,"%c",base64_table[secondLetter]);
	fprintf(f,"%c",base64_table[firstLetter]);
	fprintf(f,"\n");
	debug_binary_print(*word);
	return;
}



