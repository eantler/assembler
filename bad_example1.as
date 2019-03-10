; this the example from calss but with too many inputs for bne line

.entry LENGTH
.extern W
MAIN:	mov	@r3, LENGTH
LOOP:	jmp L1
		prn -5
		bne W, 5, 6, ANOTHER
		sub @r1,@r4
		bne L3
L1:		inc K
.entry LOOP
	jmp		W
END:	stop
STR:	.string "abcdef"
LENGTH:	.data 6,-9,15
K:		.data	22
.extern		L3
