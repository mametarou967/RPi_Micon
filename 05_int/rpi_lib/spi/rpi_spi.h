#ifndef RPILIB_SPI_H
#define RPILIB_SPI_H

void SPI0_begin(int ss_port);
void SPI0_end(void);
void SPI0_setClockDivider(unsigned int divider);
void SPI0_setDataMode(unsigned int mode);
unsigned int SPI0_transfer(unsigned int value);
void SPI0_setBitOrder(int order);

#endif