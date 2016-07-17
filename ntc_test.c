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
#pragma config FOSC = INTOSCIO
#pragma config WDTEN = OFF
#pragma config MCLRE = ON
#pragma config PBADEN = OFF

// Set the lowest possible duty cycle that we can have.
const unsigned int MIN_DUTY_CYCLE = 30;
const unsigned int MAX_DUTY_CYCLE = 255;

// Set the minimum temperature to 18C
const float MIN_TEMP = 18.0;
const float MAX_TEMP = 35.0;

// Define our functions here (C requires function prototypes
// like this...stick this in header next time
void configure_adc(void);
void configure_pwm(void);
void set_fan_speed(float temp);

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
    T2CON = 0b00000111;
    PR2 = 249;
    CCPR1L = 25;
}

// Algorithm to linearly adjust the fanspeed based on temperature.
void set_fan_speed(float temp)
{
    float tTemp = 0;
    if(temp <= MIN_TEMP)
    {
        CCPR1L = MIN_DUTY_CYCLE;
    }
    else 
    {
        float tTemp = temp / MAX_TEMP;
        if(tTemp >= 1)
        {
            CCPR1L = MAX_DUTY_CYCLE;
        }
        else
        {
            CCPR1L = (int)MAX_DUTY_CYCLE*tTemp;
        }
    }
}

void main(void)
{
    float temp = 0;
    // Set all pins RD1-7 to inputs, RD0 (MSB is set to output)
    //TRISD = 0b11111110;
    TRISCbits.RC2 = 0;
    configure_adc();
    configure_pwm();
    while(1)
    {
        // Get temperature on pin RA0
        temp = get_temperature(0);
        set_fan_speed(temp);
    }
}