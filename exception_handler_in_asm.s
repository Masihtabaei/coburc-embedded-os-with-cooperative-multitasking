	AREA exception_handlers, CODE ; Defining an area holding instructions related to the exception handlers
	; Using a directive to avoid the following  Error:
	;	L6238E: Invalid call from '~PRES8' function to 'REQ8' function
	; See more information that I found here:
	;	https://web.eece.maine.edu/zhu/book/faq.php#PRESERVE8
	PRESERVE8 {TRUE}
	; Making the symbol (function prototype of 
	; the hard_fault_handler_over_c-function
	; defined in the exception_handler.h-file) visible/accessible in this assembly file
	IMPORT hard_fault_handler_over_c
		
	; Providing a handler for hard faults
	; Parameter(s): -
	; Return Value(s): -
HardFault_Handler PROC
	EXPORT HardFault_Handler ; Exposing the definition of this procedure
	TST LR, #4
	ITE EQ
	MRSEQ R0, MSP
	MRSNE R0, PSP
	MOV R1, LR
	BL hard_fault_handler_over_c ; Calling a function written in C which takes care of the rest

	ENDP ; Ending the procedure
	END ; Ending the program