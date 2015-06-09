/*!
 *  @file       FootController.h
 *  @brief      Handles user I/O (LCD, switches and LEDs)
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       10/05/2015
 */
 
#pragma once

#include "FootSwitch.h"

#include <vector>

#include <LiquidCrystal.h>
#include <Arduino.h>

namespace midi
{
	class FootSwitch;

	class FootController
	{
	protected:
		LiquidCrystal* myLcd;
		std::vector<FootSwitch*> mySwitches;
		uint8_t myPage;
		uint8_t myBank; //Not the same as the MIDI bank!
      
	public:
		FootController();
		virtual ~FootController();
	};
}
