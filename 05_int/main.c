#include "rpi_lib/rpi.h"
#include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include <stdio.h>
#include <stdint.h>

void set_vector_table(void)
{
    extern void *_initial_vector_start;
    extern void *_initial_vector_end;

    volatile unsigned int *vec = 0;
    volatile unsigned int *p;
    volatile unsigned int *s = (unsigned int *) &_initial_vector_start;
    volatile unsigned int *e = (unsigned int *) &_initial_vector_end;
    
    printf("Vector table check\n");
    printf("Addr : Hex\n");

    for(p = s ; p < e ; p++)
    {
        *vec = *p;
        printf("0x%02x : 0x%08x\n",vec,*vec);
        vec++;
    }
}

// タイマー割込み処理
void timerIRQ_handler(void)
{
    static unsigned int led = 0;

    if(led)
    {
        printf("LED: ON\n");
        digitalWrite(16,PIN_VALUE_LOW);
    }
    else
    {
        printf("LED: OFF\n");
        digitalWrite(16,PIN_VALUE_HIGH);
    }

    led = !led;
}

void IRQ_handler(void)
{
    // 割込みを禁止
    disable_IRQ();

    printf("get IRQ in IRQ handler\n");
    printf("CPSR (in IRQ handler) = 0x%08x\n",getmode());

    if(*INTERRUPT_IRQ_BASIC_PENDING & 0x01 != 0)
    {
        // タイマー割込み
        printf("Timer IRQ start\n");
        printf("Timer Raw IRQ before: 0x%08x\n",*TIMER_RAWIRQ);
        printf("irq_pending0 before 0x%08x\n",*INTERRUPT_IRQ_BASIC_PENDING);

        // タイマー割込み処理
        timerIRQ_handler();

        // 割り込みフラグクリア
        *TIMER_IRQ_CLR = 0;

        // フラグがクリアされたかチェック
        printf("irq pending0 after 0x%08\n",*INTERRUPT_IRQ_BASIC_PENDING);
        printf("Timer Raw IRQ after 0x%08x\n",*TIMER_RAWIRQ);
        printf("Timer IRQ end\n");
    }
    
    // 割込みを許可
    enable_IRQ();
    return;
}

int main(void)
{
    rpi_init();

    set_vector_table();

    // すべての割込み不可
    *INTERRUPT_DISABLE_BASIC_IRQS = 0xffffffff;
    *INTERRUPT_DISABLE_IRQS1 = 0xffffffff;
    *INTERRUPT_DISABLE_IRQS2 = 0xffffffff;
    *INTERRUPT_FIQ_CTRL = 0;

    // タイマ割り込み設定
    *INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

    // 設定のため一旦タイマー停止
    *TIMER_CONTROL &= 0xffffff00;

    // timer clock を1MHzに設定
    // (0xF9=249: timer clock = 250MHz/(249+1))
    *TIMER_PREDIVIDER = 0x000000F9;

    // タイマー設定(4sec)
    *TIMER_LOAD = 4000000-1;
    *TIMER_RELOAD = 4000000-1;

    // 割り込みフラグをクリア
    *TIMER_IRQ_CLR = 0;

    // タイマー開始
    // Timer enable , 32bit Timer
    *TIMER_CONTROL |= 0x000000A2;

    // 割り込み許可
    *INTERRUPT_ENABLE_BASIC_IRQS = 0x01;

    // IRQ許可
    enable_IRQ();

    while(1);

    return 0;
}
