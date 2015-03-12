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

	#define LONG_PRESS 1000

  /* General abstract class to represent footswitch common functionality */
  class Footswitch
  {
    private:
      int16_t myCurrentPress; //Negative value means not pressed
      uint8_t myLedState; //Currently just on and off.
      
    public:
      int16_t updateFootswitch(bool pressed); //Updates current press.
      void updateLed(uint8_t state);
    
    protected:
      void init();
      
    private:
      virtual bool handlePress(uint16_t duration);
  };
  
  /*
  class BankSwitch : public Footswitch
  {
    public:
      BankSwitch();
      ~BankSwitch();
    
    private:
      bool handlePress(uint16_t duration);
  };
  
  class TempoTunerSwitch : public Footswitch
  {
    private:
      bool handlePress(uint16_t duration);
  };
  
  class PageChangeSwitch : public Footswitch
  {
    private:
      bool handlePress(uint16_t duration);
  };
  */
}
