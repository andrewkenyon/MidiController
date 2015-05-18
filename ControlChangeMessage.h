// ControlChangeMessage.h

#ifndef _CONTROLCHANGEMESSAGE_h
#define _CONTROLCHANGEMESSAGE_h

#include "MidiMessage.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	class ControlChangeMessage : public MidiMessage
	{
	private:
		uint8_t myChannel; //Channel message was recieved on. For System messages this is actually the sub-type (e.g. SysEx).
		uint8_t myControllerNumber;
		uint8_t myControllerValue;
		uint8_t myDataCounter;

	public:
		ControlChangeMessage(uint8_t channel);
		~ControlChangeMessage();

		bool addData(uint8_t newData);

		uint8_t getChannel() const;
		uint8_t getControllerNumber() const;
		uint8_t getControllerValue() const;
		
		void sendMessage(MidiConnection* conn);
	};
}

#endif

