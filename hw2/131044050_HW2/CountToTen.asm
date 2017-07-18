        ; 8080 assembler code
        .hexfile CountToTen.hex
        .binfile CountToTen.com
        ; try "hex" for downloading in hex format
        .download bin 
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
FORK		equ 7
EXEC 		equ 8
WAITPID		equ 9

	; Position for stack pointer
stack   equ 04000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
; ---------------------------------------------------------------
; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

	;Bu program 0dan 10a kadar olan sayilari ekrana basar


begin:
	LXI SP,stack 	; always initialize the stack pointer
    mvi c, 11	; init C with 11
	mvi a, 0	; A = 0
	mvi b, 0	; B = 0
loop:
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS	; call the OS
	INR b		; ++B
	DCR c		; --C
	JNZ loop	; goto loop if C!=0	
	hlt			; end program
