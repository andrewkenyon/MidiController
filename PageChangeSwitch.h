// PageChangeSwitch.h

#ifndef _PAGECHANGESWITCH_h
#define _PAGECHANGESWITCH_h

#include "Footswitch.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class PageChangeSwitch : public Footswitch
	{
	public:
		PageChangeSwitch(FootController* ctrl);
		~PageChangeSwitch();

	private:
		bool handlePress(uint16_t duration);
	};
}


#endif

