///@file main.c
/**************************************************************************
 *  COPYRIGHT (C) Peter Raab  2019                                        *
 *                                                                        *
 *                                                                        *
 **************************************************************************
 *  PROJECT       EBSy: Px - Basisproject                                 *
 *  MODULE        main.c                                                  *
 *  REVISION      2.0                                                     *
 **************************************************************************
 *  PURPOSE:                                                              *
 *   The main routine is the start of every C-program. The purpose of     *
 *   the main is to initialize the hardware and start the operation       *
 *   system.                                                              *
 *                                                                        *
 **************************************************************************
 *  REMARKS:                                                              *
 *                                                                        *
 *                                                                        *
 **************************************************************************
 *  CHANGE HISTORY:                                                       *
 *   Revision  Date         Author      Description                       *
 *     1.0      06.09.2019   P. Raab     creation                         *
 *     2.0      04.01.2021   P. Raab     port to stm32G474                *
 *                                                                        *
 *                                                                        *
 *************************************************************************/


/* ---------------- G L O B A L   D E F I N I T I O N S ---------------- */

/*  - I n c l u d e s                                                    */

#include "main.h"


/* ----------------------- F U N C T I O N S  -------------------------- */

/*
	Function Name: manage_right_hand_side
	Input/Parameters: void (no input)
	Output: void (no output)
	Brief Description: Defining the manage_right_hand_side-function used for taking care of  
										 logic of the right hand side of our led sequence. 
	Long Description: Here you can see the definiton of the previously declared manage_right_hand_side-function.
										This function contains the logic of the right hand side of our led sequence.
										It specifies the direction of shift and the current led position.
										List of actions done in this function:		
											- Managing right hand side of our led sequence
*/
void manage_right_hand_side(void)
{
	// Defining a static variable to keep track of shift direction
	static uint8_t direction = TO_LEFT;
	// Defining a static variable to keep track of led's position on this hand side
	static uint8_t led_numeric_position = 1;
	// Checking if we are at the beginning of the right hand side (0001 in binary)
	if(led_numeric_position == 1)
	{
		// We should shift the leds to left if we are at the beginning
		direction = TO_LEFT;
	}
	// Checking if we are at the end of this hand side (1000 in binary)
	else if(led_numeric_position == 8)
	{
		// We should shift the leds to right if we are at the end of this hand side
		direction = TO_RIGHT;
	}
	/* 
		Moving/Shifting the current led by calling the corresponding function 
		with parameters specifying the hand side, direction and current led's position and
		storing the new position (returned from the function called) into the previously defined variable.
	*/
	led_numeric_position = move_led(RIGHT_HAND_SIDE, led_numeric_position, direction);
}

/*
	Function Name: manage_left_hand_side
	Input/Parameters: void (no input)
	Output: void (no output)
	Brief Description: Defining the manage_left_hand_side-function used for taking care of  
										 logic of the left hand side of our led sequence. 
	Long Description: Here you can see the definition of the previously declared manage_left_hand_side-function.
										This function contains the logic of the left hand side of our led sequence.
										It specifies the direction of shift and the current led position.
										List of actions done in this function:		
											- Managing left hand side of our led sequence
*/
void manage_left_hand_side(void)
{
	// Defining a static variable to keep track of shift direction
	static uint8_t direction = TO_RIGHT;
	// Defining a static variable to keep track of led's position on this hand side
	static uint8_t led_numeric_position = 8;
	// Checking if we are at the beginning of the right hand side (0001 in binary)
	if(led_numeric_position == 1)
	{
		// We should shift the leds to left if we are at the beginning
		direction = TO_LEFT;
	}
	// Checking if we are at the end of this hand side (1000 in binary)
	else if(led_numeric_position == 8)
	{
		// We should shift the leds to right if we are at the end of this hand side
		direction = TO_RIGHT;
	}
	/* 
		Moving/Shifting the current led by calling the corresponding function 
		with parameters specifying the hand side, direction and current led's position and
		storing the new position (returned from the function called) into the previously defined variable.
	*/
	led_numeric_position = move_led(LEFT_HAND_SIDE, led_numeric_position, direction);
}

