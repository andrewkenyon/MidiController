/*!
 *  @file       FootController.cpp
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       17/01/2015
 */


#include "FootController.h"

namespace midi
{
  FootController::FootController()
  {
    this->myLcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
    this->myLcd->begin(LCD_COLUMNS,LCD_ROWS);
  }
  
  FootController::~FootController()
  {
  }

  /* Displays preset number in top left of LCD.
 Accepts int, so can accept larger values than the actual MIDI program 
 limit of 128 (MidiInterface uses bank changes to get around this). */
  void FootController::displayProgramNo(int ProgramNo)
  {
    this->myLcd->setCursor(0, 0);
    int position = this->myLcd->print("Pre ");
    position += this->myLcd->print(ProgramNo);
    for(int i=0; i < LCD_COLUMNS-7-position; i++)
    {
      this->myLcd->print(" ");
    }
  }

  /*  Display preset name on bottom left */
  void FootController::displayProgramName(String ProgramName)
  {
    this->myLcd->setCursor(0, 1);
    int position = this->myLcd->print(ProgramName);
    for(int i=0; i < LCD_COLUMNS-position; i++)
    {
      this->myLcd->print(" ");
    }
  }  
  
  /*  Display beats per minute on top left */
  int FootController::displayBpm(int tempo)
  {
    int bpm = 60000/tempo;
  
    this->myLcd->setCursor(9, 0);
    int position=0;
  
    //Pad depending on number of digits necessary
    if(bpm < 100)
    {
      position += this->myLcd->print(" ");
      if(bpm < 10)
      {
        position += this->myLcd->print(" ");
      }
    }
  
    position += this->myLcd->print(bpm);
    position += this->myLcd->print("bpm");
    return position;
  }

  /* Clears tempo "!" mark if half way through pulses, 
  in order to show as 50/50 on/off pulse */
  void FootController::displayTempoPulse(boolean display)
  {
    this->myLcd->setCursor(15, 0);
    if (true)
    {
      this->myLcd->print("!");
    }
    else
    {
      this->myLcd->print(" ");
    }
  }

}
