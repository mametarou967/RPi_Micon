#include "rpi_lib/rpi.h"
#include "rpi_lib/peripherals/rpi_peripherals.h"
#include "rpi_lib/gpio/rpi_gpio.h"
#include "rpi_lib/delay/rpi_delay.h"
#include <stdio.h>
#include <stdint.h>

#define SPIO_BASE       (0x00204000)
#define SPIO_CS         ((vu32_t *)PHY_PERI_ADDR(SPIO_BASE + 0x00))
#define SPIO_FIFO       ((vu32_t *)PHY_PERI_ADDR(SPIO_BASE + 0x04))
#define SPIO_CLK        ((vu32_t *)PHY_PERI_ADDR(SPIO_BASE + 0x08))

#define SPIO_CS_CSPOL0_HIGH     ((vu32_t)0x1 << 21)
#define SPIO_CS_CSPOL0_LOW      ((vu32_t)0x0 << 21)
#define SPIO_CS_TA_ACTIVE       ((vu32_t)0x1 << 7)
#define SPIO_CS_CSPOL_HIGH      ((vu32_t)0x1 << 6)
#define SPIO_CS_CSPOL_LOW       ((vu32_t)0x0 << 6)
#define SPIO_CS_CLEAR_TX        ((vu32_t)0x1 << 4)
#define SPIO_CS_CLEAR_RX        ((vu32_t)0x2 << 4)
#define SPIO_CS_CLEAR_ALL       ( SPIO_CS_CLEAR_TX | SPIO_CS_CLEAR_RX )

#define SPIO_CS_CS_CS0          ((vu32_t)0x00)
#define SPIO_CS_CS_CS1          ((vu32_t)0x01)
#define SPIO_CS_CS_CS2          ((vu32_t)0x02)


#define SPI0_CS_MODE_0x        ((vu32_t)0x00 << 3) // CPOL = 0
#define SPI0_CS_MODE_1x        ((vu32_t)0x01 << 3) // CPOL = 0
#define SPI0_CS_MODE_x0        ((vu32_t)0x00 << 2) // CPHA = 0
#define SPI0_CS_MODE_x1        ((vu32_t)0x01 << 2) // CPHA = 0
#define SPIO_CS_MODE_00         (SPI0_CS_MODE_0x | SPI0_CS_MODE_x0)
#define SPIO_CS_MODE_01         (SPI0_CS_MODE_0x | SPI0_CS_MODE_x1)
#define SPIO_CS_MODE_10         (SPI0_CS_MODE_1x | SPI0_CS_MODE_x0)
#define SPIO_CS_MODE_11         (SPI0_CS_MODE_1x | SPI0_CS_MODE_x1)

#define SPIO_CS_RXF_MASK        ((vu32_t)0x01 << 20) // 受信バッファ満タン
#define SPIO_CS_RXR_MASK        ((vu32_t)0x01 << 19) // 受信バッファ3/4以上
#define SPIO_CS_TXD_MASK        ((vu32_t)0x01 << 18) // 送信バッファ空あり
#define SPIO_CS_RXD_MASK        ((vu32_t)0x01 << 17) // 受信バッファデータあり
#define SPIO_CS_DONE_MASK       ((vu32_t)0x01 << 16) // 受信バッファデータあり

#define ADT7310_CMD_ADDR_STATUS     (0x00 << 3)
#define ADT7310_CMD_ADDR_CONFIG     (0x01 << 3)
#define ADT7310_CMD_ADDR_TEMP       (0x02 << 3)
#define ADT7310_CMD_READ            (0x01 << 6)
#define ADT7310_CMD_WRITE           (0x01 << 6)
#define ADT7310_CMD_CONTINUOUS      (0x01 << 2)

