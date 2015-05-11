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

#include <vector>
#include <stdint.h>

#include "MidiInterface.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

using namespace midi;

MidiInterface* gMidiInterface;

uint16_t test;

/********************************************************************************/

void setup()
{
  gMidiInterface = new MidiInterface((int8_t) 0); //Start MidiInterface
  test = 10;
  delay(5000);
}

void loop()
{
  //gMidiInterface->update();
  gMidiInterface->test(test++);
}
