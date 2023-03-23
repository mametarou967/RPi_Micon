#ifndef RPILIB_PERIPHERALS_H
#define RPILIB_PERIPHERALS_H

#include "../rpi_type.h"

#define PHY_PERI_ADDR(x)        (0x20000000 + (x))

// timer
#define SYST_BASE               (0x00003000)
#define SYST_CS                 ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x00))
#define SYST_CLO                ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x04))
#define SYST_CHI                ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x08))
#define SYST_C0                 ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x0C))
#define SYST_C1                 ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x10))
#define SYST_C2                 ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x14))
#define SYST_C3                 ((vu32_t *)PHY_PERI_ADDR(SYST_BASE + 0x18))

// gpio
#define GPIO_BASE               (0x00200000)
#define GPIO_GPFSEL0            ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x00))
#define GPIO_GPFSEL1            ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x04))
#define GPIO_GPFSEL2            ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x08))
#define GPIO_GPFSEL3            ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x0C))
#define GPIO_GPFSEL4            ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x10))
#define GPIO_GPFSEL5            ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x14))
// reserved
#define GPIO_GPSET0             ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x1C))
#define GPIO_GPSET1             ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x20))
//reserved
#define GPIO_GPCLR0             ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x28))
#define GPIO_GPCLR1             ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x2C))
//...
#define GPIO_GPLEV0             ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x34))
#define GPIO_GPLEV1             ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x38))

#define GPIO_GPPUD              ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x94))
#define GPIO_GPPUDCLK0          ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x98))
#define GPIO_GPPUDCLK1          ((vu32_t *)PHY_PERI_ADDR(GPIO_BASE + 0x9C))

// UART peripheral
#define UART0_BASE              (0x00201000)
#define UART0_DR                ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x00))
#define UART0_FR                ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x18))
#define UART0_IBRD              ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x24))
#define UART0_FBRD              ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x28))
#define UART0_LCRH              ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x2C))
#define UART0_CR                ((vu32_t *)PHY_PERI_ADDR(UART0_BASE + 0x30))


#endif