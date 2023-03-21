#include "rpi_lib/rpi_type.h"
#include "rpi_lib/rpi.h"
#include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include "rpi_lib/delay/rpi_delay.h"
#include "rpi_lib/uart/rpi_uart.h"

#define BUF_SIZE        256

int main(void){

    char buf[BUF_SIZE];
    int receive_index = 0;

    rpi_init();

    Serial_begin(115200);

    while(1)
    {
        // 受信フェーズ

        // bufを空にする
        for(receive_index = 0;receive_index < BUF_SIZE;receive_index++)
            buf[receive_index] = 0;

        uart0_gets(buf,BUF_SIZE);

        // 送信フェーズ
        uart0_puts(buf);
    }

    return 0;
}