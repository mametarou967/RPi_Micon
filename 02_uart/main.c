#include "rpi_lib/rpi_type.h"
#include "rpi_lib/rpi.h"
#include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include "rpi_lib/delay/rpi_delay.h"
#include "rpi_lib/uart/rpi_uart.h"

#define BUF_SIZE        256

int main(void){

    rpi_init();

    Serial_begin(115200);

    int c;
    char buf[BUF_SIZE];
    int send_index = 0;
    int receive_index = 0;
    
    while(1)
    {
        // 受信フェーズ
        for(receive_index = 0;receive_index < BUF_SIZE;receive_index++)
            buf[receive_index] = 0;
        c = 0;
        receive_index = 0;
        do{
            while(*UART0_FR & (1 << 4));
            c = *UART0_DR;
            buf[receive_index] = 0xff & c;
            receive_index++;
            if(receive_index > BUF_SIZE) break;
        }while(c != 0x0a );

        // 送信フェーズ
        send_index = 0;
        while(buf[send_index] != '\0')
        {
            // 送信FIFOの満タンが解除されるまで待つ
            while(*UART0_FR & (1 << 5));
            *UART0_DR = buf[send_index];
            send_index++;
        }
    }

    return 0;
}