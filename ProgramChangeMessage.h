// ProgramChangeMessage.h

#ifndef _PROGRAMCHANGEMESSAGE_h
#define _PROGRAMCHANGEMESSAGE_h

#include "MidiMessage.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	class ProgramChangeMessage : public MidiMessage
	{
	private:
		uint8_t myChannel; //Channel message was recieved on. For System messages this is actually the sub-type (e.g. SysEx).
		uint8_t myProgram;

	public:
		ProgramChangeMessage(uint8_t channel);

		uint8_t getChannel() const;
		bool addData(uint8_t newData);

		uint8_t getProgram() const;
	};
}

#endif

