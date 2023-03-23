#ifndef RPILIB_GPIO_H
#define RPILIB_GPIO_H

#define PIN_INDEX_MAX 53

typedef enum _PIN_MODE
{
    PIN_MODE_INPUT,
    PIN_MODE_OUTPUT,
    PIN_MODE_INPUT_PULLUP,
    PIN_MODE_INPUT_PULLDOWN,
    PIN_MODE_ALT0,
    PIN_MODE_ALT1,
    PIN_MODE_ALT2,
    PIN_MODE_ALT3,
    PIN_MODE_ALT4,
    PIN_MODE_ALT5
} PIN_MODE;

typedef enum _PIN_VALUE
{
    PIN_VALUE_HIGH,
    PIN_VALUE_LOW
} PIN_VALUE;

void pinMode(int pin, PIN_MODE pinMode);
void digitalWrite(int pin,PIN_VALUE pinValule);
PIN_VALUE digitalRead(int pin);

#endif