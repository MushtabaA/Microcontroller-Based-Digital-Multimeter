/* 
 * File:   TimeDelay.h
 * Author: Ahad Ali, Mushtaba Al Yasseen, and Parbir Lehal
 *
 * Created on October 12, 2021, 3:26 PM
 */


#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include "xc.h"

//The function declarations need to be called here
void delay_ms(int time_ms);
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void);
#endif