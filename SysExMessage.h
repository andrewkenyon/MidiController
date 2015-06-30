// SysExMessage.h

#ifndef _SYSEXMESSAGE_h
#define _SYSEXMESSAGE_h

#include "MidiMessage.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

namespace midi
{
	// SysEx header
	#define MAN_ID1 0x00
	#define MAN_ID2 0x01
	#define MAN_ID3 0x74
	#define MODEL_ID 0x06 // # (Axe-Fx II XL = 0x06)

	class SysExMessage : public MidiMessage
	{

	private:
		std::vector<uint8_t> myData;
		std::vector<uint8_t>::iterator myNextByte;

	public:
		SysExMessage();
		~SysExMessage();

		bool addData(uint8_t newData);
		const std::vector<uint8_t>& getData() const;
	};
}

#endif