/*
	Function Name: move_led
	Input/Parameters:
		- uint8_t hand_side: parameter specifying the hand side of our led sequence where the led to shift is located
		- uint8_t led_numeric_position: parameter specifying the numerical position of led to shift
		- uint8_t direction: parameter specifying the direction of shift 
Output: void uint8_t (position of the current led)
	Brief Description: Defining the move_led-function used for moving/shifting leds. 
	Long Description: Here you can see the definiton of the previously declared  move_led-function.
										This function moves/shifts a led at a specific position on either left or right
										hand side in a specific direction (either to left or right)
										List of actions done in this function:		
											- Shifting the proper led in the proper direction
											- Calculating and returning the position of the current led
*/
uint8_t move_led(uint8_t hand_side, uint8_t led_numeric_position, uint8_t direction)
{
	// Defining a variable to use it as a container for the new value of GPIOx_ODR
	uint16_t new_gpio_odr_value = GPIOC->ODR;
	// Checking if we have to move/shift a led on the right hand side
	if(hand_side == RIGHT_HAND_SIDE)
	{
		/* 
			We should mask the corresponding bits of leds for the left hand side
			if we have to shift a led on the right hand side.
		*/
		new_gpio_odr_value &= 0x00F0;
	}
		// Checking if we have to move/shift a led on the left hand side
	else if(hand_side == LEFT_HAND_SIDE)
	{
		/* 
			We should mask the corresponding bits of leds for the right hand side
			if we have to shift a led on the left hand side.
		*/
		new_gpio_odr_value &= 0x000F;
	}
	
	/*
		Here we shift the bits to the left for 4 position if we have to shift the leds
		on the left hand side because we did not want to have two different cases for 
		led numerical positions. So we decided to code the position in the same manner
		and differentating them with the help of the hand_side parameter.
	*/ 
	new_gpio_odr_value |= (hand_side == LEFT_HAND_SIDE ? led_numeric_position << 4 : led_numeric_position);
	
	// Checking if we have to shift to right
	if(direction == TO_RIGHT)
	{
		// Shifting the current led 1 position to right
		led_numeric_position >>= 1;
	}
	// Checking if we have to shift to left
	else if(direction == TO_LEFT)
	{
		// Shifting the current led 1 position to left
		led_numeric_position <<= 1;
	}
	// Writing the new value into GPIOx_ODR
	GPIOC->ODR = new_gpio_odr_value;
	// Returning the new current position (after shifting/moving the led)
	return led_numeric_position;
}

/**
Function defined as a part of our API for creating a new process.

Creating a new process by accomplishing following tasks:
	- Creating a new instance of the "process_context_block" and assigning an ID, a state and the given function pointer to it
	- Adding the created instance to the list of the processes

@param[in] ledx function_of_process_to_create (function pointer to the function for which we will create a process)
@param[out] process_id (ID of the process created)

*/
process_id create(process_function function_for_process_to_create)
{
	// Defining a variable for storing id of the process created
	uint32_t actual_process_id = 0;
	// Defining a static variable for storing id of the last process created 
	static uint32_t current_process_id = 0;
	
	// Checking whether we still can create a new process
	if(current_process_id < NUMBER_OF_PROCESSES)
	{
		// Assigning function of the process to the corresponding entry inside of the stack
		processes_stacks[current_process_id][SIZE_OF_SUB_STACK_PRO_PROCESS - 1] = (uintptr_t) function_for_process_to_create;
		// Iterating over the stack entries (skipping the first entry [function pointer/LR])
		for(int i = SIZE_OF_SUB_STACK_PRO_PROCESS - 2; i > 0; i--)
		{
			// Initializing the entry with the value zero
			processes_stacks[current_process_id][i] = (uint32_t) 0;
		}
		// Creating a process context block for the new process
		process_context_block process_context_block_for_process_to_create = {current_process_id, IDLE, (uintptr_t) (&processes_stacks[current_process_id][SIZE_OF_SUB_STACK_PRO_PROCESS - SIZE_OF_STACK_FRAME_PRO_PROCESS])};
		// Adding the process newly created to the list of processes
		process_list[current_process_id] = process_context_block_for_process_to_create;
		// Setting the actual process id
		actual_process_id = current_process_id;
		// Increasing the static variable that we use for keeping track of available process ids
		current_process_id++;
	}
	// Returning id of the process created
	return actual_process_id;
}

