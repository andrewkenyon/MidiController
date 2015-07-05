/*!
 *  @file       TempoTunerSwitch.cpp
 *  @brief      Handles tempo and tuner switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       03/07/2015
 */

#include "TempoTunerSwitch.h"

namespace midi
{    
  TempoTunerSwitch::TempoTunerSwitch(MidiInterface* interface) : FootSwitch()
  {
	  this->myInterface = interface;
  }
  
  TempoTunerSwitch::~TempoTunerSwitch()
  {
  }
  
  bool TempoTunerSwitch::handlePress(uint16_t duration)
  {
	if(duration < LONG_PRESS)
	{
		this->myInterface->tapTempo(millis());
	}
	else
	{
		this->myLedState = this->myInterface->toggleTunerMode();
	}
				
    return true;
  }
  
}
