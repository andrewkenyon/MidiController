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
  MidiMessage::MidiMessage()
  {
    this->myStatus = INVALID; //At initiallisation no command has been recieved, so there is no message yet!
  }
  
  MidiMessage::~MidiMessage()
  {
    delete &myData; //Do I really want to do this?
  }
  
  /* Process command byte. Only PC, CC and SysEx are supported */
  boolean MidiMessage::processCommand(byte command)
  {
    this->myType = (command >> 4) & 0x7;
    this->myChannel = command & 0x0F;
    
    /*SysEx is actually a subtype of system messages. 
    The type (bits 2-4) is really for system msg (7), requiring the usual "channel" bits to select 0 for SysEx. 
    We do not support the other types, but must still check and disregard if not 0xF0.*/
    if((this->myType == SYSTEM_EXCLUSIVE)&&(this->myChannel != 0))
    {
      this->myStatus = INVALID;
      return false;
    }
    else
    {
      if(this->myType == SYSTEM_EXCLUSIVE)
      {
        this->myData.reserve(20); //Target hardware largest message was < 20, so shouldn't need to reallocate. But...
      }
      else
      {
        this->myData.reserve(2);
      }
      this->myStatus = INCOMPLETE;
      return true;
    }
  }
      
  /* If it is appropriate add data byte to myData as appropriate for the type.
  Update the status of the message accordingly */
  boolean MidiMessage::addData(byte newData)
  {
    if(this->myStatus == INCOMPLETE)
    {
      if(this->myType == SYSTEM_EXCLUSIVE)
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
      else if( ((this->myType == PROGRAM_CHANGE) || (this->myType == CHANNEL_PRESSURE)) && (this->myData.size() < 1) )
      {          
        this->myData.push_back(newData);
        this->myStatus = COMPLETE;
        return true; 
      }
      else if((this->myType != SYSTEM_EXCLUSIVE) && (this->myData.size() < 2))
      {
        this->myData.push_back(newData);
        if(this->myData.size() == 2)
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
      
  byte MidiMessage::getType()
  {
    return this->myType;
  }
  
  byte MidiMessage::getChannel()
  {
    return this->getChannel();
  }
  
  vector<byte> MidiMessage::getData()
  {
    return this->myData;  
  }
    
  byte MidiMessage::getStatus()
  {
    return this->myStatus;
  }
  
}
