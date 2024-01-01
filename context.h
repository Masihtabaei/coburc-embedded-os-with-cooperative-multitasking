#ifndef _CONTEXT_H
   #define _CONTEXT_H


#include <stdint.h>

/*************************************************************************/
/* -------------- E X P O R T E D   D E F I N I T I O N S -------------- */
/*************************************************************************/


/*  - T y p e s                                                          */

typedef uint32_t process_id;

/*  - C o n s t a n t s                                                  */


/*  - V a r i a b l e s                                                  */



/*  - M a c r o s                                                        */


/*  - F l a g s / B i t  M a s k s                                       */


/*  - F u n c t i o n  P r o t o t y p e s                               */

	void  load_first_context        (process_id process_id_to_load);
	void	switch_context						(process_id current_process_id, process_id next_process_id);
	void secure_the_process_stack_pointer_over_c (process_id process_id_of_process_to_update, uintptr_t new_value_of_process_stack_pointer);
	uintptr_t retrieve_the_process_stack_pointer_over_c(process_id process_id_of_process_to_retrieve_its_stack_pointer);

#endif  // _DELAY_H
