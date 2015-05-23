/*!
 *  @file       MidiInterface.h
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       17/01/2015
 */
 
#pragma once

#include "FootController.h"
#include "MidiConnection.h"

#include <Arduino.h>

namespace midi
{
	#define GRID_COLUMNS 12
	#define GRID_ROWS 4
	
	class FootController;

	class MidiInterface
	{
		private:
			MidiConnection* myConnection;
			FootController* myController;

			int8_t myChannel = -1;
      
			uint16_t myProgram;
			uint8_t myBank; //The midi bank (CC #0) currently used. //Allow negative values to represent unknown starting state
      
			int myTempo; //Tempo in bpm
			unsigned long myPreviousTempoPulse;
      
		public:
			MidiInterface(const int8_t channel);
			MidiInterface();
			~MidiInterface();
		  
			void test(const uint16_t testInt);
			void update();

			int getProgram();
			int getBank();
				
			void sendExtendedProgramChange(const uint16_t extProgram);
				
			static uint16_t processSeptets(const uint8_t sept0, const uint8_t sept1, const uint8_t sept2);
			static uint16_t processSeptets(const uint8_t sept0, const uint8_t sept1);	  
			
			static String extractString(const std::vector<uint8_t>& data, const uint8_t start);
      
		private:         
			void updateTempo();
			void pulseTempo();
		  
		  /****************************************/
		  
		  /* SysEx get and set message generation */
		  
		  	static void encodeForSysEx(const uint16_t num, const uint8_t bytes, SysExMessage* msg);
					
			static void generateHeader(SysExMessage* msg);			

			
			void getFunction(const uint8_t function);
	  
			void getParameter(const uint16_t& effect, const uint16_t&_tparameter);
			void setParameter(const uint16_t& effect, const uint16_t& parameter, const uint16_t& value);
		  
			void getModifier(const uint16_t effect, const uint16_t parameter, const uint8_t selector);
			void setModifier(const uint16_t effect, const uint16_t parameter, const uint8_t selector, const uint16_t value);

			void getFirmwareVersion();

			void getPresetState();

			void getPresetName();
			void getPresetNumber();
			void setPresetNumber();

			void getGridRouting();

			void enableLoopStatus(const bool enable);

			void setSceneNumber(const uint8_t scene);

		  /****************************************/

			/* Handle Messages */
			
			bool handleMsg(const MidiMessage& msg);
			bool handleProgramChange(const ProgramChangeMessage& msg);
			bool handleControlChange(const ControlChangeMessage& msg);
			bool handleSysEx(const SysExMessage& msg);

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
		  
			bool validateChecksum(const std::vector<uint8_t>& data);
			void generateChecksum(SysExMessage* msg);
		  
		  /****************************************/
		  
			std::vector<uint8_t> handleFootswitches(const std::vector<int16_t>& presses);
			uint8_t handlePress(int16_t press);
	};
}
    


