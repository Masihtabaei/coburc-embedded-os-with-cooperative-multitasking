NO EQU 0x0 ; Defining a constant representing the value of no (false) 
YES EQU 0x1 ; Defining a constant representing the value of yes (true)

	; Defining a constant that will later get used
	; to check whether in some cases functions written in C
	; can be called instead of accomplishing the tasks completely
	; in assembly
IS_C_AIDED EQU NO 
	

	AREA context_related_variables ; Defining an area for variables related to the context management
process_id DCB 0x00 ; Reserving a byte for storing the id of the current process
	ALIGN ; Making sure that the instruction is aligned so that we do not get any warning
process_id_of_process_to_run DCB 0x00 ; Reserving a byte for storing the id of the process which we run next
	AREA context_related_constants ; Defining an area for constants related to the context managements

	; Reserving a byte for storing the size of the struct
	; (representing a process context block) so that we
	; can later calculate the absolute address of the stack pointer
struct_size DCB 0x0C

	; Reserving a byte fot storing the relative offset of the stack pointer
	; inside of our struct (representing a process context block)
stack_pointer_relative_offset DCB 0x08
	
	AREA context_related_procedures, CODE, READONLY ; Defining an area holding instructions related to context management
	; Using a directive to avoid the following  Error:
	;	L6238E: Invalid call from '~PRES8' function to 'REQ8' function
	; See more information that I found here:
	;	https://web.eece.maine.edu/zhu/book/faq.php#PRESERVE8
	PRESERVE8 {TRUE}
	; Making the symbol (function prototype of 
	; the secure_the_process_stack_pointer_over_c-function
	; defined in the context.h-file) visible/accessible in this assembly file
	IMPORT secure_the_process_stack_pointer_over_c
	; Making the symbol (function prototype of 
	; the retrieve_the_process_stack_pointer_over_c-function
	; defined in the context.h-file) visible/accessible in this assembly file
	IMPORT retrieve_the_process_stack_pointer_over_c

	; Implementing a procedure for retrieving the stack pointer of the
	; current process (it basically gets the address of the 
	; process stack pointer by calling the corresponding procedure and then
	; it returns the value of the stack pointer)
	; Parameter(s): R0 (holding the process id)
	; Return Value(s): R0 (holding the value of the process with the process id previously specified as a parameter)
retrieve_the_process_stack_pointer PROC
	PUSH{LR} ; Securing the LR so that we can later jump back to the caller
	
	; Getting the address of the specified process
	; by calling the corresponding procedure
	BL get_address_of_the_process_stack_pointer
	POP{LR} ; Retrieving the LR so that we can later jump back to the caller
	
	LDR R1, [R0] ; Loading the value of the stack pointer into R1
	MOV R0, R1 ; Moving the value of R1 into R0 (register used for storing the return value)
	
	BX LR ; Jumping back to the caller
	ENDP ; Ending the procedure


	; Implementing a procedure for securing the stack pointer of the
	; current process (it basically gets the address of the 
	; process stack pointer by calling the corresponding procedure and then
	; it sets/store the value of the stack pointer)
	; Parameter(s): R0 (holding the process id)
	;				R1 (holding the value of the stack pointer)
	; Return Value(s): -
