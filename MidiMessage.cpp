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
	MidiMessage::MidiMessage()
	{
		this->myStatus = INVALID; //At initiallisation no command has been recieved, so there is no message yet!
	}
	
	MidiMessage::MidiMessage(uint8_t type)
	{
		this->myStatus = INVALID; //At initiallisation no command has been recieved, so there is no message yet!
		this->myType = type;
	}
    
	bool MidiMessage::addData(uint8_t newData)
	{
		return false;
	}
  
  /************************************************************************/

	uint8_t MidiMessage::getType() const
	{
		return this->myType;
	}
  
	uint8_t MidiMessage::getStatus() const
	{
		return this->myStatus;
	}

}
