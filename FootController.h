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

namespace midi
{
	#define LCD_COLUMNS 16
	#define LCD_ROWS 2

	#define SWITCHES 8

	#define PAGE_PRESETS 0
	#define PAGE_IA 1
	#define PAGE_LOOPER 2

	class FootController
	{
	private:
		LiquidCrystal* myLcd;
		std::vector<Footswitch*> mySwitches;
		uint8_t myPage;
      
	public:
		FootController();
		~FootController();

		void displayProgramNumber(uint16_t programNumber);
		void displayProgramName(String programName);

		int displayBpm(int tempo);
		void displayTempoPulse(bool pulse);
      
		void displayTuner(String note, uint8_t string, uint8_t cents);
      
		std::vector<int16_t> updateFootswitches();
		void updateLeds(std::vector<uint8_t> states);
	};
}
