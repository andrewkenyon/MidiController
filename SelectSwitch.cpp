/*!
 *  @file       SelectSwitch.cpp
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       03/07/2015
 */

#include "SelectSwitch.h"

namespace midi
{    

	SelectSwitch::SelectSwitch(AxeController* ctrl, uint8_t switchNumber, int8_t secondaryNumber) : FootSwitch()
	{
		this->myPrimaryNumber = switchNumber;
		this->mySecondaryNumber = secondaryNumber;
		this->myController = ctrl;
	}
	
	SelectSwitch::SelectSwitch(AxeController* ctrl, uint8_t switchNumber) : FootSwitch()
	{
		this->myPrimaryNumber = switchNumber;
		this->mySecondaryNumber = -1;
		this->myController = ctrl;
	}
  
	SelectSwitch::~SelectSwitch()
	  {
	}
  
	bool SelectSwitch::handlePress(uint16_t duration)
	{
		if( (duration < LONG_PRESS) || (this->mySecondaryNumber < 0) )
		{
			this->myController->changeProgramWithinBank(this->myPrimaryNumber);
			this->myLedState = 1;
		}
		else
		{
			this->myController->changeProgramWithinBank(this->mySecondaryNumber);
		}
	
		return true;
	}
  
}
