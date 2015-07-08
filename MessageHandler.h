/*!
 *  @file       MessageHandler.h
 *  @brief      Handles MidiMessages using the MidiInterface
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       08/07/2015
 */
 
#pragma once

#include "MidiInterface.h"

#include <Arduino.h>

namespace midi
{	
	class MessageHandler
	{
		private:
			MidiInterface* myInterface;
			uint8_t myBank;
			
			AxeController* myController;
      
		private:
			MessageHandler();
		public:
			MessageHandler(MidiInterface* interface, const uint8_t bank, AxeController* controller);
			~MessageHandler();
			
		/************* Handle Messages ***************/
		
		public:
			bool handleMsg(const MidiMessage& msg);
		private:
			bool handleProgramChange(const ProgramChangeMessage& msg);
			bool handleControlChange(const ControlChangeMessage& msg);
			bool handleSysEx(const SysExMessage& msg);
			/* SysEx Messages */
			bool handleParameter(const SysExMessage& msg);
			bool handleModifier(const SysExMessage& msg);
			bool handleFirmwareVersion(const SysExMessage& msg);
			bool handleTunerInfo(const SysExMessage& msg);
			bool handlePresetState(const SysExMessage& msg);
			bool handlePresetName(const SysExMessage& msg);
			bool handleTempoBeat();
			bool handlePresetNumber(const SysExMessage& msg);
			bool handleGridRouting(const SysExMessage& msg);
			bool handleLooperStatus(const SysExMessage& msg);
			bool handleSceneNumber(const SysExMessage& msg);
		
		/****************************************/
		  
		public:
			static uint16_t processSeptets(const uint8_t sept0, const uint8_t sept1, const uint8_t sept2);
			static uint16_t processSeptets(const uint8_t sept0, const uint8_t sept1);	
							
			static String extractString(const SysExMessage& msg, const uint8_t start);
			
			static bool validateChecksum(const SysExMessage& msg);
	};
}
    


