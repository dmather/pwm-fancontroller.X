/*
 * Author: Daniel Mather
 * This is a NTC temperature sensor library it contains code for reading
 * NTC thermistors using PIC18F micros. It also contains some ADC code.
 */

#ifndef NTC_TEMPATURE_H
#define	NTC_TEMPATURE_H

#include "math.h"

// We must use a decimal somewhere otherwise this
// is reduced to integer division (which messes up everything).
const float ADC_RESOLUTION = 5/1024.0;

// Default unsigned int is 16-bit
unsigned adc_convert(unsigned int channel);

// Get the approximate temperature using an NTC
float get_temperature(int sensor);

#endif /* NTC_TEMPERATURE_H */