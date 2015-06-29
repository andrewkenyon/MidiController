/*!
 *  @file       FootController.cpp
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       10/05/2015
 */

#include "AxeController.h"
#include "BankSwitch.h"
#include "PageChangeSwitch.h"
#include "SelectSwitch.h"
#include "TempoTunerSwitch.h"

using namespace std;

namespace midi
{
	AxeController::AxeController(MidiInterface* interface) : FootController()
	{
		this->myInterface = interface;
		  
		this->myLcd = new LiquidCrystal(7, 6, 5, 4, 3, 2);
		this->myLcd->begin(LCD_COLUMNS,LCD_ROWS);

		this->mySwitches = vector<FootSwitch*>(SWITCHES);
		this->mySwitches.push_back(new SelectSwitch(this, 0));
		this->mySwitches.push_back(new SelectSwitch(this, 1));
		this->mySwitches.push_back(new SelectSwitch(this, 2));
		this->mySwitches.push_back(new SelectSwitch(this, 3));
		this->mySwitches.push_back(new SelectSwitch(this, 4));
		this->mySwitches.push_back(new PageChangeSwitch(this));
		this->mySwitches.push_back(new TempoTunerSwitch(this));
		this->mySwitches.push_back(new BankSwitch(this));

		this->myPage = PAGE_PRESETS;
	}
		
	
  /* Displays preset number in top left of LCD.
 Accepts int, so can accept larger values than the actual MIDI program 
 limit of 128 (MidiInterface uses bank changes to get around this). */
  void AxeController::displayPresetNumber(uint16_t programNumber)
  {
    this->myLcd->setCursor(0, 0);
    int position = this->myLcd->print("Pre ");
    position += this->myLcd->print(programNumber);
    for(int i=0; i < LCD_COLUMNS-7-position; i++)
    {
      this->myLcd->print(" ");
    }
  }

  /*  Display preset name on bottom left */
  void AxeController::displayProgramName(String programName)
  {
    this->myLcd->setCursor(0, 1);
    int position = this->myLcd->print(programName);
    for(int i=0; i < LCD_COLUMNS-position; i++)
    {
      this->myLcd->print(" ");
    }
  }  
  
  /*  Display beats per minute on top left */
  int AxeController::displayBpm(int tempo)
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
  void AxeController::displayTempoPulse(bool display)
  {
    this->myLcd->setCursor(LCD_COLUMNS-1, 0);
    if (display)
    {
      this->myLcd->print("!");
    }
    else
    {
      this->myLcd->print(" ");
    }
  }
  
  void AxeController::displayTuner(String note, uint8_t string, uint8_t cents)
  {
    this->myLcd->setCursor(0,0);
    int position = this->myLcd->print(note);
    position += this->myLcd->print(" on ");
    position += this->myLcd->print(string);
    for(int i=0; i < LCD_COLUMNS-position; i++)
    {
      this->myLcd->print(" ");
    }
    
    int currentNote = LCD_COLUMNS*((int)cents-63)/128;
    
    this->myLcd->setCursor(0,1);
    position = 0;
    
    while(position < (LCD_COLUMNS/2-1))
    {
      if(position < currentNote)
        position += this->myLcd->print(" ");
      else
        position += this->myLcd->print("-");
    }
    position += this->myLcd->print("==");
    while(position < (LCD_COLUMNS))
    {
      if(position > currentNote)
        position += this->myLcd->print(" ");
      else
        position += this->myLcd->print("+");
    }
    
  }
  
  vector<int16_t> AxeController::updateFootswitches()
  {
    vector<bool> status = vector<bool>(SWITCHES);
    // TODO: Check shift register for status of buttons
    
    vector<int16_t> presses = vector<int16_t>(SWITCHES);
    for(uint8_t i=0; i < this->mySwitches.size(); i++)
    {
      presses.at(i) = this->mySwitches.at(i)->updateFootswitch(status.at(i));
    }
    return presses;
  }
  
  void AxeController::updateLeds(vector<uint8_t> states)
  {
    //Update state in software
    for(uint8_t i=0; this->mySwitches.size(); i++)
    {
      this->mySwitches.at(i)->updateLed(states.at(i));
    }
    
    //TODO: Send state info to hardware through shift register
  }
  
  
	void AxeController::changeProgramWithinBank(uint8_t preset)
	{
		this->myInterface->sendExtendedProgramChange(this->myBank*BANK_SIZE + preset);
	}	
		
}
