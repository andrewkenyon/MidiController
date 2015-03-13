/*!
 *  @file       Footswitch.cpp
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       18/02/2015
 */

#include "SelectSwitch.h"

namespace midi
{    
  SelectSwitch::SelectSwitch()
  {
    Footswitch::init();
  }
  
  SelectSwitch::~SelectSwitch()
  {
  }
  
  bool SelectSwitch::handlePress(uint16_t duration)
  {
    // TODO
    return true;
  }
  
}
