#include "isrs.h"

/* Initial variable values */
bool pin_val = false;

/* Function called by the isr. The inline keyword is used to avoid a call
 * instruction
 */
inline void isr_timer(){
    pin_val = !pin_val;
}
