#include "rpi_gpio.h"
#include "../rpi_type.h"
#include "../peripherals/rpi_peripherals.h"

static unsigned int gpfselWriteData(PIN_MODE pinMode);
static vu32_t * gpfselAddr(int pin)
{
    if(pin < 0) return 0;

    if(pin < 10)       return GPIO_GPFSEL0;
    else if (pin < 20) return GPIO_GPFSEL1;
    else if (pin < 30) return GPIO_GPFSEL2;
    else if (pin < 40) return GPIO_GPFSEL3;
    else if (pin < 50) return GPIO_GPFSEL4;
    else if (pin < PIN_INDEX_MAX) return GPIO_GPFSEL5;

    return 0;
}

void pinMode(int pin, PIN_MODE pinMode)
{
    vu32_t *gpio_gpfsel;
    int dataIndex = 0;
    unsigned int writeData = 0;
    // pinの値が範囲外の時は何もしない

    // 書き込み先のレジスタを求める
    gpio_gpfsel = gpfselAddr(pin);
    // レジスタ不定の場合は何も実施しない
    if(gpio_gpfsel == 0) return;
    // pin番号からデータインデックスを求める
    dataIndex = pin % 10;
    // 該当箇所をいったん000にする
    *gpio_gpfsel &= ~(0x7 << (dataIndex * 3));
    // 値を書き込む
    *gpio_gpfsel |= gpfselWriteData(pinMode) << (dataIndex * 3);

    // pin modeが入力だった場合、Pull-up/downを設定する
    if(
        (pinMode == PIN_MODE_INPUT) ||
        (pinMode == PIN_MODE_INPUT_PULLUP) ||
        (pinMode == PIN_MODE_INPUT_PULLDOWN))
    {
        unsigned int i = 0;
        // step1.Write to GPPUD to set the requiredcontrol signal
        unsigned int gppud = 0;
        if(pinMode == PIN_MODE_INPUT) gppud = 0x0;
        else if(pinMode == PIN_MODE_INPUT) gppud = 0x1;
        else if(pinMode == PIN_MODE_INPUT_PULLDOWN) gppud = 0x2;
        
        *GPIO_GPPUD = gppud;
        // step2.wait 150 cycles
        for(i=0;i<150;i++){
            // nop
            asm("mov r0,r0");
        }
        // step3.Write to GPPUDCLK0/1
        if(pin > 0x1f)
        {
            *GPIO_GPPUDCLK1 = (pin >> 5);
        }
        else
        {
            *GPIO_GPPUDCLK0 = pin;
        }
        // step4..wait 150 cycles
        for(i=0;i<150;i++){
            // nop
            asm("mov r0,r0");
        }
        //step5/6 remove
        *GPIO_GPPUD = 0;
        *GPIO_GPPUDCLK0 = 0;
        *GPIO_GPPUDCLK1 = 0;
    }
    return ;
}

void digitalWrite(int pin,PIN_VALUE pinValule)
{

}

int digitalRead(int pin)
{

}


static unsigned int gpfselWriteData(PIN_MODE pinMode)
{
    switch(pinMode)
    {
    case PIN_MODE_INPUT:
    case PIN_MODE_INPUT_PULLUP:
    case PIN_MODE_INPUT_PULLDOWN:
        return 0x0;
        break;
    case PIN_MODE_ALT0:
        return 0x4;
        break;
    case PIN_MODE_ALT1:
        return 0x5;
        break;
    case PIN_MODE_ALT2:
        return 0x6;
        break;
    case PIN_MODE_ALT3:
        return 0x7;
        break;
    case PIN_MODE_ALT4:
        return 0x3;
        break;
    case PIN_MODE_ALT5:
        return 0x2;
        break;
    case PIN_MODE_OUTPUT:
        return 0x1;
        break;
    default:
        return 0x0;
        break;
    }
}