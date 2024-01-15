///@file exception_handler.c

#include "exception_handler.h"

void hard_fault_handler_over_c(uint32_t* hardfault_args, uint32_t lr_value)
{
	// Defining a multi-dimensional array that will later get used as a buffer for sprintf
	char buffer[NUMBER_OF_ROWS_TO_DISPLAY][MAXIMUM_NUMBER_OF_CHARACTERRS_IN_EACH_ROW];
	uint8_t line_index = 0;
	
	// Declaring a variable to store the value of R0
	uint32_t stacked_r0;
	// Declaring a variable to store the value of R1
	uint32_t stacked_r1;
	// Declaring a variable to store the value of R2
	uint32_t stacked_r2;
	// Declaring a variable to store the value of R3
	uint32_t stacked_r3;
	// Declaring a variable to store the value of R12
	uint32_t stacked_r12;
	// Declaring a variable to store the value of LR
	uint32_t stacked_lr;
	// Declaring a variable to store the value of PC
	uint32_t stacked_pc;
	// Declaring a variable to store the value of xPSR
	uint32_t stacked_xpsr;
	
	// Defining a variable to store the address of the bus fault
	uint32_t bus_fault_address = SCB->BFAR;
	// Defining a variable to store the address of the memory management fault
	uint32_t memmanage_fault_address = SCB->MMFAR;
	// Defining a variable to store the value of configurable fault status registers
	uint32_t cfsr = SCB->CFSR;
	
		// Setting a breakpoint
	__BKPT(0);
	
	// Retrieving the R0
	stacked_r0 = ((uint32_t) hardfault_args[0]);
	// Retrieving the R1
	stacked_r1 = ((uint32_t) hardfault_args[1]);
	// Retrieving the R2
	stacked_r2 = ((uint32_t) hardfault_args[2]);
	// Retrieving the R3
	stacked_r3 = ((uint32_t) hardfault_args[3]);
	// Retrieving the R12
	stacked_r12 = ((uint32_t) hardfault_args[4]);
	// Retrieving the LR
	stacked_lr = ((uint32_t) hardfault_args[5]);
	// Retrieving the PC
	stacked_pc = ((uint32_t) hardfault_args[6]);
	// Retrieving the xPSR
	stacked_xpsr = ((uint32_t) hardfault_args[7]);

	// Displaying (here only storing in a buffer) the value of R0
	sprintf(buffer[line_index++], "Value of R0: %x", stacked_r0);
	// Displaying (here only storing in a buffer) the value of R1
	sprintf(buffer[line_index++], "Value of R1: %x", stacked_r1);
	// Displaying (here only storing in a buffer) the value of R2
	sprintf(buffer[line_index++], "Value of R2: %x", stacked_r2);
	// Displaying (here only storing in a buffer) the value of R3
	sprintf(buffer[line_index++], "Value of R3: %x", stacked_r3);
	// Displaying (here only storing in a buffer) the value of R12
	sprintf(buffer[line_index++], "Value of R12: %x", stacked_r12);
	// Displaying (here only storing in a buffer) the value of LR
	sprintf(buffer[line_index++], "Value of LR: %x", stacked_lr);
	// Displaying (here only storing in a buffer) the value of PC
	sprintf(buffer[line_index++], "Value of PC: %x", stacked_pc);
	// Displaying (here only storing in a buffer) the value of xPSR
	sprintf(buffer[line_index++], "Value of xPSR: %x", stacked_xpsr);
	
	/* 
		Checking whether the memory management address is valid and
		if valid then displaying (here only storing in a buffer) it
	*/
	if (cfsr & SCB_CFSR_MMARVALID_Msk) sprintf(buffer[line_index++], "Value of MMFAR = %x", memmanage_fault_address);
	/* 
		Checking whether the bus fault address is valid and
		if valid then displaying (here only storing in a buffer) it
	*/
	if (cfsr & SCB_CFSR_BFARVALID_Msk) sprintf(buffer[line_index++], "Value of BFAR = %x", bus_fault_address);

	// Displaying (here only storing in a buffer) the value of EXC_RETURN
	sprintf(buffer[line_index++], "Value of EXC_RETURN: %x", lr_value);

	// Setting a breakpoint
	__BKPT(0);

	// Infinite-loop to keep the program running
	while(1);
}
