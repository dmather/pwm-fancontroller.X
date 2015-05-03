/*
 * Author: Daniel Mather
 *
 * temperature controlled PWM fan controller. This is a project attempt at
 * at multiple channel smart fan controller.
 */

#include <math.h>
#include <p18f45k50.h>
#include "ntc_temperature.h"

// Config Bits
#pragma config CPUDIV = NOCLKDIV
#pragma config PLLSEL = PLL4X
#pragma config PCLKEN = ON
#pragma config FOSC = INTOSCIO
#pragma config WDTEN = OFF
#pragma config MCLRE = ON
#pragma config PBADEN = OFF

// Define our functions here (C requires function prototypes
// like this...stick this in header next time
void configure_adc(void);
void configure_pwm(void);

// Configure and enable the ADC on the selected PORT
void configure_adc(void)
{
    // Set pin RA0 to be an input
    TRISAbits.RA0 = 1;
    // Set pin RA0 to be an analog input
    ANSELAbits.ANSA0 = 1;
    // Set AD results to be right justified
    ADCON2bits.ADFM = 1;
    ADCON2bits.ACQT = 0b111;
    ADCON2bits.ADCS = 0b101;
    // Set positive voltage ref to be VDD
    ADCON1bits.PVCFG0 = 0;
    // Set ground voltage ref to be VSS
    ADCON1bits.NVCFG1 = 0;
    // Enable ADC
    ADCON0bits.ADON = 1;
}

void configure_pwm(void)
{
    CCP1CON = 0b00001100;
    //CCP1CONbits.CCP1M2 = 1;
    //CCP1CONbits.CCP1M3 = 1;
    T2CON = 0b00000100;
    PR2 = 255;
    CCPR1L = 25;
}

void main(void)
{
    // OSCCON Set main clock to 16MHz
    OSCCON = 0b01111100;
    float temp = 0;
    // Set all pins RD1-7 to inputs, RD0 (MSB is set to output)
    //TRISD = 0b11111110;
    TRISCbits.RC2 = 0;
    configure_adc();
    configure_pwm();
    while(1)
    {
        // Get temperature on pin RA0
        temp = temperature_get(0);
        if(temp >= 20 && temp <= 22)
            CCPR1L = 180;
        else if(temp >= 22 && temp <= 24) {
            CCPR1L = 185;
            DC1B0 = 1;
            DC1B1 = 1;
        }
        else if(temp >= 24 && temp <= 26)
            CCPR1L = 190;
        else if(temp >= 26 && temp <= 28)
            CCPR1L = 195;
        else if(temp >= 28 && temp <= 30)
            CCPR1L = 200;
        else if(temp >= 30 && temp <= 32)
            CCPR1L = 205;
        else if(temp >= 32 && temp <= 34)
            CCPR1L = 235;
        else
            CCPR1L = 255;
    }
}