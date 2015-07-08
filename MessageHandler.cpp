/*!
 *  @file       MessageHandler.cpp
 *  @brief      Handles MidiMessages using the MessageHandler
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       29/06/2015
 */

#include "MessageHandler.h"

using namespace std;

namespace midi
{    
	MessageHandler::MessageHandler(MidiInterface* interface, const uint8_t bank, AxeController* controller)
	{
		this->myInterface = interface;
		this->myBank = bank;
		
		this->myController = controller;
	}
	
	MessageHandler::~MessageHandler()
	{
	}
	 
	/************************** Handle Messages *****************************/
	
	bool MessageHandler::handleMsg(const MidiMessage& msg)
	{
		switch (msg.getType())
		{
			case PROGRAM_CHANGE:
			{
				if ( (( ((ProgramChangeMessage&)msg).getChannel() ) == this->myInterface->getChannel()) || (this->myInterface->getChannel() == -1) )
				{
					return this->handleProgramChange((ProgramChangeMessage&)msg);
				}
				else
				{
					return false;
				}
			}
			case CONTROL_CHANGE:
			{
				if ( (( ((ControlChangeMessage&)msg).getChannel()) == this->myInterface->getChannel()) || (this->myInterface->getChannel() == -1) )
				{
					return this->handleControlChange((ControlChangeMessage&)msg);
				}
				else
				{
					return false;
				}
			}
			case SYSTEM_MESSAGE:
			{
				return this->handleSysEx((SysExMessage&)msg);
			}
			default:
			{
				return false;
			}
		}
	}

	bool MessageHandler::handleProgramChange(const ProgramChangeMessage& msg)
	{
		this->myInterface->changePreset(this->myBank*128 + msg.getProgram(), false);
		return true;
	}

