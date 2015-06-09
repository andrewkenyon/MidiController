/*!
 *  @file       BankSwitch.h
 *  @brief      Handles bank up and down I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       09/06/2015
 */
 
#ifndef _BANKSWITCH_h
#define _BANKSWITCH_h

#include "AxeController.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class BankSwitch : public FootSwitch
	{
	private:
		AxeController* myFootController;
			
	public:
		BankSwitch(AxeController* ctrl);
		~BankSwitch();

	private:
		bool handlePress(uint16_t duration);
	};
}


#endif

