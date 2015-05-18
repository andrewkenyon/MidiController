/*!
 *  @file       MidiConnection.cpp
 *  @brief      MIDI connection for Arduino MIDI controller.
 *              Handles all Midi I/O and all but SysEx parsing.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */
 
#include "MidiConnection.h"

#include "ProgramChangeMessage.h"
#include "ControlChangeMessage.h"
#include "SysExMessage.h"

using namespace std;

namespace midi
{
  //Default constructor
	MidiConnection::MidiConnection()
  {
	  this->myMsg = NULL;
	  Serial.begin(31250);
  }
		
  MidiConnection::~MidiConnection()
  {
	  if (this->myMsg != NULL)
	  {
		  delete this->myMsg;
	  }
  }

  MidiMessage* MidiConnection::getMsg()
  {
	  MidiMessage* returnPtr = this->myMsg;
	  this->myMsg = NULL;
	  return returnPtr;
  }
  
  /* Send raw command byte (0x80-0xFF), first bit is always set to 1 */
  void MidiConnection::sendCommand(const uint8_t command, const uint8_t channelOrSubtype) const
  {
	  Serial.write(0x80 | (command << 4) | channelOrSubtype);
  }
	
  /* Send raw data byte (0x00-0x7F), first bit should always be 0 */	
  void MidiConnection::sendData(const uint8_t data) const
  {
    Serial.write(data);
  }
  
  /* Check the serial buffer and assemble a message if possible.
  getMsg() must be called in order to get message.
  Will overwrite current message when called again */
  bool MidiConnection::checkBuffer()
  {
    while(Serial.available())
    {
      byte msg = Serial.read();
      
      if((msg >> 7)) //msg is a command
      {

        uint8_t type = (msg >> 4) & 0x7;
        uint8_t secondHalf = msg & 0x0F;
      
        /*SysEx is actually a subtype of system messages. 
        The type (bits 2-4) is really for system msg (7), requiring the usual "channel" bits to select 0 for SysEx. 
        We do not support the other types, but must still check and disregard if not 0xF0 
		(This is now done in MidiInterface, to keep MidiConnection as generic as possible.*/

		switch (this->myMsg->getType())
		{
			case CONTROL_CHANGE:
				this->myMsg = new ControlChangeMessage(secondHalf);
				break;
			case PROGRAM_CHANGE:
				this->myMsg = new ProgramChangeMessage(secondHalf);
				break;
			case SYSTEM_MESSAGE:
				if (secondHalf == 0x0) //SysEx
					this->myMsg = new SysExMessage();
				break;
			default:
				break;
		}
      }
      else
      {
        this->myMsg->addData(msg);
      }
        
      if(this->myMsg->getStatus() == COMPLETE)
      {
        return true;
      }
    }
    return false;
  }
}		
