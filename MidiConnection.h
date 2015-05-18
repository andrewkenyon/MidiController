/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI connection for Arduino MIDI controller.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */
 
#pragma once

#include "MidiMessage.h"

#include <Arduino.h>

namespace midi
{  
	class MidiMessage;

  class MidiConnection
  {
    private: 
      MidiMessage* myMsg;
		
    public: 
      MidiConnection();
	  ~MidiConnection();
	  
	  bool checkBuffer();
      MidiMessage* getMsg();
	  
      void sendCommand(const uint8_t command, const uint8_t channelOrSubtype) const;
      void sendData(const uint8_t data) const;	
  };
}
