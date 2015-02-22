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
  class MidiConnection
  {
    private: 
      byte myChannel; //1-16. The channel to listen to. Messages on others are still recieved but ignored.
      
      MidiMessage* myMsg;
		
    public: 
      MidiConnection();
      MidiConnection(byte channel);
      ~MidiConnection();
		
      void setChannel(byte channel);
		
      byte getChannel();
      MidiMessage* getMsg();
			
    private: 
      void sendCommand(byte command);
      void sendData(byte data);
		
    public: 
      void sendProgramChange(byte program);
      void sendControlChange(byte control, byte value);
      void sendSysEx(vector<uint8_t> data);
			
      bool checkBuffer();		
  };
}
