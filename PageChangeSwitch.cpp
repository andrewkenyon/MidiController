/*!
 *  @file       PageChangeSwitch.cpp
 *  @brief      Handles changing to IA or looper pages.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       09/06/2015
 */

#include "PageChangeSwitch.h"

namespace midi
{    
  PageChangeSwitch::PageChangeSwitch(AxeController* ctrl) : FootSwitch()
  {
	  this->myFootController = ctrl;
  }
  
  PageChangeSwitch::~PageChangeSwitch()
  {
  }
  
  bool PageChangeSwitch::handlePress(uint16_t duration)
  {
    // TODO
    return true;
  }
  
}
