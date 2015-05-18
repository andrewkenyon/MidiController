/*!
 *  @file       AxeMessage.h
 *  @brief      Axe Fx specific SysEx data handler.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       12/05/2015
 */

#pragma once

#include "MidiInterface.h"
#include "SysExMessage.h"

#include <Arduino.h>

namespace midi
{
  class AxeMessage
  {

    public: 
      AxeMessage(const SysExMessage& msg);
      
      bool handleMessage(const MidiInterface& interface);
      
  };

}
