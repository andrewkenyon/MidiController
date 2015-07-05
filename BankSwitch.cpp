/*!
 *  @file       BankSwitch.cpp
 *  @brief      Handles bank up and down I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       09/06/2015
 */

#include "BankSwitch.h"

namespace midi
{    
  BankSwitch::BankSwitch(AxeController* ctrl) : FootSwitch()
  {
	myFootController = ctrl;
  }
  
  BankSwitch::~BankSwitch()
  {
  }
  
  bool BankSwitch::handlePress(uint16_t duration)
  {
    if(duration >= LONG_PRESS)
	{
		this->myFootController->bankDown();
	}
	else
	{
		this->myFootController->bankUp();
	}
    return true;
  }
  
}
