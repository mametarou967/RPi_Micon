#include "../timer/rpi_timer.h"
#include "rpi_delay.h"

void delayMicroseconds(unsigned int us)
{
    volatile unsigned long long alermTime = 0;
    alermTime = micros() + us ; // get_systemは10^-6秒に1回カウントされる

    while(micros() < alermTime);

    return;
}

void delay(unsigned int ms)
{
    volatile unsigned long long alermTime = 0;
    alermTime = millis() + ms; // get_systemは10^-6秒に1回カウントされる

    while(millis() < alermTime);

    return;
}