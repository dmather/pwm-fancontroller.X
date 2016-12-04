/* 
 * File:   max7221.h
 * Author: daniel
 *
 * Created on November 13, 2016, 8:32 PM
 */

#ifndef MAX7221_H
#define	MAX7221_H

#include "spi.h"
#define _XTAL_FREQ  8000000

void MAX_7221_INTENSITY(unsigned char value);

void MAX_7221_SCAN_LIMIT(unsigned char limit);

// Reg refers to the MAX7221 register
void MAX_7221_WRITE(unsigned int reg, unsigned int value);

void MAX_7221_WRITE_NUMBER(unsigned int value);

void MAX_7221_WRITE_FLOAT(float value);

void MAX_7221_INIT();

void MAX_7221_TESTMODE(unsigned char enable);

#endif	/* MAX7221_H */

