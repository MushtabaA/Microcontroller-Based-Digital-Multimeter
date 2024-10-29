/* 
 * File:   main.c
 * Author: Ahad Ali, Mushtaba Al Yasseen, and Parbir Lehal
 * Created on October 12, 2021, 3:26 PM
 */

//Include the compiler structures
#include "xc.h" // Include the necessary package to run the MPLAB Simulator
#include "IOs.h"    
#include "TimeDelay.h"
#include "config.h"
#include "UART2.h"

/*
 *  Main 
 *  Inputs: Nothing
 *  Return: Nothing
 *
 */
int main(void) {

    // Clock output on REFO/RB15 - Testing purposes only
    TRISBbits.TRISB15 = 0; // Set RB15 as output for REFO
    REFOCONbits.ROEN = 0; // Ref oscillator is enabled 
    //REFOCONbits.ROEN = 1; //in earlier video
    REFOCONbits.ROSSLP = 0; // REf oscillator is disabled in sleep
    REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
    REFOCONbits.RODIV = 0b1111;
    REFOCONbits.ROEN = 1; // Ref oscillator is enabled

    // Initialize IOs for low-power wake-up
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    IOinit(); // enables IO and CN interrupsts on Push buttons
    ADCinit();
    
    //IOinit(); //Calls function in IOs.c to intilize input/output pins
    
    NewClk(8);
    

    while (1) {
        // IOcheck(); // Calls function in IOs.c to check IO and turn on coressponding LED
        DispADC(); // Displays ADCBUF0 - from lecture
    }

    return 0;
}