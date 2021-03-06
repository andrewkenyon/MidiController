/*!
 *  @file       MidiConnection.cpp
 *  @brief      MIDI connection for Arduino MIDI controller.
 *              Handles all Midi I/O and all but SysEx parsing.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */
 
#include "MidiConnection.h"

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
		delete this->myMsg;
	}
  
    
	/* Check the serial buffer and assemble a message if possible. getMsg() must be called in order to get message. Will overwrite current message when called again */
	bool MidiConnection::checkBuffer()
	{
		while(Serial.available())
		{
			uint8_t msg = Serial.read();
		  
			if((msg >= 0x80)) //msg is a command
			{				
				uint8_t type = (msg >> 4) & 0x7;
				uint8_t secondHalf = msg & 0x0F;
			  
				/*SysEx is actually a subtype of system messages. 
				The type (bits 2-4) is really for system msg (7), requiring the usual "channel" bits to select 0 for SysEx. 
				We do not support the other types, but must still check and disregard if not 0xF0 */

				switch(type)
				{
					case CONTROL_CHANGE:	
					{
						delete this->myMsg;//new message, so clear old message
						this->myMsg = new ControlChangeMessage(secondHalf);
						break;
					}
					case PROGRAM_CHANGE:
					{
						delete this->myMsg;//new message, so clear old message
						this->myMsg = new ProgramChangeMessage(secondHalf);
						break;
					}
					case SYSTEM_MESSAGE:
					{
						if (secondHalf == 0x0) //SysEx
						{							
							delete this->myMsg;//new message, so clear old message
							this->myMsg = new SysExMessage();
						}
						break;
					}
					default:
					{
						delete this->myMsg;
						this->myMsg = NULL; //Invalid message, but store anyway.
						break;
					}
				}
			}
			else if (this->myMsg != NULL)
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
  
	const MidiMessage& MidiConnection::getMsg() const
	{
	  return *this->myMsg;
	}
  
	bool MidiConnection::sendMessage(const MidiMessage& msg) const
	{
		switch (msg.getType())
		{
			case PROGRAM_CHANGE:
			{
				this->sendProgramChange((ProgramChangeMessage&)msg);
				return true;
			}
			case CONTROL_CHANGE:
			{
				this->sendControlChange((ControlChangeMessage&)msg);
				return true;
			}
			case SYSTEM_MESSAGE:
			{
				this->sendSysEx((SysExMessage&)msg);
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	/* Send program change without changing bank. */		
	void MidiConnection::sendProgramChange(const ProgramChangeMessage& pc) const
	{
		this->sendCommand(PROGRAM_CHANGE, pc.getChannel());
		this->sendData(pc.getProgram());
	}
  
	void MidiConnection::sendControlChange(const ControlChangeMessage& cc) const
	{
		this->sendCommand(CONTROL_CHANGE, cc.getChannel());
		this->sendData(cc.getControllerNumber());
		this->sendData(cc.getControllerValue());
	}
  
	/* Send System Exclusive Message.
	Should pass in with checksum if required, but not start or terminating byte. */
	void MidiConnection::sendSysEx(const SysExMessage& sysEx) const
	{
		Serial.write(0xF0);
		for(uint8_t i=0; i < sysEx.payloadSize(); i++)
		{
		  this->sendData(sysEx.payloadAt(i));
		}
		Serial.write(0xF7);
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
}		
