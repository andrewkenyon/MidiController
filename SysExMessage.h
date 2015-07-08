/*!
 *  @file       SysExMessage.h
 *  @brief      System Exclusive Message
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       08/07/2015
 */


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

		public:
			SysExMessage();
			virtual ~SysExMessage();

			virtual bool addData(const uint8_t& newData);
			const int8_t& payloadAt(const uint16_t& index) const;
			const uint16_t& payloadSize() const;
	};
}

#endif

