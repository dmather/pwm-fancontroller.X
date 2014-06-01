#include <xc.h>
#include <math.h>

# Configuration bits
__CONFIG();

//
#define NTC_T1 25.0
#define NTC_T2 50.0
#define NTC_R1 10000.0
#define NTC_R2 7500.0

void main()
{
    double B25_50 = (log(NTC_R1) - log(NTC_R2))/(1/NTC_T1 - 1/NTC_T2);
}