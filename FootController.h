/*!
 *  @file       FootController.h
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       18/01/2015
 */
 
#pragma once

#include "Footswitch.h"

#include <vector>

#include <LiquidCrystal.h>
#include <Arduino.h>

#define LCD_COLUMNS 16
#define LCD_ROWS 2

#define SWITCHES 8

using namespace std;

namespace midi
{
  class FootController
  {
    private:
      LiquidCrystal* myLcd;
      vector<Footswitch> mySwitches;
      
    public:
      FootController();
      ~FootController();

      void displayProgramNumber(int programNumber);
      void displayProgramName(String programName);

      int displayBpm(int tempo);
      void displayTempoPulse(bool pulse);
      
      void displayTuner(String note, byte string, byte cents);
      
      vector<int16_t> updateFootswitches();
      void updateLeds(vector<uint8_t> states);
  };
}
