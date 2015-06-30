/*!
 *  @file       MessageFactory.h
 *  @brief      Generates MidiMessages
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */
 
#pragma once

#include "MidiInterface.h"
#include "AxeController.h"

#include <Arduino.h>

namespace midi
{	
	class MessageFactory
	{
		private:
			MidiInterface* myInterface;
			
		public:
			MessageFactory(MidiInterface* interface);
			
			/***********************************************************/
			
		public:
			const MidiMessage programChange(uint8_t program) const;
			const MidiMessage bankChange(uint8_t bank) const;
		  
			/************ SysEx get and set message generation *********/
		  
		private:
		  	static void encodeForSysEx(const uint16_t num, const uint8_t bytes, SysExMessage* msg);
			static void generateHeader(SysExMessage* msg);				  
			void generateChecksum(SysExMessage* msg);	
			
		/****************************************/
		
		public:		
			void getFunction(const uint8_t function);
	  
			void getParameter(const uint16_t& effect, const uint16_t&_tparameter);
			void setParameter(const uint16_t& effect, const uint16_t& parameter, const uint16_t& value);
		  
			void getModifier(const uint16_t effect, const uint16_t parameter, const uint8_t selector);
			void setModifier(const uint16_t effect, const uint16_t parameter, const uint8_t selector, const uint16_t value);

			void getFirmwareVersion();

			void getPresetState();

			void getPresetName();
			void getPresetNumber();
			void changePresetNumber();

			void getGridRouting();

			void enableLoopStatus(const bool enable);

			void setSceneNumber(const uint8_t scene);


	};
}
    


