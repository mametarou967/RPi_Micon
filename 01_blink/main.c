#include "rpi_lib/rpi.h"
#include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include "rpi_lib/delay/rpi_delay.h"


int main(void){

    rpi_init();

    // outputに設定
    // *GPIO_GPFSEL1 |= 0x01 << (3*6);
    pinMode(16, PIN_MODE_OUTPUT);

    while(1)
    {
        // LED点灯
        digitalWrite(16,PIN_VALUE_LOW);
        // *GPIO_GPCLR0 |= 0x01 << 16;
        
        delay(3000);
        
        // LED消灯
        digitalWrite(16,PIN_VALUE_HIGH);
        //*GPIO_GPSET0 |= 0x01 << 16;
        
        delay(3000);
    }

    return 0;
}