int main(void){
    uint32_t rawtemp;
    double   temp;

    rpi_init();

    // pin setting
    // CS
    pinMode( 8,PIN_MODE_ALT0);
    // MISO
    pinMode( 9,PIN_MODE_ALT0);
    // MOSI
    pinMode(10,PIN_MODE_ALT0);
    // SCLK
    pinMode(11,PIN_MODE_ALT0);
    
    // SPIO configure
    *SPIO_CS = 0;
    // CSPOL0 = HIGH , TA = 0 CS=CS0
    *SPIO_CS |= SPIO_CS_CSPOL0_LOW | SPIO_CS_CS_CS0 ;
    // 送受信バッファ初期化
    *SPIO_CS |= SPIO_CS_CLEAR_ALL;
    // clock setting
    *SPIO_CLK = 0;
    // MODE11
    *SPIO_CS |= SPIO_CS_MODE_11;

    // software reset
    // 通信開始(TA=1)
    *SPIO_CS |= SPIO_CS_TA_ACTIVE;
    // 送信可能待ち(wait TXD)
    while(!(*SPIO_CS & SPIO_CS_TXD_MASK));

    // 32bit連続1書き込み
    *SPIO_FIFO = 0xFF;
    *SPIO_FIFO = 0xFF;
    *SPIO_FIFO = 0xFF;
    *SPIO_FIFO = 0xFF;
    // 送信完了待ち
    while(!(*SPIO_CS & SPIO_CS_DONE_MASK));
    // 通信完了
    *SPIO_CS &= ~SPIO_CS_TA_ACTIVE;

    delay(50);

    // configuretion レジスタ設定
    // 通信開始(TA = 1)
    *SPIO_CS |= SPIO_CS_TA_ACTIVE;
    // 送信可能待ち(wait TXD)
    while(!(*SPIO_CS & SPIO_CS_TXD_MASK));
    *SPIO_FIFO = ADT7310_CMD_ADDR_CONFIG | ADT7310_CMD_WRITE;
    // resolution = 16bit
    *SPIO_FIFO = 0x80;
    // 送信完了待ち
    while(!(*SPIO_CS & SPIO_CS_DONE_MASK));
    // 通信完了
    *SPIO_CS &= ~SPIO_CS_TA_ACTIVE;

    delay(500);

    // 通信開始(TA = 1)
    *SPIO_CS |= SPIO_CS_TA_ACTIVE;
    // 送信可能待ち(wait TXD)
    while(!(*SPIO_CS & SPIO_CS_TXD_MASK));
    // 気温データをコンティニュアスモードで読み込み
    *SPIO_FIFO = ADT7310_CMD_ADDR_TEMP | ADT7310_CMD_READ | ADT7310_CMD_CONTINUOUS;
    // 送信完了待ち
    while(!(*SPIO_CS & SPIO_CS_DONE_MASK));
    // 通信完了
    *SPIO_CS &= ~SPIO_CS_TA_ACTIVE;

    delay(500);

    while(1)
    {
        rawtemp = 0;
        temp = 0;
        // 送信バッファ初期化
        *SPIO_CS |= SPIO_CS_CLEAR_ALL;
        
        // 通信開始(TA = 1)
        *SPIO_CS |= SPIO_CS_TA_ACTIVE;
        // 温度の読み込み(Continuousモード)
        *SPIO_FIFO = 0x0;
        *SPIO_FIFO = 0x0;
        // 受信待ち
        while(!(*SPIO_CS & SPIO_CS_RXD_MASK));
        // 上位8bit受信
        rawtemp = *SPIO_FIFO;
        rawtemp = rawtemp << 8;
        // 受信待ち
        while(!(*SPIO_CS & SPIO_CS_RXD_MASK));
        // 下位8bit受信
        rawtemp |= *SPIO_FIFO;
        // 転送完了待ち
        while(!(*SPIO_CS & SPIO_CS_DONE_MASK));
        // 通信完了
        *SPIO_CS &= ~SPIO_CS_TA_ACTIVE;

        // 温度計算
        if(rawtemp & 0x8000)
        {
            temp = (rawtemp - 32768) / 128.0;
        }
        else
        {
            temp = rawtemp / 128.0;
        }

        // temperatureレジスタ値表示
        printf("temperature register :  0x%04x\n",rawtemp);
        // 温度表示
        printf("temperature : %f(dec)\n",temp);

        delay(3000);
    }
    



    return 0;
}