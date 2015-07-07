/*!
 *  @file       MidiInterface.h
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
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
			std::vector<long> myTappedTempo;
			
			bool myTunerMode;
      
		public:
			MidiInterface(const int8_t channel);
			MidiInterface();
			~MidiInterface();
		  
			void test(const uint16_t testInt);
			void update();

			uint16_t getPreset() const;
			int8_t getChannel() const;

			void changePreset(const uint16_t preset, const bool transmit);
			
			void updateTempo();			
			void tapTempo(long currentTap);
		private:       
			void pulseTempo();
			
		public:
			bool toggleTunerMode();
		 
	};
}