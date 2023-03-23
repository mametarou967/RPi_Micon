#include "rpi_lib/rpi_type.h"
#include "rpi_lib/rpi.h"
#include "rpi_lib/timer/rpi_timer.h"
#include "rpi_lib/delay/rpi_delay.h"
#include <stdio.h>
#include "stdint.h"

int main(void){
    rpi_init();

    char str[256];
    uint64_t t;

    while(1)
    {
        t = micros();
        sprintf(str,"sysclock: %lld (us)\n",t);
        printf("%s",str);
        delay(1000);
    }


    return 0;
}