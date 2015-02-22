/*!
 *  @file       Footswitch.h
 *  @brief      Handles footswitch I/0. Currently also includes associated LED
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       18/02/2015
 */
 
#pragma once

#include <vector>

#include <Arduino.h>

namespace midi
{
  class Footswitch
  {
    private:
      int16_t myCurrentPress; //Negative value means not pressed
      uint8_t myLedState; //Currently just on and off.
      
    public:
      Footswitch();
      ~Footswitch();

      int16_t updateFootswitch(bool pressed); //Updates current press.
      void updateLed(uint8_t state);
  };
}
