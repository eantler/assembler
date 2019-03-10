
all: assembler

assembler:
	gcc -o assembler -Wall -ansi -pedantic assembler.c fileAssembler/fileAssembler.c utilities/utils.c utilities/StringHashTable/StringHashTable.c utilities/LinkedList/LinkedList.c tokenizer/tokenizer.c tokenizer/tokenizerUtils.c symbolsTable/symbolsTable.c -I. 

	
	