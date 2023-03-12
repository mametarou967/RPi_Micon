#include "rpi_lib/rpi.h"
#include "rpi_lib/peripherals/rpi_peripherals.h"

volatile unsigned long long get_systime(void)
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

void delay_ms(unsigned int delay)
{
    volatile unsigned long long alermTime = 0;
    alermTime = get_systime() + delay * 1000; // get_systemは10^-6秒に1回カウントされる

    while(get_systime() < alermTime);

    return;
}


int main(void){

    rpi_init();

    // outputに設定
    *GPIO_GPFSEL1 |= 0x01 << (3*6);

    while(1)
    {
        // LED点灯
        *GPIO_GPCLR0 |= 0x01 << 16;
        
        delay_ms(3000);
        
        // LED消灯
        *GPIO_GPSET0 |= 0x01 << 16;
        
        delay_ms(3000);
    }

    return 0;
}