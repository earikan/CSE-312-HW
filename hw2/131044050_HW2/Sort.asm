        ; 8080 assembler code
        .hexfile Sort.hex
        .binfile Sort.com
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

	;Bu program listedeki degerleri artan bir sekilde siralar

list: dw 012h, 034h, 053h, 02Ah, 05Bh, 06Fh, 033h, 021h, 07Ch, 0FFh, 0BAh, 0CBh, 0A1h, 01Ah, 03Bh, 0C3h, 04Ah, 05Dh, 062h, 0A3h, 0B1h, 05Ch, 07Fh, 0CCh, 0AAh, 034h		;

begin:
	LXI SP,stack; always initialize the stack pointer
	
	
sorts:
	MVI e, 0	; swapped = false
	LXI B, list	; store the list address
	MVI d, 25	; length
inLoop:	
	LDAX B		; load data from bc
	MOV H, A	; store data in h
	INX B		; increment to other
	INX B		; element

	LDAX B		; load data from bc
	CMP H		; A <- A - H
	JNC nonswap	; 
	
	DCX	B	; return to before 
	DCX	B	; element
	STAX B		; store to address

	INX B		; take back to second 
	INX B		; element 
	MOV A, H	; store second num
	STAX B		; store the second num
	; swap ends , change swapped
	MVI	E, 1	; swapped = true
nonswap:
	DCR D		; D--
	JNZ inLoop	; 

	INR E
	DCR E
	JNZ sorts	; 


	LXI B, list	; store the list address
	MVI d, 26	; init d with 5

loop:	
	MVI A, PRINT_MEM; store the OS call code to A
	call GTU_OS	; call the OS
	INX B		;
	INX B
	DCR D		; --D
	JNZ loop	; goto loop if D!=0	

	HLT ;




