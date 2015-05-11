// BankSwitch.h

#ifndef _BANKSWITCH_h
#define _BANKSWITCH_h

#include "Footswitch.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class BankSwitch : public Footswitch
	{
	public:
		BankSwitch(FootController* ctrl);
		~BankSwitch();

	private:
		bool handlePress(uint16_t duration);
	};
}


#endif