	bool MessageHandler::handleControlChange(const ControlChangeMessage& msg)
	{
		if(msg.getControllerNumber() == (uint8_t)CC_BANKMSB) //Bank change
		{
			if(this->myBank != msg.getControllerValue())
			{
				this->myBank = msg.getControllerValue();
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool MessageHandler::handleSysEx(const SysExMessage& msg)
	{
		//Check it has at least the bare minimum to be a meaningful SysEx message
		if(msg.payloadSize() < 5)
		{
			return false;
		}
    
		//Check header matches
		if( (msg.payloadAt(0) != MAN_ID1) || (msg.payloadAt(1) != MAN_ID2) || (msg.payloadAt(2) != MAN_ID3) || (msg.payloadAt(3) != MODEL_ID) )
		{
			return false;
		}
    
		switch(msg.payloadAt(4) /*Function ID*/ )
		{
			case MIDI_PARAMETER:                                         return this->handleParameter(msg);
			case MIDI_MODIFIER:                                          return this->handleModifier(msg); //Wiki says response to modifier is 0x02 (MIDI parameter); I am assuming this is a mistake.
			case MIDI_GET_FIRMWARE_VERSION:                              return this->handleFirmwareVersion(msg);
			case MIDI_TUNER_INFO:                                        return this->handleTunerInfo(msg);
			case MIDI_GET_PRESET_EFFECT_BLOCKS_AND_CC_AND_BYPASS_STATE:  return this->handlePresetState(msg);
			case MIDI_GET_PRESET_NAME:                                   return this->handlePresetName(msg);
			case MIDI_TEMPO_BEAT:                                        return this->handleTempoBeat();
			case MIDI_PRESET_NUMBER:                                     return this->handlePresetNumber(msg);
			case MIDI_GET_ROUTING_GRID_LAYOUT:                           return this->handleGridRouting(msg);
			case MIDI_LOOPER_STATUS:                                     return this->handleLooperStatus(msg);
			case MIDI_SET_SCENE_NUMBER:                                  return this->handleSceneNumber(msg);
			default:                                                     return false;
		}
	}

	bool MessageHandler::handleParameter(const SysExMessage& msg)
	{
		uint16_t effect = processSeptets(msg.payloadAt(5), msg.payloadAt(6));
		uint16_t parameter = processSeptets(msg.payloadAt(7), msg.payloadAt(8));
		uint16_t value = processSeptets(msg.payloadAt(9), msg.payloadAt(10), msg.payloadAt(11));    
		
		//Not sure what bytes 12-16 do!!!
		
		String text = extractString(msg, 17);
		
		// TODO: Do something with this information...
		
		return validateChecksum(msg);
	}	
  
	bool MessageHandler::handleModifier(const SysExMessage& msg)
	{
		uint16_t effect = processSeptets(msg.payloadAt(5), msg.payloadAt(6));
		uint16_t parameter = processSeptets(msg.payloadAt(7), msg.payloadAt(8));
		uint8_t selector = msg.payloadAt(9);
		// byte 10 is apparently always 0
		uint16_t value = processSeptets(msg.payloadAt(11), msg.payloadAt(12), msg.payloadAt(13));    
		
		String text = extractString(msg, 14);
		
		// TODO: Do something with this information...
		
		return validateChecksum(msg);
	}
	
	bool MessageHandler::handleFirmwareVersion(const SysExMessage& msg)
	{
		uint16_t version = processSeptets(msg.payloadAt(5), msg.payloadAt(6));

		// bytes 7-10 apparently are unimportant
		
		// TODO: Do something with this information...
		
		return validateChecksum(msg);
	}
  
	bool MessageHandler::handleTunerInfo(const SysExMessage& msg)
	{
		if(msg.payloadSize() >= 8)
		{
		  this->myController->displayTuner(NOTE[msg.payloadAt(5)], msg.payloadAt(6), msg.payloadAt(7));
		  return true; //No checksum
		}
		return false;
	}  
  
	bool MessageHandler::handlePresetState(const SysExMessage& msg)
	{
		uint16_t i=5;
		while(i < (msg.payloadSize()-1)) //For the whole data packet, except the checksum
		{
			//For each block
		  
			//Status bits
			bool enabled = msg.payloadAt(i) & 0x01;
			bool yState = msg.payloadAt(i) & 0x02;

			uint8_t ccNumber = (msg.payloadAt(i+1) >> 1) + (msg.payloadAt(i+2) << 6);
		  
			uint8_t effectId = (msg.payloadAt(i+3) >> 3) + (msg.payloadAt(i+4) << 4);
		  
			i+=5;
		}
		// TODO Do something with this info
		return validateChecksum(msg);
	}
  
	bool MessageHandler::handlePresetName(const SysExMessage& msg)
	{
		String presetName = extractString(msg,5);
		if(validateChecksum(msg))
		{
		this->myController->displayProgramName(presetName);
		return true;
		}
		else
		{
			return false;
		}    
	}
    
	bool MessageHandler::handleTempoBeat()
	{    
		this->myInterface->updateTempo();
	}
  
	bool MessageHandler::handlePresetNumber(const SysExMessage& msg)
	{
		if(validateChecksum(msg))
		{
			this->myInterface->changePreset(processSeptets(msg.payloadAt(5), msg.payloadAt(6)), false);
			return true;
		}
		else
		{
			return false;
		}
	}
	
	  
	
	bool MessageHandler::handleGridRouting(const SysExMessage& msg)
	{
		uint8_t effects[GRID_COLUMNS][GRID_ROWS];
		bool routing[GRID_COLUMNS][GRID_ROWS][GRID_ROWS];
	 
		for(uint8_t column=0; column < GRID_COLUMNS; column++)
		{
			for(uint8_t row=0; row < GRID_ROWS; row++)
			{
				effects[column][row] = processSeptets(msg.payloadAt(5+(column*GRID_ROWS)+row),msg.payloadAt(6+(column*GRID_ROWS)+row));
        
				for(uint8_t input=0; input < GRID_ROWS; input++)
				{
					routing[column][row][input] = (0x01 << input) & msg.payloadAt(7+(column*GRID_ROWS)+row);
				}
			}
		}
    
		//TODO: Do something with this information
		return validateChecksum(msg);
	}
  
      
	bool MessageHandler::handleLooperStatus(const SysExMessage& msg)
	{
		bool record = 0x01 & msg.payloadAt(5); //Bit 0: Record
		bool play = 0x02 & msg.payloadAt(5); //Bit 1: Play
		bool once = 0x04 & msg.payloadAt(5); //Bit 2: Once
		bool overdub = 0x08 & msg.payloadAt(5); //Bit 3: Overdub
		bool reverse = 0x10 & msg.payloadAt(5); //Bit 4: Reverse
		bool half = 0x20 & msg.payloadAt(5); //Bit 5: Half
		bool undo = 0x40 & msg.payloadAt(5); //Bit 6: Undo
    
		uint8_t range = msg.payloadAt(6);

		//TODO Do something with info
    
		return validateChecksum(msg);
	}
  
	bool MessageHandler::handleSceneNumber(const SysExMessage& msg)
	{
		uint8_t scene = msg.payloadAt(5);

		//TODO Do something with info
    
		return validateChecksum(msg);
	}
  
	/**************************************************************/
	
	/* Process 16-bit unsigned number transferred as 3 septets.
	sept0 is the bottom 7, sept1 the next 7, and sept2 is the top 2. */
	uint16_t MessageHandler::processSeptets(uint8_t sept0, uint8_t sept1, uint8_t sept2)
	{
		return sept0 + ((uint16_t)sept1 << 7) + ((uint16_t)sept2 << 14);
	}
  
	/* Process 16-bit unsigned number transferred as 3 septets.
	sept0 is the bottom 7, sept1 the next 7, and sept2 is the top 2. */
	uint16_t MessageHandler::processSeptets(uint8_t sept0, uint8_t sept1)
	{
		return sept0 + ((uint16_t)sept1 << 7);
	}
		
	/* Extract a string stored as a set of MIDI SysEx bytes */
	String MessageHandler::extractString(const SysExMessage& msg, uint8_t start)
	{
		String text = "";
		uint8_t i = start;
		while(msg.payloadAt(i) != 0x00) //Stop at null character
		{
			text += msg.payloadAt(i); //Concatenate character to string
			i++;
		}
		return text;
	}
	
	/* Not all functions have checksums */
	bool MessageHandler::validateChecksum(const SysExMessage& msg)
	{
		uint8_t checksum = 0xF0;
		for(uint8_t i=0; i < msg.payloadSize()-1; i++)
		{
			checksum ^= msg.payloadAt(i);
		}
		return (checksum == msg.payloadAt(msg.payloadSize()-1));
	}  
}
