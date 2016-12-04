#include "xc.h"
#include "max7221.h"
#include <stdio.h>

void MAX_7221_INTENSITY(unsigned char value)
{
    LATCbits.LATC6 = 0;
    SPI1_Exchange8bit(0x0a);
    SPI1_Exchange8bit(value);
    LATCbits.LATC6 = 1;
}

void MAX_7221_SCAN_LIMIT(unsigned char limit)
{
    LATCbits.LATC6 = 0;
    SPI1_Exchange8bit(0x0b);
    SPI1_Exchange8bit(limit);
    LATCbits.LATC6 = 1;
}

// Reg is the register
void MAX_7221_WRITE(unsigned int reg, unsigned int value)
{
    LATCbits.LATC6 = 0;
    SPI1_Exchange8bit(reg);
    SPI1_Exchange8bit(value);
    LATCbits.LATC6 = 1;
}

void MAX_7221_WRITE_NUMBER(unsigned int value)
{
    unsigned int d0, d1, d2;
    d0 = (value%1000)/100;
    d1 = (value%100)/10;
    d2 = (value%10);
    MAX_7221_WRITE(0x01, (unsigned int)d0);
    MAX_7221_WRITE(0x02, (unsigned int)d1);
    MAX_7221_WRITE(0x03, (unsigned int)d2);
}

// Need to take float value and 
void MAX_7221_WRITE_FLOAT(float value)
{
    if(value > 99.9)
        return;
    if(value < 0)
        return;
     
    // Leave extra digit for NULL terminator.
    char digits[4];
    
    sprintf(digits, "%.1f", value);
    if(digits[1] == '.') {
        unsigned int dig1 = (digits[0] - '0')|0x80;
        MAX_7221_WRITE(0x01, dig1);
    } else {
        MAX_7221_WRITE(0x01, digits[0]);
    }
    if(digits[2] = '.') {
        unsigned int dig2 = (digits[1] - '0')|0x80;
        MAX_7221_WRITE(0x02, dig2);
    } else {
        MAX_7221_WRITE(0x02, digits[1]);
    }
    MAX_7221_WRITE(0x03, digits[3]);
}

void MAX_7221_INIT()
{
    MAX_7221_INTENSITY(0x0d);
    MAX_7221_SCAN_LIMIT(0x02); 
    MAX_7221_WRITE(0x09, 0x07);
    MAX_7221_WRITE(0x0c, 0x01);
}

void MAX_7221_TESTMODE(unsigned char enable)
{
    if(enable) {
        MAX_7221_WRITE(0x0f, 0x01);
    } else {
        MAX_7221_WRITE(0x0f, 0x00);
    }
}