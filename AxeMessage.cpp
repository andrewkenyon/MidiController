/*!
 *  @file       AxeMessage.cpp
 *  @brief      Axe Fx specific SysEx data handler.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       12/05/2015
 */

#include "AxeMessage.h"

using namespace std;

namespace midi
{    
  AxeMessage::AxeMessage(const SysExMessage& msg)
  {
  }
  
  bool AxeMessage::handleMessage(const MidiInterface& interface)
  {
	  //TODO
	  return false;
  }

}
