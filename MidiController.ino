#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>

/*!
 *  @file       MidiController.ino
 *  @brief      Main file for Arduino MIDI controller.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       15/01/2015
 */

#include "MidiInterface.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

using namespace midi;

MidiInterface* gMidiInterface;

/********************************************************************************/

void setup()
{  
  gMidiInterface = new MidiInterface(); //Start MidiInterface
}

void loop()
{
  delay(1000);
  gMidiInterface->test();
}