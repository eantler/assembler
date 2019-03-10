; another example

.entry LENGTH
.extern W
.extern ANOTHER
.entry EXAMPLE
MAIN:	mov	@r3, LENGTH
LOOP:	jmp L1
		prn -5
		bne W
		add ANOTHER, @r3
		prn -2
END:	stop
STR:	.string "abcdef"
LENGTH:	.data 6,-9,15
K:		.data	22
L:		.string "hello my friend"


