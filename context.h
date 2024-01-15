#ifndef _CONTEXT_H
   #define _CONTEXT_H

#include <stdint.h>
#include "process.h"

/*************************************************************************/
/* -------------- E X P O R T E D   D E F I N I T I O N S -------------- */
/*************************************************************************/

/*  - F u n c t i o n  P r o t o t y p e s                               */

	/* 
		Declaring a function for the load_first_context -function 
		(you can find more information on this function down by its defintion)
	*/
	void  load_first_context        (process_id process_id_to_load);
		/* 
		Declaring a function for the switch_context-function 
		(you can find more information on this function down by its defintion)
	*/
	void	switch_context						(process_id current_process_id, process_id next_process_id);
		/* 
		Declaring a function for the secure_the_process_stack_pointer_over_c-function 
		(you can find more information on this function down by its defintion)
	*/
	void secure_the_process_stack_pointer_over_c (process_id process_id_of_process_to_update, uintptr_t new_value_of_process_stack_pointer);
		/* 
		Declaring a function for the retrieve_the_process_stack_pointer_over_c-function 
		(you can find more information on this function down by its defintion)
	*/
	uintptr_t retrieve_the_process_stack_pointer_over_c(process_id process_id_of_process_to_retrieve_its_stack_pointer);

#endif  // _DELAY_H
