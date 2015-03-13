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
  
  /************************************************************************/

  uint8_t MidiMessage::getType()
  {
    return this->myType;
  }
  
  vector<uint8_t> MidiMessage::getData()
  {
    return this->myData;  
  }
    
  uint8_t MidiMessage::getStatus()
  {
    return this->myStatus;
  }

}