/**
Function defined as a part of our API for destroying a process.

Destroying a process by accomplishing following tasks:
	- Getting the process context of the process which should be destroyed with the given ID from the processes list
	- Taking a decision whether the process can be destroyed or not by checking these 2 cases:
		- Checking whether a process is already destroyed
			-> Returning a manipulation status indicating that the process is already destroyed.
		- Checking whether a process is currently running
			-> Returning a manipulation status indicating the the process is currently running and can not get destroyed consequently. 
		- Destroying the process if non of above mentioned cases are applicable

@param[in] process_id id_of_process_to_destroy (parameter containing the ID for identifying the process to destroy)
@param[out] process_manipulation_status (status of the manipulation [in this case destruction]-action)

*/
process_manipulation_status destroy(process_id id_of_process_to_destoroy)
{
	// Getting the process context vlock of the process which should be destroyed with the given ID from the processes list
	process_context_block process_to_destroy = process_list[id_of_process_to_destoroy];
	
	// Checking whether a process is already destroyed
	if(process_to_destroy.state == DESTROYED)
	{
		// Retruning the "ALREADY_DESTROYED" as the status of the manipulation indicating that a process can not get destroyed multiple times.
		return ALREADY_DESTROYED;
	}
	// Checking whether a process is currently running
	else if(process_to_destroy.state == RUNNING)
	{
		// Retruning the "DESTRUCTION_BLOCKED_DURING_EXECUTION" as the status of the manipulation indicating that the process can not get destroyed during its execution.
		process_list[id_of_process_to_destoroy].state = MARKED_FOR_DESTRUCTION;
		return DESTRUCTION_BLOCKED_DURING_EXECUTION;
	}
	// Destroying the process if non of upper mentioned cases are applicable
	else
	{
		// Assigning the "DESTROYED" state to the process indicating that the proces is destroyed
		process_list[id_of_process_to_destoroy].state = DESTROYED;
		// Retruning the "SUCCESSFULLY_DESTROYED" as the status of the manipulation indicating that the process is destroyed successfully.
		return SUCCESSFULLY_DESTROYED;
	}
}

/**
Function defined as a part of our API for process scheduling (most important) and freeing up the ressources.

Scheduling and freeing up the resources by accomplishing following tasks:
	- Finding the process currently running and changing its state
	- Finding the process that should get executed next and changing its state
	- Switching from the old process to the new one by calling the corresponding function
	

@param[in] void
@param[out] void

*/
void yield(void)
{
	// Defining a variable for storing id of the process currently running
	process_id id_of_process_currently_running = 0;
	// Defining a variable for storing id of the process that should get executed next
	process_id id_of_process_to_run = 0;
	// Defining a variable as an index for iterating over processes
	uint8_t index_for_iterating_over_processes = 0;
	
	// Iterating over the list of the processes to find the process currently running
	while((process_list[index_for_iterating_over_processes].state != RUNNING && process_list[index_for_iterating_over_processes].state != MARKED_FOR_DESTRUCTION)&& index_for_iterating_over_processes < NUMBER_OF_PROCESSES)
	{
		// Increasing the index
		index_for_iterating_over_processes++;		
	}
	// Assigning the index of process found to the preivously defined variable for storing id of the process currently running
	id_of_process_currently_running = index_for_iterating_over_processes;
	
	// Checking whether the process currently running is marked for destruction
	if(process_list[id_of_process_currently_running].state == MARKED_FOR_DESTRUCTION)
	{
		// Destroying the process that was marked for destruction
		destroy(id_of_process_currently_running);
	}
	else
	{
		// Changing the state of the process currently running to the "IDLE"-state
		process_list[id_of_process_currently_running].state = IDLE;
	}
	
	// Re-calculating the index so that we can use it again for iterating over the list of processes
	index_for_iterating_over_processes = ((id_of_process_currently_running + 1) == NUMBER_OF_PROCESSES) ? 0 : id_of_process_currently_running + 1;
	// Iterating over the list of the processes to find the process that should get executed next
	while(process_list[index_for_iterating_over_processes].state != IDLE)
	{
		// Re-calculating the index so that we can continue the iteration
		index_for_iterating_over_processes = ((index_for_iterating_over_processes + 2) % NUMBER_OF_PROCESSES == 0) ? 0 : index_for_iterating_over_processes + 1;
	}
	/* 
		Assigning the index of process found to
		the preivously defined variable for storing id of the process
		that should get executed next
	*/
	id_of_process_to_run = process_list[index_for_iterating_over_processes].id;
	// Setting the state of the new process into the "RUNNING"-state
	process_list[id_of_process_to_run].state = RUNNING;
	/*
		Switching from the old process to the new one
		(actually changing the contexts of those processes)
		by calling the corresponding function
	*/
	switch_context(id_of_process_currently_running, id_of_process_to_run);
}

