        ; 8080 assembler code
        .hexfile Parallel.hex
        .binfile Parallel.com
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

	
programone:	dw 'CountToTen.com',00H ; null terminated string
programtwo:	dw 'PrintNumbers.com',00H ; 
programthree:	dw 'PrintNumbersRev.com',00H ;
 
begin:
	LXI SP,stack 	; always initialize the stack pointer
	mvi a, 0	; A = 0
	mvi b, 0	; B = 0

	MVI A, FORK	; store the OS call code to A
	call GTU_OS	; call the OS
	
	mvi a, 0	
	cmp b
	jnz process2	

	lxi b, programone
	MVI A, EXEC	; store the OS call code to A
	call GTU_OS	; call the OS
	
	hlt			; end program
	
	
process1:	
	MVI A, FORK	; store the OS call code to A
	call GTU_OS	; call the OS

	mvi a, 0	
	cmp b
	jnz process2	

	lxi b, programtwo
	MVI A, EXEC	; store the OS call code to A
	call GTU_OS	; call the OS
	
	
process2:
	lxi b, programthree
	MVI A, EXEC	; store the OS call code to A
	call GTU_OS	; call the OS

	hlt			; end program
	


	
	
	
	
	
	
