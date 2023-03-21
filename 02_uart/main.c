#include "rpi_lib/rpi_type.h"
#include "rpi_lib/rpi.h"
// #include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include "rpi_lib/delay/rpi_delay.h"

#define PHY_PERI_ADDR(x)        (0x20000000 + (x))
// UART peripheral
#define UART0_BASE      0x00201000
#define UART0_DR         ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x00))
#define UART0_FR         ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x18))
#define UART0_IBRD       ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x24))
#define UART0_FBRD       ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x28))
#define UART0_LCRH       ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x2C))
#define UART0_CR         ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x30))

#define BUF_SIZE        256

int main(void){

    rpi_init();

    // UARTの無効化
    *UART0_CR = 0;

    // ポートの設定
    pinMode(14, PIN_MODE_INPUT_PULLDOWN);
    pinMode(15, PIN_MODE_INPUT_PULLDOWN);
    pinMode(14, PIN_MODE_ALT0);
    pinMode(15, PIN_MODE_ALT0);

    // ボーレートの設定
    *UART0_IBRD = 1;
    *UART0_FBRD = 40;

    // 8bit + FIFO en
    *UART0_LCRH = 0x70;

    // RXE en TXE en UART en
    *UART0_CR = 0x0301;

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