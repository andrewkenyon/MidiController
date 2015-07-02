/*!
 *  @file       MessageHandler.h
 *  @brief      Handles MidiMessages using the MidiInterface
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       29/06/2015
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
			uint8_t* myBank;
			uint8_t* myProgram;
			
			AxeController* myController;
      
		private:
			MessageHandler();
		public:
			MessageHandler(MidiInterface* interface, uint8_t* bank, uint8_t* program, AxeController* controller);
			~MessageHandler();
			
		/************* Handle Messages ***************/
		
		public:
			bool handleMsg(const MidiMessage& msg);
		private:
			bool handleProgramChange(const ProgramChangeMessage& msg);
			bool handleControlChange(const ControlChangeMessage& msg);
			bool handleSysEx(const SysExMessage& msg);
			/* SysEx Messages */
			bool handleParameter(const std::vector<uint8_t>& data);
			bool handleModifier(const std::vector<uint8_t>& data);
			bool handleFirmwareVersion(const std::vector<uint8_t>& data);
			bool handleTunerInfo(const std::vector<uint8_t>& data);
			bool handlePresetState(const std::vector<uint8_t>& data);
			bool handlePresetName(const std::vector<uint8_t>& data);
			bool handleTempoBeat();
			bool handlePresetNumber(const std::vector<uint8_t>& data);
			bool handleGridRouting(const std::vector<uint8_t>& data);
			bool handleLooperStatus(const std::vector<uint8_t>& data);
			bool handleSceneNumber(const std::vector<uint8_t>& data);
		
		/****************************************/
		  
		public:
			static uint16_t processSeptets(const uint8_t sept0, const uint8_t sept1, const uint8_t sept2);
			static uint16_t processSeptets(const uint8_t sept0, const uint8_t sept1);	
							
			static String extractString(const std::vector<uint8_t>& data, const uint8_t start);
			
			static bool validateChecksum(const std::vector<uint8_t>& data);
	};
}
    


