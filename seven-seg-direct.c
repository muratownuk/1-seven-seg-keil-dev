/*
    includes
*/
#include <c8051F000.h>

/*
    16-bit SFR definitions for C8051F005
*/
sfr16 RCAP2=0xCA;                   // timer 2 reload value
sfr16 T2=0xCC;                      // timer 2

/*
    function prototypes
*/
void vWatchDog(bit status);
void vOSC_Init(void);
void vPort_Init(void);
void vTimer2_Init(void);
void vDisplayNumber(int number);
void vDelay_ms(unsigned int ms);

/*
    global constants
*/
#define OFF 0
#define ON  1

#define SYSCLK 16000000                 // internal oscillator frequency
#define DELAY  1000                     // 1 second delay
#define RELOAD_VAL -((SYSCLK/12)/1000)  // RCAP2 reload value for timer 2

/*
    main
*/
void main(void){

    int count=0;

    // initialize c8051f005
    vWatchDog(OFF);                 // disable watchdog timer
    vOSC_Init();                    // initialize oscillator (16.0 MHz)
    vPort_Init();                   // initialize xbar & GPIO
    vTimer2_Init();                 // initialize timer 2

    // main program
    while(1){
        
        vDisplayNumber(count);      // display number on 7-seg LED
        vDelay_ms(DELAY);           // 1 second delay
        count++;

        if(count==10)
            count=0;

    }

}

/*
    functions
*/
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

/*
    vOSC_Init: 
    use internal oscillator (OSCICN) at 16.0 MHz (SYSCLK) and turn off 
    external oscillator (OSCXCN).

    parameters: none
    return    : none
    

*/
void vOSC_Init(void){

    OSCXCN&=0x00;                    // turn off external oscillator
    OSCICN|=0x83;                    // CLKSL=0 (internal oscillator) @16MHz

}

/*
    vPORT_Init:
    enable crossbar (XBARE) for port outputs and use ports P0.0-6 as outputs
    (push-pull). Initiallize the P0 outputs all to low.

    parameters: none
    return    : none

*/
void vPort_Init(void){

    XBR2=0x40;                      // enable crossbar (xbar) and weak-pullups
    PRT0CF=0x7F;                    // P0.0-6 as push-pull outputs
    P0=0x00;                        // turn all of port 0 (P0) outputs low

}

/*
    vTimer2_init:
    initialize timer 2 to how we want to use it. timer 2 in mode 1 with 
    auto-reload. Disable timer 2 interrupts (ET2) as we will use timer 2 flag 
    (TF2) directly.

*/
void vTimer2_Init(void){

    ET2=0;                          // disable T2 interrupt (EA=0 anyways)
    // make sure T2M=0 in clock control reg, this uses SYSCLK/12 as time base.
    CKCON&=!0x20;
    // load the reload value for T2 in RCAP2. timer 2 overflows at 1KHz.                   
    RCAP2=RELOAD_VAL;
    T2=RCAP2;                       // initial load value for T2           
 
}

/*
    vDisplayNumber:
    display the number on the 7-segment LED display.

    parameters: number (the number to display)
    return    : none  

*/
void vDisplayNumber(int number){

    switch(number){
        case 0:
            P0=0x3F;                // ABCDEF ON
            break;
        case 1:
            P0=0x06;                // BC ON
            break;
        case 2:
            P0=0x5B;                // ABDEG ON
            break;
        case 3:                     
            P0=0x4F;                // ABCDG ON
            break;
        case 4:
            P0=0x66;                // BCFG ON
            break;
        case 5:
            P0=0x6D;                // ACDFG ON
            break;
        case 6:
            P0=0x7D;                // ACDEFG ON
            break;
        case 7:
            P0=0x07;                // ABC ON
            break;
        case 8:
            P0=0x7F;                // ABCDEFG ON
            break;
        case 9:
            P0=0x6F;                // ABCDFG ON
            break;
        default:
            P0=0x00;                // NONE ON
    }

}

/*
    vDelay_ms:
    create milliseconds delay with this function.

    parameters: ms (number of milliseconds of delay)
    return    : none

*/
void vDelay_ms(unsigned int ms){
    
    TR2=1;                          // start timer 2
    while(ms){        
        TF2=0;                      // clear timer 2 flag
        while(!TF2)
            ;                       // wait until T2 overflows
        ms--;                       // decrement millisecond count
    }
    TR2=0;                          // stop timer 2 

}






























