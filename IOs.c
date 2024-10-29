/* 
 * File:   IOs.c
 * Author: Ahad Ali, Mushtaba Al Yasseen, and Parbir Lehal
 *
 * Created on October 12, 2021, 3:26 PM
 */

#include "IOs.h"
#include "TimeDelay.h"
#include "UART2.h"


// Global Variables which are being used for the Application Project #1
uint16_t minutes = 00; // represents minutes on the clock (countdown/timer)
uint16_t seconds = 00; // represents seconds on the clock (countdown/timer)
int countdown_on = 0;

/*
 *  IO initializing 
 *  Inputs: The three pushbuttons TRISA4, TRISA2 and TRISB4
 *          Pull Up resistors CN1PUE, CN0PUE and CN30PUE 
 *  Return: nothing
 *
 */
void IOinit() {

    AD1PCFG = 0xFFFF; // Turn all analog pins as digital (as per lab manual)

    // Set all inputs and outputs
    TRISAbits.TRISA4 = 1; // Set RA4 as input
    TRISBbits.TRISB4 = 1; // Set RB4 as input
    TRISAbits.TRISA2 = 1; // Set RA2 as input
    TRISBbits.TRISB8 = 0; // Set RB8 as output

    // Set up the Pull up resistors
    CNPU1bits.CN1PUE = 1; // Pull CN1 - RB4 Up 
    CNPU1bits.CN0PUE = 1; // Pull CN1 - RA4 Up 
    CNPU2bits.CN30PUE = 1; // Pull CN1 - RA2 Up

    //Initialize CN Interrupt
    CNEN1bits.CN0IE = 1; //Enables CN interrupt for CN0 (Pushbutton PB1)
    CNEN1bits.CN1IE = 1; //Enables CN interrupt for CN1 (Pushbutton PB2)
    CNEN2bits.CN30IE = 1; //Enables CN interrupt for CN30 (Pushbutton PB3)

    IPC4bits.CNIP = 7; //Set the CN interrupt priority to a 7
    IFS1bits.CNIF = 0; //Clears the CN interrupt flag
    IEC1bits.CNIE = 1; //Enable the CN interrupt (general)

    //    UARTDisplayTime(); // displays starting time of 00m : 00s

    return; // return to main



}

/*
 *  PushButton Check 
 *  Inputs: Nothing
 *  Return: The LED LATB8
 *
 */
void IOcheck() { // PB1 = RA2, PB2 = RA4, PB3 = RB4
//    //UARTDisplayTime(); // displays starting time of 00m : 00s
//
//
//    if ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)) {
//    } else if ((PORTAbits.RA2 == 0 && PORTBbits.RB4 == 0)) {
//    } else if ((PORTAbits.RA2 == 0 && PORTAbits.RA4 == 0)) {
//    } else if ((PORTAbits.RA4 == 0 && PORTBbits.RB4 == 0)) {
//    } else if (PORTAbits.RA2 == 0) { // PB1/bottom
//        if (minutes < 59 && countdown_on == 0) {
//            delay_ms(1000);
//            minutes++;
//        }
//    } else if (PORTAbits.RA4 == 0) { // PB2/middle
//        if (seconds < 59 && countdown_on == 0) {
//            delay_ms(1000);
//            seconds++;
//        }
//    } else if (PORTBbits.RB4 == 0) { // PB3/top
//        //delay_ms(2000);
//        uint16_t timePressed = 0;
//        while (PORTBbits.RB4 == 0) {
//            timePressed += 1;
//            delay_ms(1000);
//            
//        }
//
//        Disp2String('\n');
//        if (timePressed < 3) {
//            if (countdown_on == 1) {
//                delay_ms(100);
//                countdown_on = 0;
//            } else {
//                delay_ms(100);
//                countdown_on = 1;
//            }
//        } else {
//            minutes = 0; // Resets minutes back to zero
//            seconds = 0; // Resets seconds back to zero
//            countdown_on = 0; // turns off countdown;
//            //UARTDisplayTime(); // displays starting time of 00m : 00s
//            
//        }
//    }
//    //    else {
//    //
//    //    }
//
//
//    if (countdown_on == 1) {
//
//        if (minutes == 0 && (seconds >= 0 && seconds <= 0)) {
//            countdown_on = 0;
//            LATBbits.LATB8 = 1;
//        } else if (seconds == 0) {
//            minutes--;
//            seconds = 59;
//        } else {
//            LATBbits.LATB8 = 1 ? LATBbits.LATB8 == 0 : 0;
//            seconds--;
//            delay_ms(1000);
//        }
//
//
//    }

}


// clkval = 8
// clkval = 500 500 kHz
// clkval = 32 32 kHz

void NewClk(unsigned int clkval) {
    char COSCNOSC;
    if (clkval == 8) //8MHZ
    {
        COSCNOSC = 0x00;
    } else if (clkval == 500) //500kHZ
    {
        COSCNOSC = 0x66;
    } else if (clkval == 32) {
        COSCNOSC = 0x55;
    } else //default 32 kHZ
    {
        COSCNOSC = 0x55;
    }
    SRbits.IPL = 7;
    CLKDIVbits.RCDIV = 0;
    __builtin_write_OSCCONH(COSCNOSC);
    __builtin_write_OSCCONL(0x01);
    OSCCONbits.OSWEN = 1;
    while (OSCCONbits.OSWEN == 1) {
    }
    SRbits.IPL = 0; //enable interrupts



}

//void UARTDisplayTime() // Function which will output the time when called
//{
//    NewClk(8);
//    if (countdown_on == 1 && minutes == 0 && seconds == 1) {
//        Disp2String("\r                        \r");
//        Disp2Dec(minutes); // Displaying the minutes in decimal (function was edited to have 2 values)
//        Disp2String("m:"); // Displaying the minutes unit
//        Disp2Dec(00); // Displaying the seconds in decimal (function was edited to have 2 values)
//        Disp2String("s"); // Displaying the seconds unit}
//        Disp2String("  -- ALARM\r"); // Displaying the seconds unit}
//    } else {
//        Disp2String("\r                        \r");
//        Disp2Dec(minutes); // Displaying the minutes in decimal (function was edited to have 2 values)
//        Disp2String("m:"); // Displaying the minutes unit
//        Disp2Dec(seconds); // Displaying the seconds in decimal (function was edited to have 2 values)
//        Disp2String("s\r"); // Displaying the seconds unit}
//
//    }
//    NewClk(32);
//}




// Gets called when any button is pressed (exits out of idle) instead of using polling

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0; // clear IF flag
    IOcheck(); // Call IOCheck to determine what to do next
    Nop(); // debounce buffer
    return; // return to main
}