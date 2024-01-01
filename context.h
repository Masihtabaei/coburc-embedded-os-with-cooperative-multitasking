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

#endif  // _DELAY_H
