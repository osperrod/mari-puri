/**
   @file radio.cpp
   @brief Abstracting the radio modem
   Very basic. Some extra work needd for a real implementation
*/

#include <Arduino.h>
#include <MKRWAN.h>
#include "radio.hpp"
#include "credentials.h"  // LoRaWAN credentials

static LoRaModem modem;
static bool connected = false;

bool radio_Init(void)
{
  // modem initialization, Europe region
  if (!modem.begin(EU868))
  {
    // LoraWAN modem initization fails
    return false;
  }

  // join
  if (!modem.joinABP(DEVADDR, NWKSKEY, APPSKEY))
  {
    // joinABP() method failed connecting
    return false;
  }

  modem.minPollInterval(60);

  connected = true;

  return true;
}

void radio_startPayload(uint8_t port)
{
  modem.setPort(port);     // A field of the package, some are reserved
  modem.beginPacket();  // Start encoding a packet    
}

bool radio_sendPayload(void)
{
  uint8_t err;
  
  err = modem.endPacket(false);
  if (err > 0) {
    return true;
  } else {
    return false;
  } 
}

void radio_addUint16_t(uint16_t value)
{
  modem.write((uint8_t)(value >> 8));  // Encode MSB of counter
  modem.write((uint8_t)(value & 0xFF));// and the LSB, you decide
}

void radio_addTemperature(float value)
{
  int16_t temperature_encoded;

  // the temperature adequately scaled
  // It is up to you to decide the format of data
  temperature_encoded = value * 10.0f; // two's complement
  modem.write((uint8_t)(temperature_encoded >> 8));  // Encode MSB
  modem.write((uint8_t)(temperature_encoded & 0xFF));// and the LSB, you decide 
}

// Cayenne LPP
void radio_addCLPPTemperature(uint8_t channel, float value)
{
  int16_t temperature_encoded;

  modem.write((uint8_t)channel);  // Channel 1
  modem.write((uint8_t)0x67);     // It is a Cayenne LPP temperature
  
  // the temperature adequately scaled
  // It is up to you to decide the format of data
  temperature_encoded = value * 10.0f; // two's complement
  modem.write((uint8_t)(temperature_encoded >> 8));  // Encode MSB
  modem.write((uint8_t)(temperature_encoded & 0xFF));// and the LSB, you decide 
}


// returns -1 if no downlink data available or the first byte available in the data buffer 
int16_t radio_downlinkRead(void)
{
  if (modem.available())
  {
    return modem.read();
  } else {
    return -1;
  }
}

void radio_Run(void)
{
  modem.poll(); // not clear in the manual the pourpose of that
}

/*** End of file *********************************************************************************/
