#include "ntc_temperature.h"
#include "xc.h"

// Default unsigned int is 16-bit
unsigned adc_convert(int channel)
{
    // Set channel to be 0 (RA0)
    ADCON0bits.CHS = channel;
    // Start analog to digital conversion
    ADCON0bits.GO = 1;
    while(ADCON0bits.DONE);
    // Shift the high bits 8 places and tack the low bits on the end
    // Must be a 16-bit variable
    return(ADRESH<<8)|ADRESL;
}

// Get the approximate temperature using an NTC
float get_temperature(int sensor)
{
    float Vout;
    // R1 is a low temp-co resistor
    unsigned int R1 = 10000;
    // R2 is a nominal 10k thermistor at 25C
    unsigned int R2 = 10000;
    float Rth;
    float Tn = 25+273.15;
    unsigned int Bth = 4073;
    float temp;
    // ADC spits out unsigned 16-bit int somewhere
    // between 0-1023 so we need to convert that to
    // a voltage by multiplying against the 5v
    // resolution of the ADC.
    Vout = adc_convert(sensor) * ADC_RESOLUTION;
    Rth = ((5 * R2) / (5 - Vout)) - R2;
    temp = (Bth*Tn) / (Bth + log(Rth / R1) * Tn);
    // Convert temperature from kelvin to Celsius
    temp = temp - 273.15;

    return temp;
}