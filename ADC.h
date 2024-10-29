

/* 
 * File: ADC.h
 * Authors: Ahad Ali, Mushtaba Al Yasseen, Parbir Lehal
*/

#ifndef ADC_H
#define ADC_H

#include "xc.h"

void ADCinit(void);
uint16_t do_ADC(void);
void DispADC(void);

void __attribute__((interrupt, no_auto_psv)) _ADC1Interrupt(void);

#endif