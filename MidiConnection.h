/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI connection for Arduino MIDI controller.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */
 
#pragma once

#include "ProgramChangeMessage.h"
#include "ControlChangeMessage.h"
#include "SysExMessage.h"

#include <Arduino.h>

namespace midi
{  
  class MidiConnection
  {
    private: 
      uint8_t myChannel; //1-16. The channel to listen to. Messages on others are still recieved but ignored.
      
      MidiMessage* myMsg;
		
    public: 
      MidiConnection();
      MidiConnection(uint8_t channel);
      ~MidiConnection();
		
      void setChannel(uint8_t channel);
		
      uint8_t getChannel();
      MidiMessage* getMsg();
			
    private: 
      void sendCommand(uint8_t command);
      void sendData(uint8_t data);
		
    public: 
      void sendProgramChange(uint8_t program);
      void sendControlChange(uint8_t control, uint8_t value);
	  void sendSysEx(std::vector<uint8_t> data);
			
      bool checkBuffer();		
  };
}
