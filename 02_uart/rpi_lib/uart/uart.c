#include "../peripherals/rpi_peripherals.h"
#include "../gpio/rpi_gpio.h"
#include "../timer/rpi_timer.h"
#include "rpi_uart.h"

static int readTimeoutMs = -1;
static int writeTimeoutMs = -1;

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

void Serial_end()
{
    // UARTの無効化
    *UART0_CR = 0;
    pinMode(14, PIN_MODE_INPUT);
    pinMode(15, PIN_MODE_INPUT);

    return ;
}

// 受信データが存在する場合は1
// 受信データが存在しない場合は0
int Serial_available(void)
{
    // 5bit目 RXFE 受信FIFOが空の時に1になる
    if(*UART0_FR & (1 << 4)) return 0;
    else return 1;
}


int Serial_write(char *buf,int len)
{
    int buf_index = 0;

    unsigned long long startMills;
    
    startMills = millis();

    for(buf_index = 0;buf_index < len;buf_index++)
    {
        while(1)
        {
            if(*UART0_FR & (1 << 5))
            {
                // 送信バッファが満タン
                if(writeTimeoutMs != -1)
                {
                    // 送信タイムアウトが設定されている場合は評価
                    if((millis() - startMills) > writeTimeoutMs)
                    {
                        // タイムアウトしたので終了
                        return -1;
                    }
                }
            }
            else
            {
                *UART0_DR = buf[buf_index];
            }
        }
    }

    return buf_index + 1;
}

int Serial_read(void)
{
    int c = 0;
    unsigned long long startMills;
    startMills = millis();

    while(1)
    {
        if(Serial_available())
        {
            // 受信データが存在する場合
            c = *UART0_DR;
            return c;
        }
        else
        {
            // 受信データが存在しない場合
            if(readTimeoutMs != -1)
            {
                // 送信タイムアウトが設定されている場合は評価
                if((millis() - startMills) > readTimeoutMs)
                {
                    // タイムアウトしたので終了
                    return -1;
                }
            }
        }
    }
    // ここに到達することはないはず
    return -1;
}

void setSerialTimeout(int read_ms,int write_ms)
{
    readTimeoutMs = read_ms;
    writeTimeoutMs = write_ms;

    return;
}

/*
int Serial_write(char *buf,int len);
int Serial_read(void);
int uart0_putc(int c);
*/