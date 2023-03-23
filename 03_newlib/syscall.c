#include "rpi_lib/uart/rpi_uart.h"
#include "sys/types.h"
#include <sys/stat.h>

extern void *__bss_end;
unsigned int heap_end = (unsigned int)&__bss_end;
unsigned int prev_heap_end;

register char * stack_ptr asm ("sp");

caddr_t _sbrk(int incr)
{
    prev_heap_end = heap_end;
    if ((char *)heap_end + incr > stack_ptr)
    {
        return (caddr_t)-1;
    }
    heap_end = heap_end + incr;
    return (caddr_t)prev_heap_end;
}

int _write(int file, char *ptr, int len) {
    int r;
    for(r=0;r<len;r++) uart0_putc(ptr[r]);
    return len;
}

int _read(int file, char *ptr, int len) {
    int r;
    for(r=0;r<len;r++) ptr[r] = uart0_getc();
    return len;
}

int _close(int file) {
  return -1;
}

int _fstat(int file, struct stat *st) {
  return 0;
}

int _isatty(int file) {
  return 1;
}

int _lseek(int file, int ptr, int dir) {
  return 0;
}

void _exit(int code)
{
    return;
}

int _kill(int pid, int sig) {
  return -1;
}

int _getpid(void) {
  return 1;
}