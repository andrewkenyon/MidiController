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
    this->myBank = 0;
  }
  
  MidiConnection::MidiConnection(byte channel)
  {
    Serial.begin(31250);
    this->myChannel = channel;
    this->myBank = 0;
  }
		
  MidiConnection::~MidiConnection()
  {
  }
			
  void MidiConnection::setChannel(byte channel)
  {
    this->myChannel = channel;
  }
		
  byte MidiConnection::getChannel()
  {
    return this->myChannel;
  }
  
  MidiMessage* MidiConnection::getMsg()
  {
    return this->myMsg;
  }
  
  /* Send raw command byte (0x80-0xFF), first bit is always set to 1 */
  void MidiConnection::sendCommand(byte command)
  {
    Serial.write(0x80 | (command << 4) | this->myChannel);
  }
	
  /* Send raw data byte (0x00-0x7F), first bit should always be 0 */	
  void MidiConnection::sendData(byte data)
  {
    Serial.write(data);
  }

  /* Send program change without changing bank. */		
  void MidiConnection::sendProgramChange(byte program)
  {
    this->sendCommand(PROGRAM_CHANGE);
    this->sendData(program);
  }
  
  /* Work out the bank and PC commands necessary for a Program larger than 127.
  Try to always use this for user I/O, so as to shield user from bank number complications. */
  void MidiConnection::sendExtendedProgramChange(int extProgram)
  {
    if(this->myBank != extProgram/128)
    {
      this->myBank = extProgram/128;
      this->sendControlChange(CC_BANKMSB, this->myBank);
    }
    this->sendProgramChange(extProgram%128);
  }
  
  void MidiConnection::sendControlChange(byte control, byte value)
  {
    this->sendCommand(CONTROL_CHANGE);
    this->sendData(control);
    this->sendData(value);
  }
  
  /* Check the serial buffer and assemble a message if possible.
  getMsg() must be called in order to get message.
  Will overwrite current message when called again */
  boolean MidiConnection::checkBuffer()
  {
    while(Serial.available())
    {
      byte msg = Serial.read();
      
      if((msg >> 7)) //msg is a command
      {
        this->myMsg = new MidiMessage();
        if((msg & 0x0F) == this->myChannel)
        {
          this->myMsg->processCommand(msg); // TODO Need to deal with invalid commands
        }
      }
      else
      {
        this->myMsg->addData(msg);
      }
      
      if(this->myMsg->getStatus() == COMPLETE)
        return true;
    }
    return false;
  }
}		
