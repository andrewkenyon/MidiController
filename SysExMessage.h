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

		// Function IDs for Axe-FX SysEx
	#define MIDI_PARAMETER 0x02 //MIDI_GET_PARAMETER, MIDI_SET_PARAMETER
	#define MIDI_MODIFIER 0x07 //MIDI_GET_MODIFIER, MIDI_SET_MODIFIER
	#define MIDI_GET_FIRMWARE_VERSION 0x08
	#define MIDI_TUNER_INFO 0x0D
	#define MIDI_GET_PRESET_EFFECT_BLOCKS_AND_CC_AND_BYPASS_STATE 0x0E
	#define MIDI_GET_PRESET_NAME 0x0F
	#define MIDI_TEMPO_BEAT 0x10
	#define MIDI_PRESET_NUMBER 0x14
	#define MIDI_GET_ROUTING_GRID_LAYOUT 0x20
	#define MIDI_LOOPER_STATUS 0x23  //MIDI_LOOPER_STATUS_ENABLE, MIDI_LOOPER_STATUS
	#define MIDI_SET_SCENE_NUMBER 0x29 

		// Modifier Parameter Selector IDs:
	#define MOD_SOURCE 0x0 //modifier source
	#define MOD_MIN 0x1 //min
	#define MOD_MAX 0x2 //max
	#define MOD_START 0x3 //start
	#define MOD_MID 0x4 //mid
	#define MOD_END 0x5 //end
	#define MOD_SLOPE 0x6 //slope
	#define MOD_DAMPING 0x7 //damping
		//#define 0x8 //?
		//#define 0x9 //?
	#define MOD_AUTO_ENGAGE 0xA //auto engage
	#define MOD_PC_RESET 0xB //pc reset
	#define MOD_OFF_VAL 0xC //off val
	#define MOD_OFFSET 0xE //offset

	static const String NOTE[] =
	{
		"A",
		"Bb",
		"B",
		"C",
		"Db",
		"D",
		"Eb",
		"E",
		"F",
		"Gb",
		"G",
		"Ab"
	};

	class SystemMessage : public MidiMessage
	{
	private:
		uint8_t mySubType; //For System messages second half of byte is the sub-type (e.g. SysEx).

	public:
		SystemMessage(uint8_t subType);
		~SystemMessage();

		bool addData(uint8_t newData);
	};
}

#endif

