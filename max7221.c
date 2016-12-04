#include "xc.h"
#include "max7221.h"
#include <stdio.h>
#include <string.h>

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
void MAX_7221_WRITE(uint8_t reg, uint8_t value)
{
    LATCbits.LATC6 = 0;
    SPI1_Exchange8bit(reg);
    SPI1_Exchange8bit(value);
    LATCbits.LATC6 = 1;
}

void MAX_7221_WRITE_NUMBER(uint8_t value)
{
    uint8_t d0, d1, d2;
    d0 = (uint8_t)(value%1000)/100;
    d1 = (uint8_t)(value%100)/10;
    d2 = (uint8_t)(value%10);
    MAX_7221_WRITE(0x01, d0);
    MAX_7221_WRITE(0x02, d1);
    MAX_7221_WRITE(0x03, d2);
}

// Write out the float value to the screen
void MAX_7221_WRITE_FLOAT(const float value, const unsigned int max_digits)
{
    if(value > 99.9)
        return;
    if(value < 0)
        return;
     
    // Leave extra digit for NULL terminator.
    char digits[10];
    
    sprintf(digits, "%*f", max_digits, value);
    uint8_t i = 0;
    for(char *digit = digits; *digit != '\0'; ++digit) {
        if(i >= max_digits) break;
        if(i < strlen(digits)-1) {
            if(*(digit+1) == '.')
                MAX_7221_WRITE(++i, *digit | 0x80);
            else {
                switch(*digit) {
                    case '.':
                        break;
                    case '\0':
                        MAX_7221_WRITE(++i, 0x0f);
                        break;
                    default:
                        MAX_7221_WRITE(++i, *digit);
                        break;
                }
            }
        }
    }
}

void MAX_7221_INIT()
{
    MAX_7221_INTENSITY(0x06);
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