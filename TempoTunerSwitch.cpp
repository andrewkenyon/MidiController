/*!
 *  @file       TempoTunerSwitch.cpp
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       18/02/2015
 */

#include "TempoTunerSwitch.h"

namespace midi
{    
  TempoTunerSwitch::TempoTunerSwitch(FootController* ctrl) : Footswitch(ctrl)
  {
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
