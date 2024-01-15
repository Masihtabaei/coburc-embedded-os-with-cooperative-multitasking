#ifndef _EXCEPTION_HANDLER_H
	#define _EXCEPTION_HANDLER_H

#include "stm32g474xx.h"
#include <stdint.h>
#include "stdio.h"

/*  - M a c r o s                                                        */

#define NUMBER_OF_ROWS_TO_DISPLAY ((uint8_t) 18)
#define MAXIMUM_NUMBER_OF_CHARACTERRS_IN_EACH_ROW ((uint8_t) 100)
/*  - F u n c t i o n  P r o t o t y p e s                               */

/* 
	Declaring a function for the hard_fault_handler_over_c-function 
	(you can find more information on this function down by its defintion)
*/
void hard_fault_handler_over_c(uint32_t* hardfault_args, uint32_t lr_value);

#endif // _EXCEPTION_HANDLER_H
