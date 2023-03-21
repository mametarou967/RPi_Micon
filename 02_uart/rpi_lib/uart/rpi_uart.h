#ifndef RPILIB_UART_H
#define RPILIB_UART_H

int Serial_begin(int baudrate);
void Serial_end();
int Serial_available(void);
int Serial_write(char *buf,int len);
int Serial_read(void);
void setSerialTimeout(int read_ms,int write_ms);
int uart0_putc(int c);

#endif
