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
            // 受信が有効にならない間は待機
            c = Serial_read();
            if(c != -1){
                // 正しく受信できた場合
                buf[receive_index] = 0xff & c;
                receive_index++;
            }
            if(receive_index > BUF_SIZE) break;
        }while(c != 0x0a );

        // 送信フェーズ
        uart0_puts(buf);
    }

    return 0;
}