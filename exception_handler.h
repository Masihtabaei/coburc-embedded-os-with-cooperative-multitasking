#ifndef _EXCEPTION_HANDLER_H
	#define _EXCEPTION_HANDLER_H

#include "stm32g474xx.h"
#include <stdint.h>
#include <stdint.h>
#include "stdio.h"

void hard_fault_handler_over_c(uint32_t* hardfault_args, uint32_t lr_value);
#endif
