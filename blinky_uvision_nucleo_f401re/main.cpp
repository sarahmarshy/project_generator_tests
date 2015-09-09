#include "mbed.h"

DigitalOut led(LED1);
RawSerial pc(USBTX, USBRX);

int main()
{
    while (true) {
        led = !led; // toggle led
        wait(0.2f);
        pc.puts("Blink!");
    }
}