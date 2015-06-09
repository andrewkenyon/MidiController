/*!
 *  @file       SelectSwitch.h
 *  @brief      Handles main bottom row "select" switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       09/06/2015
 */

#ifndef _SELECTSWITCH_h
#define _SELECTSWITCH_h

#include "AxeController.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class SelectSwitch : public FootSwitch
	{		
	private:
		AxeController* myFootController;
		uint8_t myNumber;
		
	public:
		SelectSwitch(AxeController* ctrl, uint8_t switchNumber);
		~SelectSwitch();

	private:
		bool handlePress(uint16_t duration);
	};
}


#endif

