#include "rpi_lib/rpi.h"

#define GPFSEL1     0x20200004
#define GPSET0      0x2020001C
#define GPCLR0      0x20200028

#define SYST_CLO    0x20003004
#define SYST_CHI    0x20003008

volatile unsigned long long get_systime(void)
{
    unsigned long long t ;
    unsigned int chi;
    unsigned int clo;

    chi = *(volatile unsigned int *)SYST_CHI;
    clo = *(volatile unsigned int *)SYST_CLO;

    if(chi != *(volatile unsigned int *)SYST_CHI)
    {
        chi = *(volatile unsigned int *)SYST_CHI;
        clo = *(volatile unsigned int *)SYST_CLO;
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
    *(volatile unsigned int *)GPFSEL1 |= 0x01 << (3*6);

    while(1)
    {
        // LED点灯
        *(volatile unsigned int *)GPCLR0 |= 0x01 << 16;
        
        delay_ms(3000);
        
        // LED消灯
        *(volatile unsigned int *)GPSET0 |= 0x01 << 16;
        
        delay_ms(3000);
    }

    return 0;
}