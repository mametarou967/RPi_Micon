#include "rpi_lib/rpi_type.h"
#include "rpi_lib/rpi.h"
// #include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include "rpi_lib/delay/rpi_delay.h"

#define PHY_PERI_ADDR(x)        (0x20000000 + (x))
// UART peripheral
#define UART0_BASE      0x00201000
#define UART0_DR         ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x00))
#define UART0_IBRD       ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x24))
#define UART0_FBRD       ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x28))
#define UART0_LCRH       ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x2C))
#define UART0_CR         ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x30))


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

    while(1)
    {
        delay(1000);
        *UART0_DR = 0x00ff & 'a';
    }

    return 0;
}