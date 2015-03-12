// SelectSwitch.h

#ifndef _SELECTSWITCH_h
#define _SELECTSWITCH_h

#include "Footswitch.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class SelectSwitch : public Footswitch
	{
	public:
		SelectSwitch();
		~SelectSwitch();

	private:
		bool handlePress(uint16_t duration);
	};
}


#endif

