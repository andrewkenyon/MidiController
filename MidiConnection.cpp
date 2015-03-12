/*!
 *  @file       MidiConnection.cpp
 *  @brief      MIDI connection for Arduino MIDI controller.
 *              Handles all Midi I/O and all but SysEx parsing.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */
 
#include "MidiConnection.h"

namespace midi
{
  //Default constructor
  MidiConnection::MidiConnection()
  {

	  Serial.begin(31250);
	  this->myChannel = 0;
  }
  
  MidiConnection::MidiConnection(uint8_t channel)
  {
    Serial.begin(31250);
    this->myChannel = channel;
  }
		
  MidiConnection::~MidiConnection()
  {
  }
			
  void MidiConnection::setChannel(uint8_t channel)
  {
    this->myChannel = channel;
  }
		
  uint8_t MidiConnection::getChannel()
  {
    return this->myChannel;
  }
  
  MidiMessage* MidiConnection::getMsg()
  {
    return this->myMsg;
  }
  
  /* Send raw command byte (0x80-0xFF), first bit is always set to 1 */
  void MidiConnection::sendCommand(uint8_t command)
  {
    Serial.write(0x80 | (command << 4) | this->myChannel);
  }
	
  /* Send raw data byte (0x00-0x7F), first bit should always be 0 */	
  void MidiConnection::sendData(uint8_t data)
  {
    Serial.write(data);
  }

  /* Send program change without changing bank. */		
  void MidiConnection::sendProgramChange(uint8_t program)
  {
    this->sendCommand(PROGRAM_CHANGE);
    this->sendData(program);
  }
  
  void MidiConnection::sendControlChange(uint8_t control, uint8_t value)
  {
    this->sendCommand(CONTROL_CHANGE);
    this->sendData(control);
    this->sendData(value);
  }
  
  /* Send System Exclusive Message.
  Should pass in with checksum if required, but not start or terminating byte. */
  void MidiConnection::sendSysEx(std::vector<uint8_t> data)
  {
    Serial.write(0xF0);
    for(uint8_t i=0; i < data.size(); i++)
    {
      this->sendData(data.at(i));
    }
    Serial.write(0xF7);
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
        delete this->myMsg;

        uint8_t type = (msg >> 4) & 0x7;
        uint8_t secondHalf = msg & 0x0F;
      
        /*SysEx is actually a subtype of system messages. 
        The type (bits 2-4) is really for system msg (7), requiring the usual "channel" bits to select 0 for SysEx. 
        We do not support the other types, but must still check and disregard if not 0xF0.*/
        if(type == SYSTEM_MESSAGE)
        {
          this->myMsg = new SystemMessage(secondHalf);
        }
        else if((type == PROGRAM_CHANGE) && (this->myChannel == secondHalf))
        {
            this->myMsg = new ProgramChangeMessage(secondHalf);
        }
		else if ((type == CONTROL_CHANGE) && (this->myChannel == secondHalf))
		{
			this->myMsg = new ControlChangeMessage(secondHalf);
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
