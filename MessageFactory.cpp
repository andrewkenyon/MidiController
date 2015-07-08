/*!
 *  @file       MessageFactory.cpp
 *  @brief      Generates MidiMessasges
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */

#include "MessageFactory.h"

using namespace std;

namespace midi
{    	
	MessageFactory::MessageFactory(MidiInterface* interface, const uint8_t bank, const uint8_t program)
	{
		this->myInterface = interface;
		this->myBank = bank;
		this->myProgram = program;
	}
	
	MessageFactory::~MessageFactory()
	{
	}
	
	/**************************************************************/
	
	const ProgramChangeMessage MessageFactory::programChange(uint8_t program) const
	{
		ProgramChangeMessage pc(this->myInterface->getChannel());
		pc.addData(program);
		return pc;
	}	
	
	const ControlChangeMessage MessageFactory::bankChange(uint8_t bank) const
	{
		ControlChangeMessage cc(this->myInterface->getChannel());
		cc.addData((uint8_t)CC_BANKMSB);
		cc.addData(bank);
		return cc;
	}

	/*********** SysEx get and set message generation *************/
  
	void MessageFactory::encodeForSysEx(uint16_t num, uint8_t bytes, SysExMessage* msg)
	{
		for(int i=0; i < bytes; i++)
		{
			msg->addData((num >> (i*7)) & 0x7F);
		}
	}
	
	void MessageFactory::generateHeader(SysExMessage* msg)
	{  
		msg->addData(MAN_ID1);
		msg->addData(MAN_ID2);
		msg->addData(MAN_ID3);
		msg->addData(MODEL_ID);
	}    
	
	void MessageFactory::generateChecksum(SysExMessage* msg)
	{
		uint8_t checksum = 0xF0;
		for(uint8_t i=0; i < msg->payloadSize(); i++)
		{
			checksum ^= msg->payloadAt(i);
		}
		msg->addData(checksum);    
	}
  
	/************************************************************************************/
    
	SysExMessage MessageFactory::getFunction(uint8_t function)
	{
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(function); // Function ID
		
		generateChecksum(&sysEx);

		return sysEx;
	}	
	
	SysExMessage MessageFactory::getParameter(const uint16_t& effect, const uint16_t& parameter)
	{	
	  
		SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_PARAMETER); // Function ID

		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		encodeForSysEx(0, 3, &sysEx); //Get still requires arbitrary value as far as I can tell

		sysEx.addData(0x00); // 0 = query value
    
		generateChecksum(&sysEx);

		return sysEx;
	}
  
	SysExMessage MessageFactory::setParameter(const uint16_t& effect, const uint16_t& parameter, const uint16_t& value)
	{	
	  	SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_PARAMETER); // Function ID
	
		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		encodeForSysEx(value, 3, &sysEx);				
    
		sysEx.addData(0x01); // 1 = set value
    
		generateChecksum(&sysEx);
		
		return sysEx;
	}
  
	SysExMessage MessageFactory::getModifier(uint16_t effect, uint16_t parameter, uint8_t selector)
	{
	  	SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_MODIFIER); // Function ID
    
		vector<uint8_t> data;
		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		
		sysEx.addData(selector);
		sysEx.addData(0);
		
		encodeForSysEx(0, 3, &sysEx); //Get still requires arbitrary value as far as I can tell
		
		sysEx.addData(0x00); // 0 = query value
		
		generateChecksum(&sysEx);
		
		return sysEx;
	}
  
	SysExMessage MessageFactory::setModifier(uint16_t effect, uint16_t parameter, uint8_t selector, uint16_t value)
	{
	  	SysExMessage sysEx;
		generateHeader(&sysEx);
    
		sysEx.addData(MIDI_MODIFIER); // Function ID
    
		encodeForSysEx(effect, 2, &sysEx);
		encodeForSysEx(parameter, 2, &sysEx);
		sysEx.addData(selector);
		sysEx.addData(value);
		encodeForSysEx(0, 3, &sysEx); //Get still requires arbitrary value as far as I can tell
		
		sysEx.addData(0x00); // 0 = query value
		
		generateChecksum(&sysEx);
		
		return sysEx;
	}
  
	SysExMessage MessageFactory::getFirmwareVersion()
	{
		return getFunction(MIDI_GET_FIRMWARE_VERSION);
	}
  
	SysExMessage MessageFactory::getPresetState()
	{
		return getFunction(MIDI_GET_PRESET_EFFECT_BLOCKS_AND_CC_AND_BYPASS_STATE);
	}
  
	SysExMessage MessageFactory::getPresetName()
	{
		return getFunction(MIDI_GET_PRESET_NAME);
	}
  
	SysExMessage MessageFactory::getPresetNumber()
	{
		return getFunction(MIDI_PRESET_NUMBER);
	}
  
	/* Set preset number using SysEx. Wiki is not very clear on this message, so may be wrong. */
	SysExMessage MessageFactory::changePresetNumber()
	{    
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(MIDI_PRESET_NUMBER);
		
		sysEx.addData(this->myBank);
		sysEx.addData(this->myProgram);
		
		sysEx.addData(0x13); // Seems to be arbritary byte!
		
		generateChecksum(&sysEx);

		return sysEx;
	}
  
	SysExMessage MessageFactory::getGridRouting()
	{
		return getFunction(MIDI_GET_ROUTING_GRID_LAYOUT);
	}
  
	SysExMessage MessageFactory::enableLoopStatus(bool enable)
	{
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(MIDI_LOOPER_STATUS); // Function ID

		if(enable)  
			sysEx.addData(0x01); // 1 = enable
		else
			sysEx.addData(0x00);
		  
		generateChecksum(&sysEx);

		return sysEx;
	}
  
    SysExMessage MessageFactory::setSceneNumber(uint8_t scene)
	{
		SysExMessage sysEx;
		generateHeader(&sysEx);
		
		sysEx.addData(MIDI_SET_SCENE_NUMBER); // Function ID
	  
		sysEx.addData(scene);
		  
		generateChecksum(&sysEx);

		return sysEx;
	}

}
