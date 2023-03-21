#include "../peripherals/rpi_peripherals.h"
#include "../gpio/rpi_gpio.h"
#include "rpi_uart.h"

int Serial_begin(int baudrate)
{
    int ibrd = 0;
    int fbrd = 0;

    if(baudrate == 9600)
    {
        // (3000000 / (16 * 9600) = 19.53125
        // (0.53125*64)+0.5 = 34
        ibrd = 19;
        fbrd = 34;

    }
    else if(baudrate == 115200)
    {
        // (3000000 / (16 * 115200) = 1.627
        // (0.627*64)+0.5 = 40
        ibrd = 1;
        fbrd = 40;
    }
    else
    {
        return -1;
    }

    // UARTの無効化
    *UART0_CR = 0;

    // ポートの設定
    pinMode(14, PIN_MODE_INPUT_PULLDOWN);
    pinMode(15, PIN_MODE_INPUT_PULLDOWN);
    pinMode(14, PIN_MODE_ALT0);
    pinMode(15, PIN_MODE_ALT0);

    // ボーレートの設定
    *UART0_IBRD = ibrd;
    *UART0_FBRD = fbrd;

    // 8bit + FIFO en
    *UART0_LCRH = 0x70;

    // RXE en TXE en UART en
    *UART0_CR = 0x0301;

    return 0;
}


/*
void Serial_end();
int Serial_available(void);
int Serial_write(char *buf,int len);
int Serial_read(void);
void setSerialTimeout(int read_ms,int write_ms)
int uart0_putc(int c);
*/