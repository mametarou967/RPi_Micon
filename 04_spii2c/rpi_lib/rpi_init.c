#include "rpi.h"
#include "uart/rpi_uart.h"

void rpi_init(void)
{
    clearBss();
    Serial_begin(115200);
}