/**
Function used as a process for taking care of the startup process and all initializations.

Taking care of the startup process and all initialization by accomplishing following task (this process is executed only once and at the very beginning):
	- Setting the state of itself into the "RUNNING" state
	- Creating the rest of processes
	- Enabling the clock for GPIOA
	- Configuring the first 8 pins of GPIOA as output-pins
	- Turning all leds off
	- Destroying itself
	- Giving up the resources at the end

@param[in] void
@param[out] void

*/
void process_for_startup_and_initialization(void)
{
	// Setting the state of this process into the "RUNNING"-state
	process_list[0].state = RUNNING;
	
	// Creating the rest of processes
	
	// Creating a process for taking care of the left strip of leds
	create(process_for_left_strip_of_leds);
	// Creating a process for taking care of the right strip of leds
	create(process_for_right_strip_of_leds);
	// Creating a process for having a delay between leds getting shifted
	create(process_for_delay_between_steps);
	
	//create(process_for_right_strip_of_leds);
	//create(process_for_delay_between_steps);
	//create(process_for_right_strip_of_leds);
	//create(process_for_delay_between_steps);
	
	// Enabling the clock for GPIO-port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	// Configuring the first 8 pins of GPIO-port A as GP-output pins
	GPIOC->MODER &= 0xFFFF0000;
	GPIOC->MODER |= GPIO_MODER_MODE0_0 |
									GPIO_MODER_MODE1_0 |
									GPIO_MODER_MODE2_0 |
									GPIO_MODER_MODE3_0 |
									GPIO_MODER_MODE4_0 |
									GPIO_MODER_MODE5_0 |
									GPIO_MODER_MODE6_0 |
									GPIO_MODER_MODE7_0;
	
	// Turning all leds off
	GPIOC->ODR = 0x0000;
	
	// Destroying itself (actually marking itself for destruction)
	destroy(0);
	
	// Giving up the resources cooperatievly
	yield();
}

/**
Function used as a process for managing the left strip of leds.

Managing leds on the left half side (shifting of its leds) by calling the corresponding function and giving up the resources at the end by calling the yield function

@param[in] void
@param[out] void

*/
void process_for_left_strip_of_leds(void)
{
	// Shifting the leds on the left hand side by calling the corresponding function
	manage_left_hand_side();
	// Giving up the resources cooperatievly
	yield();
}

/**
Function used as a process for managing the right strip of leds.

Managing leds on the right half side (shifting of its leds) by calling the corresponding function and giving up the resources at the end by calling the yield function

@param[in] void
@param[out] void

*/
void process_for_right_strip_of_leds(void)
{
	// Shifting the leds on the right hand side by calling the corresponding function
	manage_right_hand_side();
	// Giving up the resources cooperatievly
	yield();
}

/**
Function used as a process for the delay between steps (i. e. led-shifting step).

Serving as a delay process by calling the corresponding delay function and giving up the resources by calling the yield function

@param[in] void
@param[out] void

*/
void process_for_delay_between_steps(void)
{
	//process_list[0].state = DESTROYED;
	// Waiting for 500 milliseconds
	delayms(500);
	// Giving up the resources cooperatievly
	yield();
}

/**
Function used for securing the stack pointer over the C as an alternative to Assembly.

Securing the stack pointer of a specific process by assigning the new value of the stack pointer to the corresponding struct member

@param[in] process_id process_id_of_process_to_update (id of the process its stack pointer should get secured)
@param[in] uintptr_t new_value_of_process_stack_pointer (new/current value of the stack pointer for the process previously specified by its id)
@param[out] void

*/
void secure_the_process_stack_pointer_over_c(process_id process_id_of_process_to_update, uintptr_t new_value_of_process_stack_pointer)
{
	// Assigning the new/current value of the stack pointer to the corresponding struct member
	process_list[process_id_of_process_to_update].process_stack_pointer = new_value_of_process_stack_pointer;
}

/**
Function used for retrieving the stack pointer over the C as an alternative to Assembly.

Retrieving the stack pointer of a specific process by returning the current value of the stack pointer

@param[in] process_id process_id_of_process_to_retrieve_its_stack_pointer (id of the process its stack pointer should get secured)
@param[out] uintptr_t (current value of the stack pointer for a process previously specified by its id)

*/
uintptr_t retrieve_the_process_stack_pointer_over_c(process_id process_id_of_process_to_retrieve_its_stack_pointer)
{
	// Accessing and returning the current value of the stack pointer for this process
	return process_list[process_id_of_process_to_retrieve_its_stack_pointer].process_stack_pointer;
}


/* --------------  S t a r t    o f    p r o g r a m  -----------------  */

/**
Function used as the entry point of the application.

Serving as the entry point of the application by taking care of following tasks:
	- Creating a process for startup and initialization
	- Loading the initial/first context
	- Keeping the program running by using an infinite loop (while(1/true))

@param[in] void
@param[out] int (exit-code)

*/
int main(void)
{
	// Uncomment the following line of code to produce a HardFault
	//*(uint32_t*)0xDEADBEEF=(0xA05F<<3);
	
	// Creating a process for startup and initialization and assigning the returning value (process id generated) to a local variable
	uint8_t process_id_of_initial_process = create(process_for_startup_and_initialization);
	// Loading the initial/first context
	load_first_context(process_id_of_initial_process);
	
	// Infinite-loop to keep the program running
	while(1);
}
