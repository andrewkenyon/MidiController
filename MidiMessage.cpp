/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI command/data message class.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       10/05/2015
 */

#include "MidiMessage.h"

using namespace std;

namespace midi
{   
	MidiMessage::MidiMessage() : myType(-1)
	{
		this->myStatus = INVALID; //At initiallisation no command has been recieved, so there is no message yet!
	}
	
	MidiMessage::MidiMessage(uint8_t type) : myType(type)
	{
		this->myStatus = INVALID; //At initiallisation no command has been recieved, so there is no message yet!
	}
	
	MidiMessage::~MidiMessage()
	{
	}

	int8_t MidiMessage::getType() const
	{
		return this->myType;
	}
  
	uint8_t MidiMessage::getStatus() const
	{
		return this->myStatus;
	}

}
