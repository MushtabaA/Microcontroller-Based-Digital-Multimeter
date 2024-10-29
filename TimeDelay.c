/* 
 * File:   TimeDelay.c
 * Author: Ahad Ali, Mushtaba Al Yasseen, and Parbir Lehal
 *
 * Created on October 12, 2021, 3:26 PM
 */

#include "TimeDelay.h"

/*
 *  Delay Function
 *  Inputs: tmr_ticks which is an unsigned int to be used for PR2
 *          idle_on which is a flag used to trigger idle mode
 *  Return: nothing
 *
 */
void delay_ms(int time_ms) {

    //T2CON config
    T2CONbits.TSIDL = 0; // operate in idle mode
    T2CONbits.T32 = 0; // operate timer 2 and timer 3 as 2 16 bit timers
    T2CONbits.TCS = 0; // use internal clock (fosc/2)

    //T2CON prescaler select
    T2CONbits.TCKPS0 = 1;
    T2CONbits.TCKPS1 = 1; // 1:256 


    // Timer 2 interrupt config
    IEC0bits.T2IE = 1; // Enable timer interrupt
    IFS0bits.T2IF = 0; // Clear timer 2 flag  

    float temp = (time_ms / 1000.0) * 15625.0;

    PR2 = (int) temp;

    //    PR2 = (time_ms / 1000) * 15625; // PR2 = time(s) / (Prescaler * 2/fosc) = 1 / (256 * 0.25us) = 15625


    T2CONbits.TON = 1; //start timer 2

    Idle(); //Enter idle state while waiting for interrupt

    return; // return to IOCheck where this function is called
}

/*
 *  Timer 2 Interrupt Service Routine
 *  Inputs: nothing
 *  Return: nothing
 */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    T2CONbits.TON = 0; //Stop timer
    TMR2 = 0; //zero TMR2 register on exit

    return;
}