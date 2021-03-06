/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI command/data message class.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       11/05/2015
 */
 
#pragma once

#include <vector>

#include <Arduino.h>

namespace midi
{
	#define NOTE_ON 0
	#define NOTE_OFF 1
	#define KEY_PRESSURE 2
	#define CONTROL_CHANGE 3
	#define PROGRAM_CHANGE 4
	#define CHANNEL_PRESSURE 5
	#define PITCH_BEND 6
	#define SYSTEM_MESSAGE 7 //Currently only support SysEx, so must also check second half of byte is 0

	#define CC_BANKMSB 0 // Bank change. Only most significant bit, but most potential paired hardware does not even support this.
	#define CC_BANKLSB 32 //Probably not going to use this...

	#define INVALID 0 // Something is wrong with the message, or no command has yet been recieved.
	#define INCOMPLETE 1 // Message is potentially valid, but needs (more) data.
	#define COMPLETE 2 // Message is ready to pass to MidiInterface

	class MidiMessage
	{
		protected:
			const int8_t myType; // currently only PC, CC System message (only SysEx) support intended. -1 represents not set (can never be a valid message)
			uint8_t myStatus; //INVALID, INCOMPLETE or COMPLETE
		  
		private:
			MidiMessage();
		protected: 
			MidiMessage(uint8_t type);
		public:
			virtual ~MidiMessage();

		public:
			virtual bool addData(const uint8_t& newData) = 0;

			virtual int8_t getType() const;
			uint8_t getStatus() const;
	};

}
