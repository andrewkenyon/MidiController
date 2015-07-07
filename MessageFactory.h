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
			uint8_t myBank;
			uint8_t myProgram;
			
		private:
			MessageFactory(); //disallow default contructor
		public:
			MessageFactory(MidiInterface* interface, uint8_t bank, uint8_t program);
			~MessageFactory();
			
			/***********************************************************/
			
		public:
			const ProgramChangeMessage programChange(uint8_t program) const;
			const ControlChangeMessage bankChange(uint8_t bank) const;
		  
			/************ SysEx get and set message generation *********/
		  
		private:
		  	static void encodeForSysEx(const uint16_t num, const uint8_t bytes, SysExMessage* msg);
			static void generateHeader(SysExMessage* msg);				  
			void generateChecksum(SysExMessage* msg);	
			
		/****************************************/
		
		public:		
			SysExMessage getFunction(const uint8_t function);
	  
			SysExMessage getParameter(const uint16_t& effect, const uint16_t&_tparameter);
			SysExMessage setParameter(const uint16_t& effect, const uint16_t& parameter, const uint16_t& value);
		  
			SysExMessage getModifier(const uint16_t effect, const uint16_t parameter, const uint8_t selector);
			SysExMessage setModifier(const uint16_t effect, const uint16_t parameter, const uint8_t selector, const uint16_t value);

			SysExMessage getFirmwareVersion();

			SysExMessage getPresetState();

			SysExMessage getPresetName();
			SysExMessage getPresetNumber();
			SysExMessage changePresetNumber();

			SysExMessage getGridRouting();

			SysExMessage enableLoopStatus(const bool enable);

			SysExMessage setSceneNumber(const uint8_t scene);


	};
}
    


