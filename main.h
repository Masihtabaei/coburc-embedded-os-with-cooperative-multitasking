#ifndef _MAIN_H
	#define _MAIN_H

#include <stdint.h>
#include "stm32g474xx.h"
#include "delay.h"
#include "context.h"
#include "process.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */
// Defining a macro representing the right hand side of our led sequence
#define RIGHT_HAND_SIDE ((uint8_t) 0)
// Defining a macro representing the left hand side of our led sequence
#define LEFT_HAND_SIDE ((uint8_t) 1)
// Defining a macro representing the direction of led shift (in this case we shift them from left to right)
#define TO_RIGHT ((uint8_t) 2)
// Defining a macro representing the direction of led shift (in this case we shift them from right to left)
#define TO_LEFT ((uint8_t) 3)


/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

/* 
	Declaring a function for the manage_right_hand_side-function 
	(you can find more information on this function down by its defintion)
*/
void manage_right_hand_side(void);
/* 
	Declaring a function for the manage_left_hand_side-function 
	(you can find more information on this function down by its defintion)
*/
void manage_left_hand_side(void);
// Declaring a function for the move_led-function (you can find more information on this function down by its defintion)
uint8_t move_led(uint8_t hand_side, uint8_t led_numeric_position, uint8_t direction);


#endif // _MAIN_H
