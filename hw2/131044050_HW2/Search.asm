        ; 8080 assembler code
        .hexfile Search.hex
        .binfile Search.com
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

	;Bu program kullanicidan alinan integer degeri listede arar

list: dw 012h, 034h, 053h, 02Ah, 05Bh, 06Fh, 033h, 021h, 07Ch, 0FFh, 0BAh, 0CBh, 0A1h, 01Ah, 03Bh, 0C3h, 04Ah, 05Dh, 062h, 0A3h, 0B1h, 05Ch, 07Fh, 0CCh, 0AAh, 034h		;

errorStr: dw 'error',00AH,00H	;

begin:
	LXI SP,stack 	; always initialize the stack pointer
	MVI A, READ_MEM	; store the OS call code to A - int to bc
	call GTU_OS	; call the OS
	LDAX B		; A <- (BC) aranan
	MOV E, A	;
	LXI B, list	; store the list address
    mvi d, 26	; init d with 26

loop:
	LDAX B		;
	CMP E		;
	JZ equal	;
	INX B		;
	INX B		;
	DCR d		; --d
	JNZ loop	; goto loop if d!=0	
	JMP notEqual	;
	
equal:	
	MVI A, 26	;
	SUB D		;
	MOV D, A	;
	MOV B, D	;
	MVI A, PRINT_B	; store the OS call code to A
	call GTU_OS		; call the OS
	JMP end		;
	
notEqual:	
	LXI B, errorStr	; put the address of string in registers B and C
	MVI A, PRINT_STR	; store the OS call code to A
	call GTU_OS	; call the OS
	JMP end	;

end:
	hlt				; end program
