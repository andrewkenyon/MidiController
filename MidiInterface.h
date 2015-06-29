/*!
 *  @file       MidiInterface.h
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       29/06/2015
 */
 
#pragma once

#include "SysExDefs.h"
#include "AxeController.h"
#include "MidiConnection.h"

#include <Arduino.h>

namespace midi
{	
	class AxeController;

	class MidiInterface
	{
		private:
			MidiConnection* myConnection;
			AxeController* myController;

			int8_t myChannel = -1;
      
			uint8_t myProgram; //Program 0-127, in combination with bank, preset up to 14-bit number (~16000).
			uint8_t myBank; //The midi bank (CC #0) currently used.
      
			int myTempo; //Tempo in bpm
			unsigned long myPreviousTempoPulse;
      
		public:
			MidiInterface(const int8_t channel);
			MidiInterface();
			~MidiInterface();
		  
			void test(const uint16_t testInt);
			void update();

			uint16_t getPreset();
			int8_t getChannel();
			
			bool setProgram(uint8_t programChange);
			bool setBank(uint8_t bankChange);
				
			void sendExtendedProgramChange(const uint16_t extProgram);
      
	  
		public:
			void updateTempo();			
		private:       
			void pulseTempo();
		  
		  /****************************************/
		  
		  /* SysEx get and set message generation */
		  
		private:
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
		  
			void generateChecksum(SysExMessage* msg);
		  
		  /****************************************/
		  
			std::vector<uint8_t> handleFootswitches(const std::vector<int16_t>& presses);
			uint8_t handlePress(int16_t press);
	};
}
    


