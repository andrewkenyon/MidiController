/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI command/data message class.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */
 
#pragma once


#include <vector>

#include "SysExDefs.h"

#include <Arduino.h>

#define NOTE_ON 0
#define NOTE_OFF 1
#define KEY_PRESSURE 2
#define CONTROL_CHANGE 3
#define PROGRAM_CHANGE 4
#define CHANNEL_PRESSURE 5
#define PITCH_BEND 6
#define SYSTEM_EXCLUSIVE 7 //Currently only support SysEx, so must also check second byte is 0

#define CC_BANKMSB 0 // Bank change. Only most significant bit, but most potential paired hardware does not even support this.
#define CC_BANKLSB 32 //Probably not going to use this...

#define INVALID 0 // Something is wrong with the message, or no command has yet been recieved.
#define INCOMPLETE 1 // Message is potentially valid, but needs (more) data.
#define COMPLETE 2 // Message is ready to pass to MidiInterface

using namespace std;

namespace midi
{
  class MidiMessage
  {
    private:
      uint8_t myType; // currently only PC, CC System message (only SysEx) support intended.
      uint8_t myChannel; //Channel message was recieved on. For System messages this is actually the sub-type (e.g. SysEx).
      vector<uint8_t> myData; //PC 1 byte, CC 2 bytes, SysEx no limit.
      uint8_t myStatus; //INVALID, INCOMPLETE or COMPLETE
      
    public: 
      MidiMessage();
      ~MidiMessage();
      
      bool processCommand(uint8_t command);
      bool addData(uint8_t newData);
      
      uint8_t getType();
      uint8_t getChannel();
      vector<uint8_t> getData();    
      uint8_t getStatus();
  };
}
