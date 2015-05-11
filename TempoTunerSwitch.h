// SelectSwitch.h

#ifndef _TEMPOTUNERSWITCH_h
#define _TEMPOTUNERSWITCH_h

#include "Footswitch.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class TempoTunerSwitch : public Footswitch
	{
	public:
		TempoTunerSwitch(FootController* ctrl);
		~TempoTunerSwitch();

	private:
		bool handlePress(uint16_t duration);
	};
}


#endif

