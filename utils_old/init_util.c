/*
    init_util.c: quick include of frequently used initializers functions for 
    the C8051F005. 

*/

/*
    includes
*/
#include "init_util.h"
#include <c8051F000.h>

/*
    defines
*/
#define OFF 0
#define ON  1

/*
    vWatchDog: 
    Turn watchdog timer On or Off.
*/
void vWatchDog(bit status){

    if(status==ON)
        return;                         // watchdog is enabled on power-on
    
    else{
        WDTCN=0xDE;                     // disable watchdog timer
        WDTCN=0xAD;
    }
}