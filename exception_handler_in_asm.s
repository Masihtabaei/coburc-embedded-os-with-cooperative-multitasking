	AREA exception_handlers, CODE
	PRESERVE8
	EXPORT HardFault_Handler
	IMPORT hard_fault_handler_over_c
HardFault_Handler PROC
	TST LR, #4
	ITE EQ
	MRSEQ R0, MSP
	MRSNE R0, PSP
	MOV R1, LR
	BL hard_fault_handler_over_c

	ENDP
	END