/*!
 *  @file       Footswitch.cpp
 *  @brief      Handles footswitch I/0. Currently also includes associated LED
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       18/02/2015
 */
 
#include "FootSwitch.h"

namespace midi
{  
	Footswitch::Footswitch(FootController* ctrl)
	{
		this->myFootController = ctrl;
		this->myCurrentPress = -1;
		this->myLedState = 0;
	}
	
	Footswitch::~Footswitch()
	{
		
	}
  
  // Updates current press. When switch is released sends length of button press
  int16_t Footswitch::updateFootswitch(bool pressed)
  {
    if(pressed && (this->myCurrentPress < 0))
    {
      this->myCurrentPress = millis();
      return -1;
    }
    else if(!pressed && (this->myCurrentPress >= 0))
    {
      uint16_t press = millis() - this->myCurrentPress;
      this->handlePress(press);
      this->myCurrentPress = -1;
      return press;
    }
  }
  
  /* LED state. Only in sofware */
  void Footswitch::updateLed(uint8_t state)
  {
    this->myLedState = state;
  }
  
  
  
}
