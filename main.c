#include "config.h"
#include "xc.h"
#include "max7221.h"
#include "ntc_temperature.h"

// Set the lowest possible duty cycle that we can have.
const unsigned int MIN_DUTY_CYCLE = 100;
const unsigned int MAX_DUTY_CYCLE = 255;

// Set the minimum temperature to 18C
const float MIN_TEMP = 20.0;
const float MAX_TEMP = 40.0;

// Define our functions here (C requires function prototypes
// like this...stick this in header next time
void configure_adc(void);
void configure_pwm(void);
void set_fan_speed(float temp);

void init_oscillator()
{
    // SCS FOSC; IDLEN disabled; IRCF 2MHz; 
    OSCCON = 0x60;
    // INTSRC INTRC_31_25KHz; PLLEN disabled; PRISD disabled; SOSCGO disabled; 
    OSCCON2 = 0x08;
    // SPLLMULT 4xPLL; TUN 0; 
    OSCTUNE = 0x00;
    // ACTSRC SOSC; ACTUD enabled; ACTEN disabled; 
    ACTCON = 0x00;
    // Set the secondary oscillator
}

void init_pins()
{
    LATE = 0x00;    
    LATD = 0x00;    
    LATA = 0x00;    
    LATB = 0x00;    
    LATC = 0x00;    

    /**
    TRISx registers
    */    
    TRISE = 0x07;
    TRISA = 0xFF;
    TRISB = 0xF5;
    TRISC = 0x87;
    TRISD = 0xFF;

    /**
    ANSELx registers
    */   
    ANSELC = 0x84;
    ANSELB = 0x34;
    ANSELD = 0xFF;
    ANSELE = 0x07;
    ANSELA = 0x2F;
    
     /**
    WPUx registers
    */ 
    //WPUB = 0xFF;
    INTCON2bits.nRBPU = 0;
}

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
    PSTR1CON = 0b00010001;
    PR2 = 254;
    ECCP1AS = 0b00000000;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M3 = 1;
    //T2CON = 0b00001111;
    T2CON = 0b00000100;
    
    CCPR1L = 25;
}

// Algorithm to linearly adjust the fanspeed based on temperature.
void set_fan_speed(float temp)
{
    if(temp < MIN_TEMP)
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
            int new_duty_cycle = (int)MAX_DUTY_CYCLE * tTemp;
            CCPR1L = new_duty_cycle;
        }
    }
}

void main(void)
{
    init_pins();
    init_oscillator();
    SPI1_Initialize();
    
    float temp = 0;
    // Set all pins RD1-7 to inputs, RD0 (MSB is set to output)
    //TRISD = 0b11111110;
    // RC2 is PWM pin
    TRISCbits.RC2 = 0;
    
    configure_adc();
    configure_pwm();
    int ticks = 0;
    
    while (1)
    {  
        MAX_7221_INIT();
        temp = get_temperature(0);
        set_fan_speed(temp);
        if(ticks % 125 == 0) {
            MAX_7221_WRITE_FLOAT(temp);
            ticks = 0;
        }
        ticks++;
    }
}