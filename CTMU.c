/*
 * File: CTMU.c
 * Author: Ahad Ali, Mushtaba Al Yasseen, and Parbir Lehal
 * Created on December 02, 2021, 5:26 PM
 */

#include "xc.h"
#include "CTMU.h"

#define COUNT 500         // @ 8MHz = 125us.
#define ETIME COUNT * 2.5 // time in us
#define RCAL .027         // R value is 4200000 (4.2M)
#define ADSCALE 1023      // for unsigned conversion 10 sig bits
#define ADREF 3.3         // Vdd connected to A/D Vr+

void setup(void) { // CTMUCON = CTMU Control Register
    
    CTMUCON = 0x0000;       // make sure CTMU is disabled
    CTMUCONbits.CTMUEN = 0; // Disable the CTMU
    CTMUCONbits.TGEN = 1; // setup all the CTMU bits
    CTMUCONbits.EDG1POL = 1;
    CTMUCONbits.EDG2POL = 1;
    
    CTMUICON = 0x0000;
    CTMUICONbits.IRNG0 = 1; // .55uA, Nominal - No Adjustment
    CTMUICONbits.IRNG1 = 0; // .55uA, Nominal - No Adjustment

    /*
     * Set up AD Converter
     */
    AD1CON1bits.ADON = 0;     // Turn off ADC, turn on before sampling in do_ADC()
    AD1CON1bits.ADSIDL = 0;   // Continue ADC operation in idle mode
    AD1CON1bits.FORM = 0b00;  // 0b00: Int, 01: Signed Int, 10:Fractional, 11:Signed Fract
    AD1CON1bits.SSRC = 0b000; // Start ADC conversion Time set by SAMC has passed
    AD1CON1bits.ASAM = 0;     // Next Sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 0;     // Set this bit in ADC routine to start sampling

    // Sampling time and ADC Clk select
    AD1CON2bits.VCFG = 0b000;  // Vref+=AVDD   Vref-=AVSS
    AD1CON2bits.CSCNA = 0;     // Do not scan inputs; use channel selected by CH0SA bits
    AD1CON2bits.SMPI = 0b0000; // Any ADC interrrups occur at completion of each sample/convert seq.
    AD1CON2bits.BUFM = 0;      // Buffer confugured as 1 16 word buffer
    AD1CON2bits.ALTS = 0;      // Always use MUXA input MUX settings
    AD1CON3bits.ADRC = 0;      // 0: Use System clk; 1: ADC uses internal RC clock for idle or sleep mode
    AD1CON3bits.SAMC = 0;
    AD1CON3bits.ADCS = 0; // ADCS conversion clock select bits. Ignored if using internal AD RC clock.

    // MUX settings
    AD1CHSbits.CH0NB = 0;      // Ch0 MUXB -ve Input is VR-
    AD1CHSbits.CH0SB = 0b0011; // YOUR ADC CHANNEL HERE AN3
    AD1CHSbits.CH0NA = 0;      // Ch0 -ve Input is VR-
    AD1CHSbits.CH0SA = 0b0011; // YOUR ADC CHANNEL HERE AN3

    // IO port selection
    TRISBbits.TRISB1 = 1;  // SET YOUR PIN AS AN INPUT RB1
    AD1PCFGbits.PCFG3 = 0; // SET YOUR PIN AS A VOLTAGE AN3/RB1/PIN5
    AD1CON1bits.ADON = 1;

    CTMUCONbits.CTMUEN = 1; // Enable the CTMU
}

unsigned int do_CTMU(void) {
    int j = 0;
    float CTMUISrc = 0.0;
    float CTMUCap = 0.0;
    float Vavg = 0.0;
    float VTot = 0.0;
    float Vcal = 0.0;
    float Vread = 0.0;

    setup();
    for (j = 0; j < 10; j++) {
        AD1CON1bits.SAMP = 1;    // Manual sampling start
        CTMUCONbits.IDISSEN = 1; // drain any charge on the circuit
        delay_ms(62.5 * 0.001);  // wait 62.5 us
        CTMUCONbits.IDISSEN = 0; // end drain of circuit

        CTMUCONbits.EDG1STAT = 1; // Begin charging the circuit
        delay_ms(62.5 * 0.001);   // wait for 62.5 us for circuit to charge
        CTMUCONbits.EDG1STAT = 0; // Stop charging circuit and begin

        IFS0bits.AD1IF = 0;     // make sure A/D Int not set
        AD1CON1bits.SAMP = 0;   // begin A/D conversion
        while (!IFS0bits.AD1IF) // Wait for A/D conversion to complete
        {
        }
        Vread = ADC1BUF0;   // Get the value from the A/D converter
        IFS0bits.AD1IF = 0; // Clear AD1IF
        VTot += Vread;      // Add the reading to the total
    }
    //using multiple loops, so have to divide to average
    Vavg = (VTot / 10); // Average of 10 readings
    Vcal = (Vavg / ADSCALE * ADREF);
    CTMUISrc = Vcal / RCAL; // CTMUISrc is in 1/100ths of uA
    CTMUCap = (CTMUISrc * ETIME / Vcal) / 100;
    
    VTot = 0;
    unsigned int a = (unsigned int) CTMUCap;
    return (a);
}
