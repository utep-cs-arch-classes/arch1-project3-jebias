	.file	"main.c"
	.local	state
	.text
	.globl	main
	.extern play
	.extern gameOver

state:	.word 0
main:	jmp case0

case0:	call play
	add #1, state
	cmp #1, state
	jz case1

case1:	call gameOver

	ret
