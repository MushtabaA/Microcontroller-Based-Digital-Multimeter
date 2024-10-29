/*
 * File: CTMU.h
 * Author: Ahad Ali, Mushtaba Al Yasseen, and Parbir Lehal
 * Created on December 02, 2021, 5:26 PM
 */

#ifndef CTMU_H
#define	CTMU_H

#include "xc.h"
#include "IOs.h"
#include "UART2.h"
#include "TimeDelay.h"

//The function declarations need to be called here
void setup(void);
uint16_t do_CTMU(void);
#endif