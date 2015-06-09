/*!
 *  @file       SelectSwitch.cpp
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       09/06/2015
 */

#include "SelectSwitch.h"

namespace midi
{    
  SelectSwitch::SelectSwitch(AxeController* ctrl, uint8_t switchNumber) : FootSwitch()
  {
	  this->myNumber = switchNumber;
	  this->myFootController = ctrl;
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
