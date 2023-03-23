#include "rpi_lib/rpi_type.h"
#include "rpi_lib/rpi.h"
#include "stdio.h"

int main(void){
    rpi_init();

    printf("HelloWorld\n");

    return 0;
}