/*
  IoT seminars. Angel Perles
  Final temperature demo app
*/

#include "led.hpp"
#include "temperature.hpp"
#include "radio.hpp"

// Time between transmissions
#define SEND_TIME 120000      // Transmit every 2 minutes, maximun allowed by the modem
//#define SEND_TIME (5*60*1000) // Every 5 minutes

// Uncomment to allow standalone operation (no terminal)
// #define STANDALONE

static uint32_t time_stamp;
static bool first_time = true;

void setup() 
{
  
  LED_Init();
  temperature_Init();
  
  //console initialization
  Serial.begin(9600);
#ifndef STANDALONE   
   while(!Serial); // this is safer for these SAMD21 devices 
#endif  
  
  if (!radio_Init())
  {
#ifndef STANDALONE
    Serial.println("Radio initialization failed");  
#endif
    for(;;){};
  }
  
  // Trick to send every SEND_TIME
  time_stamp = millis();
}

void loop() 
{
  uint32_t time_now;
  static uint16_t counter = 0;
  float temperature;
  
  // read temperature sensor
  temperature = temperature_Read(); 
  
  // temperature check, e.g "siren alarm" using the LED
  if (temperature > 34.0) // caloret faller
  {  
    LED_On();
  }
  
  // Transmitt data every SEND_TIME miliseconds
  time_now = millis();
  if ((first_time) || (time_now >= (time_stamp + SEND_TIME)))
  {
    first_time = false;
#ifndef STANDALONE
    Serial.println(time_stamp);
#endif
    time_stamp = time_now;
    
    // uplink
    radio_startPayload(2);
    
    // Custom payload
    // radio_addUint16_t(counter);
    // radio_addTemperature(temperature);
  
    // Cayenne payload
    radio_addCLPPTemperature(1, temperature);
    
    radio_sendPayload();
    
    counter++;
  }
  
  // downlink process
  // e.g stop alarm
  int16_t code;
  code = radio_downlinkRead();
  switch (code)
  {
    case -1:  // no data in the downlink
      break;
    case 0x00:
      LED_On();
      break;
    case 0x01:
      LED_Off();
      break;
    default:
      // Not handled code
      break;
  }

  // ideally, sleep for a while
  delay(50); // busy wait, no saving power
    
} /* loop() */

/*** end of file ***/
