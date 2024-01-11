/* temperature_fake.cpp 
 * A pseudo temperature sensor for the COVID era.
 * If your have a real sensor, then think on how to update this
 */

#include <stdint.h>
#include "temperature.hpp"

#define TEMPERATURE_MIN 12.7
#define TEMPERATURE_MAX 37.2
#define TEMPERATURE_STEP 0.06

#ifndef TEMPERATURE_REAL

void temperature_Init(void) 
{
}

float temperature_Read(void) 
{
    static float temp = TEMPERATURE_MIN;
    static uint8_t direction = 1; // 1 = up, 0 = down

    if (direction == 1)
    {
      temp += TEMPERATURE_STEP;
      if (temp >TEMPERATURE_MAX)
      {
        temp = TEMPERATURE_MAX;
        direction = 0;
      }
    } else {
      temp -= TEMPERATURE_STEP;
      if (temp < TEMPERATURE_MIN)
      {
        temp = TEMPERATURE_MIN;
        direction = 1;
      }
    }

    return temp;
}

#endif
/*** End of file *********************************************************************************/
