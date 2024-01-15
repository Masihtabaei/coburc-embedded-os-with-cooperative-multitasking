#ifndef _PROCESS_H
	#define _PROCESS_H

#include <stdint.h>

/*  - T y p e s                                                          */

// Defining a type representing the id of a process
typedef uint32_t process_id;

// Defining a type for a function pointer representing the process function
typedef void (*process_function)(void);

// Defining a type for an enum representing the state of a process
typedef enum  
{
	IDLE,
	RUNNING,
	DESTROYED,
	MARKED_FOR_DESTRUCTION
} process_state;

// Defining a type for an enum representing the state of a manipulated process
typedef enum
{
	SUCCESSFULLY_DESTROYED,
	ALREADY_DESTROYED,
	DESTRUCTION_BLOCKED_DURING_EXECUTION
} process_manipulation_status;

// Defining a type for a struct representing the process context block
typedef struct 
{
	process_id id;
	process_state state;
	uintptr_t process_stack_pointer;
} process_context_block;


/*  - M a c r o s                                                        */

// Defining a macro representing the number pf processes available
#define NUMBER_OF_PROCESSES ((uint8_t) 4)
/* 
	Defining a macro representing the size of the stack frame
	for each process (it is more for each context [snapshoz] of a process)
*/
#define SIZE_OF_STACK_FRAME_PRO_PROCESS ((uint8_t) 14)
// Defining a macro representing the size of the stack for each process 
#define SIZE_OF_SUB_STACK_PRO_PROCESS ((uint8_t) 32)

/*  - V a r i a b l e s                                                  */
// Declaring a multi-dimensional array (as a global variable) containing the stack of all processes
uint32_t processes_stacks[NUMBER_OF_PROCESSES][SIZE_OF_SUB_STACK_PRO_PROCESS];
// Declaring an array (as a global variable) containing the process context blocks of all processes
process_context_block process_list[NUMBER_OF_PROCESSES];

/*  - F u n c t i o n  P r o t o t y p e s                               */

/* 
	Declaring a function for the create-function 
	(you can find more information on this function down by its defintion)
*/
process_id create(process_function function_responsible_for_this_process);
/* 
	Declaring a function for the destory-function 
	(you can find more information on this function down by its defintion)
*/process_manipulation_status destroy(process_id id_of_process_to_destoroy);

/* 
	Declaring a function for the yield-function 
	(you can find more information on this function down by its defintion)
*/
void yield(void);

/* 
	Declaring a function for the process_for_startup_and_initialization-function 
	(you can find more information on this function down by its defintion)
*/
void process_for_startup_and_initialization(void);
/* 
	Declaring a function for the process_for_left_strip_of_leds-function 
	(you can find more information on this function down by its defintion)
*/
void process_for_left_strip_of_leds(void);
/* 
	Declaring a function for the process_for_right_strip_of_leds-function 
	(you can find more information on this function down by its defintion)
*/
void process_for_right_strip_of_leds(void);
/* 
	Declaring a function for the process_for_delay_between_steps-function 
	(you can find more information on this function down by its defintion)
*/
void process_for_delay_between_steps(void);

#endif // _PROCESS_H
