/*!
 *  @file       AxeController.h
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */
 
#pragma once

#include "FootController.h"
#include "MidiInterface.h"

#include <LiquidCrystal.h>
#include <Arduino.h>

namespace midi
{
	#define LCD_COLUMNS 16
	#define LCD_ROWS 2

	#define SWITCHES 8
	#define BANK_SIZE 5

	#define PAGE_PRESETS 0
	#define PAGE_IA 1
	#define PAGE_LOOPER 2	
	
	class MidiInterface;

	class AxeController : public FootController
	{
	private:
		MidiInterface* myInterface;
		
	private:
		AxeController();
	public:
		AxeController(MidiInterface* interface);
		~AxeController();
		
	public:
		void displayPresetNumber(uint16_t programNumber);
		void displayProgramName(String programName);

		int displayBpm(int tempo);
		void displayTempoPulse(bool pulse);
      
		void displayTuner(String note, uint8_t string, uint8_t cents);
      
		void updateFootswitches();
		
	private:
		void refreshLeds();
		
		/*********************************************/
	
	public:
		void changeProgramWithinBank(uint8_t preset);
		void bankUp();
		void bankDown();
	};
}