secure_the_process_stack_pointer PROC
	PUSH{LR} ; Securing the LR so that we can later jump back to the caller
	
	; Securing the R1 because we need it later and
	; its value gets changed in the get_address_of_the_process_stack_pointer-procedure
	PUSH{R1}
	; Getting the address of the specified process
	; by calling the corresponding procedure
	BL get_address_of_the_process_stack_pointer
	POP{R1} ; Retrieving the R1 so that we can access the value that we have to secure
	POP{LR} ; Retrieving the LR so that we can later jump back to the caller
	
	STR R1, [R0] ; Securing the value of the stack pointer by writing it at the corresponding address
	
	BX LR ; Jumping back to the caller
	ENDP ; Ending the procedure


	; Implementing a procedure for getting the
	; absolute address of the stack pointer of a process
	; specified by its process id (it basically calculates the
	; absolute address using the the following algorithm:
	; absolute_address = process_list_address + 
	;						stack_pointer_relative_offset +
	;							(struct_size * process_id)							
	; Parameter(s): R0 (holding the process id)
	; Return Value(s): R0 (holding the aboslute address of the stack pointer)
get_address_of_the_process_stack_pointer PROC
	; Making the process_list-symbol (defined in the context.h-file) visible/accessible inside of this procedure
	IMPORT process_list
	
	LDR R3, =process_list ; Loading the address of the process_list-symbol into R3
	LDR R1, =struct_size ; Loading the address of the struct_size-symbol into R1
	LDRB R2, [R1] ; Loading the size of the struct (representing the process context block) into R2
	; Multiplying the R1 (holding the id of the process) and
	; R2 (holding the size of the struct) and storing the result it into R0
	MUL R0, R2
	LDR R1, =stack_pointer_relative_offset ; Loading the adress of the stack_pointer_relative_offset-symol into R1
	LDRB R2, [R1] ; Loading the relative offset of the stack pointer into R2
	; Adding R0 (holding the intermediate result) and
	; R2 (holding the relative offset of the stack pointer)
	; and storing the result into R0
	ADD R0, R2
	; Adding R0 (holding the intermediate result) and
	; R3 (holding the address of the process_list-symbol) and
	; storing the result into R0 (register used for storing the return value)
	ADD R0, R3
	
	BX LR ; Jumping back to the caller
	ENDP ; Ending the procedure

	; Implementing a procedure for taking care of the stack pointer retrieval
	; (it basically check if we should [are allowed] to call functions written in C
	; or not and then it take cares of the retrieval process by delegating it
	; to the corresponding function/procedure)
	; Parameter(s): -
	; Return Value(s): -
take_care_of_stack_pointer_retrieval PROC
	PUSH{LR} ; Securing the LR so that we can later jump back to the caller
	
	LDR R2, =process_id ; Loading the address of the process_id-symbol into R2
	LDRB R0, [R2] ; Loading id of the process into R0
	; Checking if we are allowed to use functions written in C
	; Yes -> it calls the correspodning function written in C
	; No -> it calls the corresponding procedure written in Assembly
	IF IS_C_AIDED == YES
	BL retrieve_the_process_stack_pointer_over_c ; Calling the correspodning function written in C
	ELSE
	BL retrieve_the_process_stack_pointer ; Calling the corresponding procedure written in Assembly
	ENDIF
	POP{LR} ; Retrieving the LR so that we can later jump back to the caller
	
	;  Retriving the stack pointer by writing the
	; returned/retrieved value from the corresponding
	; function/procedure into SP
	MOV SP, R0
	
	BX LR ; Jumping back to the caller
	ENDP ; Ending the procedure

	; Implementing a procedure for taking care of the stack pointer securing
	; (it basically check if we should [are allowed] to call functions written in C
	; or not and then it takes care of the securing process by delegating it
	; to the corresponding function/procedure)
	; Parameter(s): -
	; Return Value(s): -
take_care_of_stack_pointer_securing PROC
	PUSH{LR} ; Securing the LR so that we can later jump back to the caller
	
	; Incrementing thr SP by 24 bytes because
	; we secured R0, R1, R2, R3 and LR (2 times) which
	; decremented the SP to secure by 24 bytes (6 * 4 bytes)
	; and storing it into R1
	ADD R1, SP, #24
	LDR R2, =process_id ; Loading the address of the process_id-symbol into R2
	LDRB R0, [R2] ; Loading id of the process into R0
	; Checking if we are allowed to use functions written in C
	; Yes -> it calls the correspodning function written in C
	; No -> it calls the corresponding procedure written in Assembly
	IF IS_C_AIDED == YES
	BL secure_the_process_stack_pointer_over_c ; Calling a function written in C for securing the stack pointer of this process
	ELSE
	BL secure_the_process_stack_pointer ; Calling a function written in assembly for securing the stack pointer of this process
	ENDIF
	
	POP{PC} ; Jumping back to the caller
	ENDP ; Ending the procedure

	; Implementing a procedure for loading (not only) the first context
	; by retrieving its context (it basically retrieves the stack pointer
	; for (not only) the first context (specified by id of the process), then it restores/retrieves the 
	; context of that process and at the end it secures the stack pointer [but not the context]
	; for that context)
	; Parameter(s): R0 (holding the process id)
	; Return Value(s): -
load_first_context PROC
	EXPORT load_first_context ; Exposing the definition of this procedure
		
	LDR R1, =process_id ; Loading the address of the process_id-symbol into R1
	STRB R0, [R1] ; Storing value of the process id (given as parameter) at the corresponding address
	
	; Securing the R0, R1, R2 and R3 before calling a procedure
	; which takes care of stack pointer retrieval
	PUSH{R0-R3} 
	BL take_care_of_stack_pointer_retrieval ; Calling a procedure for retrieving the stack pointer
	POP{R0-R3} ; Retrieving/restoring the R0, R1, R2 and R3
	; Decrementing the stack pointer by 16 bytes
	; because we pushed those 4 registers (4 * 4 = 16 bytes)
	; and then retrieved the value of the stack pointer
	; so we have to decrement it to have the correct value of stack pointer
	SUB SP, #16
	; Retrieving/Restoring the context of this process
	POP{R4-R11}
	POP{R0-R3, R12, LR}
	
	; Securing the R0, R1, R2, R3 and LR before calling a procedure
	; which takes care of stack pointer securing
	PUSH{R0-R3, LR}
	BL take_care_of_stack_pointer_securing ; Calling a procedure for securing the stack pointer
	POP{R0-R3, LR} ; Retrieving/restoring the R0, R1, R2, R3 and LR
	
	BX LR ; Jumping back to the caller
	ENDP ; Ending the procedure
	

	; Implementing a procedure for loading (not only) the first context
	; by retrieving its context (it basically retrieves the stack pointer
	; for (not only) the first context (specified by id of the process), then it restores/retrieves the 
	; context of that process and at the end it secures the stack pointer [but not the context]
	; for that context)
	; Parameter(s): R0 (holding the process id of the current process)
	;				R1 (holding the process id of the process to switch to [next process])
	; Return Value(s): -
switch_context PROC
	EXPORT switch_context ; Exposing the definition of this procedure

	LDR R2, =process_id ; Loading the address of the process_id-symbol into R2
	STRB R0, [R2] ; Storing id of the current process (given as parameter) at the corresponding address
	
	LDR R2, =process_id_of_process_to_run ; Loading the address of the process_id_of_process_to_run-symbol into R2
	STRB R1, [R2] ; Storing id of the next process (given as parameter) at the corresponding address

	; Securing the R0, R1, R2 and R3 before calling a procedure
	; which takes care of stack pointer retrieval
	PUSH{R0-R3}
	BL take_care_of_stack_pointer_retrieval ; Calling a procedure for retrieving the stack pointer
	POP{R0-R3} ; Retrieving/restoring the R0, R1, R2 and R3
	; Decrementing the stack pointer by 16 bytes
	; because we pushed those 4 registers (4 * 4 = 16 bytes)
	; and then retrieved the value of the stack pointer
	; so we have to decrement it to have the correct value of stack pointer
	SUB SP, #16
	; Decrementing the stack pointer by 4 bytes
	; because we do not want to secure the LR so we need
	; to skip it (1 * 4 = 4 bytes)
	SUB SP, #4
	; Securing the context of this process
	PUSH{R0-R3, R12}	
	PUSH{R4-R11}

	; Securing the R0, R1, R2, R3 and LR before calling a procedure
	; which takes care of stack pointer securing
	PUSH{R0-R3, LR}
	BL take_care_of_stack_pointer_securing ; Calling a procedure for securing the stack pointer
	POP{R0-R3, LR} ; Retrieving/restoring the R0, R1, R2, R3 and LR
	
	LDR R2, =process_id_of_process_to_run ; Loading the address of the process_id_of_process_to_run-symbol into R2
	LDRB R0, [R2] ; Loading id of the next process into R0
	BL load_first_context ; Calling a procedure to load the context of the next/new process
	
	BX LR ; Jumping back to the caller
	ENDP ; Ending the procedure
		
	END ; Ending the program
		