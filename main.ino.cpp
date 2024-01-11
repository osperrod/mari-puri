#include "led.hpp"
#define PAUSE 100


void setup()
{
    LED_Init();
}

void loop()
{
    LED_On();
    delay(PAUSE);
    LED_Off();
    delay(PAUSE);
}