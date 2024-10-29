
/* 
 * File: ADC.c
 * Authors: Ahad Ali, Mushtaba Al Yasseen, Parbir Lehal
 * 
 */

#include "xc.h"
#include "ADC.h"
#include "IOs.h"
#include "UART2.h"
#include "CTMU.h"


unsigned int average;

void ADCinit(void) {

    AD1CON1bits.ADON = 0;
    AD1CON1bits.ADSIDL = 0;
    AD1CON1bits.FORM = 0b00;
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0;
    AD1CON1bits.SAMP = 0;

    // Sampling time and ADC Clk select
    AD1CON2bits.VCFG = 0b000;
    AD1CON2bits.CSCNA = 0;
    AD1CON2bits.SMPI = 0b0000;
    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0;
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = 0b11111;
    AD1CON3bits.ADCS = 0b00111111;

    // MUX settings
    AD1CHSbits.CH0NB = 0;
    AD1CHSbits.CH0SB = 0b0101;
    AD1CHSbits.CH0NA = 0;
    AD1CHSbits.CH0SA = 0b0101;

    // IO port selection
//    TRISAbits.TRISA3 = 1;
    
    TRISBbits.TRISB13 = 1;
    AD1PCFG = 0xFFFF;
    AD1PCFGbits.PCFG5 = 0;
    AD1CSSL = 0;

    // INT bit setup
    IPC3bits.AD1IP = 7;

    AD1CON1bits.ADON = 1;

    return;
}

// ADC interrupt subroutine

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void) {
    IFS0bits.AD1IF = 0; // Clear the ADC1 Interrupt Flag
}

// ADC conversion subroutine
// Returns single conversion 10bit ADC value in unsigned int form

unsigned int do_ADC(void) {

    unsigned int ADCvalue = 0;
    
    AD1CON1bits.SAMP = 1; // start sampling, conversion starts auto

        // after SSRC and SAMC settings

    while (AD1CON1bits.DONE == 0) {
    }
    AD1CON1bits.SAMP = 0; // stop the sampling

    ADCvalue = ADC1BUF0;

    return (ADCvalue);
}

/*
 * Does ADC on AN5 and displays value depending on the port later used as input,
   such as voltage, resistance, or capacitance
   markers on terminal
 */

// PB1/bottom
// PB2/middle
// PB3/top

void DispADC(void) {
    
    TRISBbits.TRISB13 = 0; // Not using pin16/RB13/AN11 as input
    TRISAbits.TRISA3 = 0; // Not using pin8/RA3/AN5 as input
    TRISBbits.TRISB1 = 0; // Not using pin5/RB1/AN3 as input
    
    // If RA2 is pressed, we display voltage value
    if (PORTAbits.RA2 == 0) {
        AD1CHSBITS.CH0SB = 0b0101;
        AD1CHSBITS.CH0SA = 0b0101;
        
        TRISAbits.TRISA3 = 1; // Setting pin8/RA3/AN5 as input
        
        uint16_t adcbuf; // var to store adcbufg value
        
        while (PORTABITS.RA2 == 0) {
            Disp2String("\r");
            NewClk(8);
            Disp2String("VOLTMETER Voltage = ");
            adcbuf = do_ADC();
            float voltage = adcbuf / 1023.0;
            voltage = voltage * 3.25;
            adcbuf = voltage;
            Disp2DecVolt(adcbuf);
            Disp2String("V");
            NewClk(32);
            delay_ms(1000);
            XmitUART2('\r', 1);
        }
        
        return;
    }
    
    // If RA4 is pressed, we display resistance value
    if (PORTAbits.RA4 == 0) {
        AD1CHSBITS.CH0SB = 0b0101;
        AD1CHSBITS.CH0SA = 0b0101;
        
        TRISBbits.TRISB13 = 1; // Setting pin16/RB13/AN11 as input
        
        uint16_t adcbuf; // var to store adcbufg value
        
        while (PORTABITS.RA4 == 0) {
            Disp2String("\r");
            NewClk(8);
            Disp2String("OHMMETER Resistance = ");
            adcbuf = do_ADC();
            float resistance = (adcbuf / 1023.0) * 1000.0;
            adcbuf = resistance;
            Disp2Dec(adcbuf);
            Disp2String("ohms");
            NewClk(32);
            delay_ms(1000);
            XmitUART2('\r', 1);
        }
        
        return;
    }
    
    // If RB4 is pressed, we display capacitance value
    if (PORTBbits.RB4 == 0) {
        
        uint16_t capacitanceVal; // var to store capacitanceVal value

        TRISBbits.TRISB1 = 1; // Setting pin5/RB1/AN3 as input
               
        while (PORTBbits.RB4 == 0) {
            Disp2String("\r");
            NewClk(8);
            Disp2String("Capacitance = ");
            capacitanceVal = do_CTMU();
            Disp2Dec(capacitanceVal);
            Disp2String(" micro Farads");
            NewClk(32);
            delay_ms(1000);
            XmitUART2('\r', 1);
        }
        
        return;
    }
}