/*!
 *  @file       TempoTunerSwitch.h
 *  @brief      Handles tempo and tuner switch I/0.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       03/07/2015
 */


#ifndef _TEMPOTUNERSWITCH_h
#define _TEMPOTUNERSWITCH_h

#include "AxeController.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	//Specific implementations for each type of switch
	class TempoTunerSwitch : public FootSwitch
	{
		private:
			AxeController* myController;
			MidiInterface* myInterface;
			
		public:
			TempoTunerSwitch(MidiInterface* interface);
			~TempoTunerSwitch();

		private:
			bool handlePress(uint16_t duration);
	};
}


#endif

