/*!
 *  @file       BankSwitch.cpp
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       10/05/2015
 */

#include "BankSwitch.h"

namespace midi
{    
  BankSwitch::BankSwitch(FootController* ctrl) : Footswitch(ctrl)
  {
  }
  
  BankSwitch::~BankSwitch()
  {
  }
  
  bool BankSwitch::handlePress(uint16_t duration)
  {
    // TODO
    return true;
  }
  
}
