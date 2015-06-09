/*!
 *  @file       TempoTunerSwitch.cpp
 *  @brief      Handles tempo and tuner switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       09/06/2015
 */

#include "TempoTunerSwitch.h"

namespace midi
{    
  TempoTunerSwitch::TempoTunerSwitch(AxeController* ctrl) : FootSwitch()
  {
	  this->myFootController = ctrl;
  }
  
  TempoTunerSwitch::~TempoTunerSwitch()
  {
  }
  
  bool TempoTunerSwitch::handlePress(uint16_t duration)
  {
    // TODO
    return true;
  }
  
}
