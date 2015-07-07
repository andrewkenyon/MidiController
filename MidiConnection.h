/*!
 *  @file       MidiConnection.h
 *  @brief      MIDI connection for Arduino MIDI controller.
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */
 
#pragma once

#include "ProgramChangeMessage.h"
#include "ControlChangeMessage.h"
#include "SysExMessage.h"

#include <Arduino.h>

namespace midi
{  
	class MidiConnection
	{	  
		private: 
			MidiMessage* myMsg;
			
		public: 
			MidiConnection();
			virtual ~MidiConnection();

			bool sendMessage(const MidiMessage& msg) const;

			virtual bool checkBuffer();
			const MidiMessage& getMsg() const;
				
		private:
			void sendProgramChange(const ProgramChangeMessage& pc) const;
			void sendControlChange(const ControlChangeMessage& cc) const;
			void sendSysEx(const SysExMessage& sysEx) const;
			
			void sendCommand(const uint8_t command, const uint8_t channelOrSubtype) const;
			void sendData(const uint8_t data) const;	
	};
}
