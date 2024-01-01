
	AREA context_related_variables
process_id DCB 0x00
process_id_of_process_to_run DCB 0x00
process_stack_pointer DCD 0x00000000
	AREA context_related_constants
struct_size DCB 0x0C
stack_pointer_relativ_offset DCB 0x08
	
	AREA context_related_procedures, CODE, READONLY
		
retrieve_the_process_stack_pointer PROC
	PUSH{LR}
	LDR R1, =process_id
	LDRB R0, [R1]
	BL get_address_of_the_process_stack_pointer
	POP{LR}
	LDR SP, [R0]
	
	BX LR
	ENDP
		
secure_the_process_stack_pointer PROC
	PUSH{LR}
	LDR R1, =process_id
	LDRB R0, [R1]
	BL get_address_of_the_process_stack_pointer
	POP{LR}
	LDR R1, =process_stack_pointer
	LDR R2, [R1]
	STR R2, [R0]
	
	BX LR
	ENDP

get_address_of_the_process_stack_pointer PROC
	IMPORT process_list
		
	LDR R3, =process_list
	LDR R1, =struct_size
	LDRB R2, [R1]
	MUL R0, R2
	LDR R1, =stack_pointer_relativ_offset
	LDRB R2, [R1]
	ADD R0, R2
	ADD R0, R3
	
	BX LR
	ENDP
		
load_first_context PROC
	EXPORT load_first_context
		
	LDR R1, =process_id
	STRB R0, [R1]
	
	BL retrieve_the_process_stack_pointer
	POP{R4-R11}
	POP{R0-R3, R12, LR}
	
	PUSH{R0-R3, LR}
	LDR R0, =process_stack_pointer
	ADD SP, #20
	STR SP, [R0]
	SUB SP, #20
	BL secure_the_process_stack_pointer
	POP{R0-R3, LR}
	
	BX LR
	ENDP
		
switch_context PROC
	EXPORT switch_context

	LDR R2, =process_id
	STRB R0, [R2]
	
	LDR R2, =process_id_of_process_to_run
	STRB R1, [R2]
	
	BL retrieve_the_process_stack_pointer
	SUB SP, #4
	PUSH{R0-R3, R12}	
	PUSH{R4-R11}

	PUSH{R0-R3, LR}
	LDR R0, =process_stack_pointer
	ADD SP, #20
	STR SP, [R0]
	SUB SP, #20
	BL secure_the_process_stack_pointer
	POP{R0-R3, LR}
	
	LDR R2, =process_id_of_process_to_run
	LDRB R0, [R2]
	BL load_first_context
	
	BX LR
	ENDP
		
	END
		