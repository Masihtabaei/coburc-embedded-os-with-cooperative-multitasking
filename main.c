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

#include <stdint.h>
#include "stm32g474xx.h"
#include "delay.h"
#include "context.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */
// Defining a macro representing the right hand side of our led sequence
#define RIGHT_HAND_SIDE ((uint8_t) 0)
// Defining a macro representing the left hand side of our led sequence
#define LEFT_HAND_SIDE ((uint8_t) 1)
// Defining a macro representing the direction of led shift (in this case we shift them from left to right)
#define TO_RIGHT ((uint8_t) 2)
// Defining a macro representing the direction of led shift (in this case we shift them from right to left)
#define TO_LEFT ((uint8_t) 3)


#define NUMBER_OF_PROCESSES ((uint8_t) 4)
#define SIZE_OF_STACK_FRAME_PRO_PROCESS ((uint8_t) 14)
#define SIZE_OF_SUB_STACK_PRO_PROCESS ((uint8_t) 32)
uint32_t processes_stacks[NUMBER_OF_PROCESSES][SIZE_OF_SUB_STACK_PRO_PROCESS];

typedef void (*process_function)(void);



typedef enum  
{
	IDLE,
	RUNNING,
	DESTROYED
} process_state;

typedef enum
{
	SUCCESSFULLY_DESTROYED,
	ALREADY_DESTROYED,
	DESTRUCTION_BLOCKED_DURING_EXECUTION
} process_manipulation_status;

typedef struct 
{
	process_id id;
	process_state state;
	uintptr_t process_stack_pointer;
} process_context_block;


process_context_block process_list[NUMBER_OF_PROCESSES];

/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

// Declaring a function for the manage_right_hand_side-function (you can find more information on this function down by its defintion)
void manage_right_hand_side(void);
// Declaring a function for the manage_left_hand_side-function (you can find more information on this function down by its defintion)
void manage_left_hand_side(void);
// Declaring a function for the move_led-function (you can find more information on this function down by its defintion)
uint8_t move_led(uint8_t hand_side, uint8_t led_numeric_position, uint8_t direction);

process_id create(process_function function_responsible_for_this_process);
// Declaring the "destroy"-function whose definition can be found inside of the "main.c"-file
process_manipulation_status destroy(process_id id_of_process_to_destoroy);
void yield(void);

void process_for_startup_and_initialization(void);
void process_for_left_strip_of_leds(void);
void process_for_right_strip_of_leds(void);
void process_for_delay_between_steps(void);

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


process_id create(process_function function_for_process_to_create)
{
	uint32_t actual_process_id = 0;
	static uint32_t current_process_id = 0;
	if(current_process_id < NUMBER_OF_PROCESSES)
	{
		processes_stacks[current_process_id][SIZE_OF_SUB_STACK_PRO_PROCESS - 1] = (uintptr_t) function_for_process_to_create;
		for(int i = SIZE_OF_SUB_STACK_PRO_PROCESS - 2; i > 0; i--)
		{
			processes_stacks[current_process_id][i] = (uint32_t) 0;
		}
		process_context_block process_context_block_for_process_to_create = {current_process_id, IDLE, (uintptr_t) (&processes_stacks[current_process_id][SIZE_OF_SUB_STACK_PRO_PROCESS - SIZE_OF_STACK_FRAME_PRO_PROCESS])};
		process_list[current_process_id] = process_context_block_for_process_to_create;
		actual_process_id = current_process_id;
		current_process_id++;
	}
	return actual_process_id;
}

/**
Function defined as a part of our API for destroying a process.

Destroying a process by accomplishing following tasks:
	- Getting the process context vlock of the process which should be destroyed with the given ID from the processes list
	- Taking a decision whether the process can be destroyed or not by checking these 2 cases:
		- Checking whether a process is already destroyed
			-> Returning a manipulation status indicating that the process is already destroyed.
		- Checking whether a process is currently running
			-> Returning a manipulation status indicating the the process is currently running and can not get destroyed consequently. 
		- Destroying the process if non of above mentioned cases are applicable

@param[in] process_id id_of_process_to_destoroy (parameter containing the ID for identifying the process to destroy)
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
		return DESTRUCTION_BLOCKED_DURING_EXECUTION;
	}
	// Destroying the process if non of upper mentioned cases are applicable
	else
	{
		// Assigning the "DESTROYED" state to the process indicating that the proces is destroyed
		process_to_destroy.state = DESTROYED;
		// Retruning the "SUCCESSFULLY_DESTROYED" as the status of the manipulation indicating that the process is destroyed successfully.
		return SUCCESSFULLY_DESTROYED;
	}
}

void yield(void)
{
	process_id id_of_process_currently_running = 0;
	process_id id_of_process_to_run = 0;
	uint8_t index_for_iterating_over_processes = 0;
	
	while(process_list[index_for_iterating_over_processes].state != RUNNING && index_for_iterating_over_processes < NUMBER_OF_PROCESSES)
	{
		index_for_iterating_over_processes++;		
	}
	id_of_process_currently_running = index_for_iterating_over_processes;
	
	index_for_iterating_over_processes = ((id_of_process_currently_running + 1) == NUMBER_OF_PROCESSES) ? 0 : id_of_process_currently_running + 1;
	process_list[id_of_process_currently_running].state = IDLE;
	while(process_list[index_for_iterating_over_processes].state != IDLE)
	{
		index_for_iterating_over_processes = ((index_for_iterating_over_processes + 2) % NUMBER_OF_PROCESSES == 0) ? 0 : index_for_iterating_over_processes + 1;
	}
	id_of_process_to_run = process_list[index_for_iterating_over_processes].id;
	process_list[id_of_process_to_run].state = RUNNING;
	switch_context(id_of_process_currently_running, id_of_process_to_run);
}

void process_for_startup_and_initialization(void)
{
	process_list[0].state = RUNNING;
	create(process_for_left_strip_of_leds);
	create(process_for_right_strip_of_leds);
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
	
	yield();
}
void process_for_left_strip_of_leds(void)
{
	manage_left_hand_side();
	yield();
}

void process_for_right_strip_of_leds(void)
{
	manage_right_hand_side();
	yield();
}

void process_for_delay_between_steps(void)
{
	process_list[0].state = DESTROYED;
	delayms(500);
	yield();
}

void secure_the_process_stack_pointer_over_c(process_id process_id_of_process_to_update, uintptr_t new_value_of_process_stack_pointer)
{
	process_list[process_id_of_process_to_update].process_stack_pointer = new_value_of_process_stack_pointer;
}

uintptr_t retrieve_the_process_stack_pointer_over_c(process_id process_id_of_process_to_retrieve_its_stack_pointer)
{
	return process_list[process_id_of_process_to_retrieve_its_stack_pointer].process_stack_pointer;
}


/* --------------  S t a r t    o f    p r o g r a m  -----------------  */
	
int main(void)
{
	uint8_t process_id_of_initial_process = create(process_for_startup_and_initialization);
	load_first_context(process_id_of_initial_process);
		while(1)
	{
	}
	
}
