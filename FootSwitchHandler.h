/*!
 *  @file       FootSwitchHandler.h
 *  @brief      Handles presses of the footswitches
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */
 
#pragma once

#include "MidiInterface.h"

namespace midi
{	
	class FootSwitchHandler
	{
		
		private:
			MidiInterface* myInterface;
			AxeController* myController;
			
		public:
			FootSwitchHandler(MidiInterface* interface, AxeController* controller);
		  
		  /****************************************/
		  
			std::vector<uint8_t> handleFootswitches(const std::vector<int16_t>& presses);
			uint8_t handlePress(int16_t press);
	};
}
    


