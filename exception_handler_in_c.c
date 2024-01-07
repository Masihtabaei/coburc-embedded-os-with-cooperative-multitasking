#include "exception_handler.h"

void hard_fault_handler_over_c(uint32_t* hardfault_args, uint32_t lr_value)
{
	char buffer[18][100];
	uint8_t line_index = 0;
	
	uint32_t stacked_r0;
	uint32_t stacked_r1;
	uint32_t stacked_r2;
	uint32_t stacked_r3;
	uint32_t stacked_r12;
	uint32_t stacked_lr;
	uint32_t stacked_pc;
	uint32_t stacked_xpsr;
	
	uint32_t bus_fault_address = SCB->BFAR;
	uint32_t memmanage_fault_address = SCB->MMFAR;
	uint32_t cfsr = SCB->CFSR;
	
	__BKPT(0);
	
	stacked_r0 = ((uint32_t) hardfault_args[0]);
	stacked_r1 = ((uint32_t) hardfault_args[1]);
	stacked_r2 = ((uint32_t) hardfault_args[2]);
	stacked_r3 = ((uint32_t) hardfault_args[3]);
	stacked_r12 = ((uint32_t) hardfault_args[4]);
	stacked_lr = ((uint32_t) hardfault_args[5]);
	stacked_pc = ((uint32_t) hardfault_args[6]);
	stacked_xpsr = ((uint32_t) hardfault_args[7]);

	sprintf(buffer[line_index++], "Value of R0: %x", stacked_r0);
	sprintf(buffer[line_index++], "Value of R1: %x", stacked_r1);
	sprintf(buffer[line_index++], "Value of R2: %x", stacked_r2);
	sprintf(buffer[line_index++], "Value of R3: %x", stacked_r3);
	sprintf(buffer[line_index++], "Value of R12: %x", stacked_r12);
	sprintf(buffer[line_index++], "Value of LR: %x", stacked_lr);
	sprintf(buffer[line_index++], "Value of PC: %x", stacked_pc);
	sprintf(buffer[line_index++], "Value of xPSR: %x", stacked_xpsr);
	
	if (cfsr & SCB_CFSR_MMARVALID_Msk) sprintf(buffer[line_index++], "Value of MMFAR = %x", memmanage_fault_address);
	if (cfsr & SCB_CFSR_BFARVALID_Msk) sprintf(buffer[line_index++], "Value of BFAR = %x", bus_fault_address);

	sprintf(buffer[line_index++], "Value of EXC_RETURN: %x", lr_value);

	__BKPT(0);

	while(1);
}
