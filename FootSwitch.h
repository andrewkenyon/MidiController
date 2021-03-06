/*!
 *  @file       FootSwitch.h
 *  @brief      Handles footswitch I/0. Currently also includes associated LED
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       03/07/2015
 */
 
#pragma once

#include <vector>

#include <Arduino.h>

#include "FootController.h"

namespace midi
{

	#define LONG_PRESS 1000
	
	class FootController;

	/* General abstract class to represent footswitch common functionality */
	class FootSwitch
	{
		public:
			FootSwitch();
			virtual ~FootSwitch();
	  
		protected:
			int16_t myCurrentPress; //Negative value means not pressed
			uint8_t myLedState; //Currently just on and off.
          
		private:
			virtual bool handlePress(uint16_t duration) = 0;
	  
		public:
			int16_t updateFootswitch(bool pressed); //Updates current press.
			void setLedState(uint8_t state);     
			uint8_t getLedState();

	};
 
}
