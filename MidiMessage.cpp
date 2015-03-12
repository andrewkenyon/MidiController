/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI command/data message class.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */

#include "MidiMessage.h"

using namespace std;

namespace midi
{      
  MidiMessage::MidiMessage(uint8_t type)
  {
    this->myStatus = INVALID; //At initiallisation no command has been recieved, so there is no message yet!
    this->myType = type;
  }
  
  MidiMessage::~MidiMessage()
  {
  }

  ControlChangeMessage::ControlChangeMessage(uint8_t channel) : MidiMessage::MidiMessage(PROGRAM_CHANGE)
  {
	  this->myChannel = channel;
  }

  ControlChangeMessage::~ControlChangeMessage()
  {
  }
  
  SystemMessage::SystemMessage(uint8_t subType) : MidiMessage::MidiMessage(SYSTEM_MESSAGE)
  {
    this->mySubType = subType;
  }
  
  SystemMessage::~SystemMessage()
  {
  }
  
  /************************************************************************/
      

  /* If it is appropriate add data byte to myData as appropriate for the type.
  Update the status of the message accordingly */
  bool ControlChangeMessage::addData(uint8_t newData)
  {
	  if (this->myStatus == INCOMPLETE)
	  {
		  if (this->myData.size() < 2)
		  {
			  this->myData.push_back(newData);
			  if (this->myData.size() == 2)
			  {
				  this->myStatus = COMPLETE;
			  }
			  return true;
		  }
		  else
		  {
			  this->myStatus = INVALID;
			  return false;
		  }
	  }
	  else
	  {
		  return false;
	  }
  }

  /* If it is appropriate add data byte to myData as appropriate for the type.
  Update the status of the message accordingly */
  bool SystemMessage::addData(byte newData)
  {
    if(this->myStatus == INCOMPLETE)
    {
        if(newData == 0x7F)
        {
          this->myStatus = COMPLETE;
        }
        else
        {
          this->myData.push_back(newData);
        }
        return true; 
    }
    else
    {
      return false;
    }
  }
      
  byte MidiMessage::getType()
  {
    return this->myType;
  }
  
  vector<byte> MidiMessage::getData()
  {
    return this->myData;  
  }
    
  uint8_t MidiMessage::getStatus()
  {
    return this->myStatus;
  }
  
  uint8_t ControlChangeMessage::getChannel()
  {
	  return this->getChannel();
  }

}
