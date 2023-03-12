#include "../peripherals/rpi_peripherals.h"
#include "rpi_timer.h"

static volatile unsigned long long get_systime()
{
    unsigned long long t ;
    unsigned int chi;
    unsigned int clo;

    chi = *SYST_CHI;
    clo = *SYST_CLO;

    if(chi != *SYST_CHI)
    {
        chi = *SYST_CHI;
        clo = *SYST_CLO;
    }

    t = chi;
    t = t << 32;
    t += clo;

    return t;
}

unsigned long long millis(void)
{
    return get_systime() / 1000;
}

unsigned long long micros(void)
{
    return get_systime();
}

