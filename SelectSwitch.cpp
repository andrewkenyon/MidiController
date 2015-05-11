/*!
 *  @file       Footswitch.cpp
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       10/05/2015
 */

#include "SelectSwitch.h"

namespace midi
{    
  SelectSwitch::SelectSwitch(FootController* ctrl, uint8_t switchNumber) : Footswitch(ctrl)
  {
	  this->myNumber = switchNumber;
  }
  
  SelectSwitch::~SelectSwitch()
  {
  }
  
  bool SelectSwitch::handlePress(uint16_t duration)
  {
    // Ignore short/long presses for now
	
	this->myFootController->changeProgramWithinBank(this->myNumber);
	
    return true;
  }
  
}
