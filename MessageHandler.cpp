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
	MessageHandler::MessageHandler(MidiInterface* interface, AxeController* controller)
	{
		this->myInterface = interface;
		this->myController = controller;
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
		this->myInterface->setProgram(msg.getProgram());
		this->myController->displayPresetNumber(this->myInterface->getPreset());
		return true;
	}

	bool MessageHandler::handleControlChange(const ControlChangeMessage& msg)
	{
		if(msg.getControllerNumber() == (uint8_t)CC_BANKMSB) //Bank change
		{
			this->myInterface->setBank(msg.getControllerValue());
			return true;
		}
		else
		{
			return false;
		}
	}

	bool MessageHandler::handleSysEx(const SysExMessage& msg)
	{
		const vector<uint8_t>& data = msg.getData();
		//Check it has at least the bare minimum to be a meaningful SysEx message
		if(data.size() < 5)
		{
			return false;
		}
    
		//Check header matches
		if( (data.at(0) != MAN_ID1) || (data.at(1) != MAN_ID2) || (data.at(2) != MAN_ID3) || (data.at(3) != MODEL_ID) )
		{
			return false;
		}
    
		switch(data.at(4) /*Function ID*/ )
		{
			case MIDI_PARAMETER:                                         return this->handleParameter(data);
			case MIDI_MODIFIER:                                          return this->handleModifier(data); //Wiki says response to modifier is 0x02 (MIDI parameter); I am assuming this is a mistake.
			case MIDI_GET_FIRMWARE_VERSION:                              return this->handleFirmwareVersion(data);
			case MIDI_TUNER_INFO:                                        return this->handleTunerInfo(data);
			case MIDI_GET_PRESET_EFFECT_BLOCKS_AND_CC_AND_BYPASS_STATE:  return this->handlePresetState(data);
			case MIDI_GET_PRESET_NAME:                                   return this->handlePresetName(data);
			case MIDI_TEMPO_BEAT:                                        return this->handleTempoBeat();
			case MIDI_PRESET_NUMBER:                                     return this->handlePresetNumber(data);
			case MIDI_GET_ROUTING_GRID_LAYOUT:                           return this->handleGridRouting(data);
			case MIDI_LOOPER_STATUS:                                     return this->handleLooperStatus(data);
			case MIDI_SET_SCENE_NUMBER:                                  return this->handleSceneNumber(data);
			default:                                                     return false;
		}
	}
  
	
	bool MessageHandler::handleGridRouting(const vector<uint8_t>& data)
	{
		uint8_t effects[GRID_COLUMNS][GRID_ROWS];
		bool routing[GRID_COLUMNS][GRID_ROWS][GRID_ROWS];
	 
		for(uint8_t column=0; column < GRID_COLUMNS; column++)
		{
			for(uint8_t row=0; row < GRID_ROWS; row++)
			{
				effects[column][row] = processSeptets(data.at(5+(column*GRID_ROWS)+row),data.at(6+(column*GRID_ROWS)+row));
        
				for(uint8_t input=0; input < GRID_ROWS; input++)
				{
					routing[column][row][input] = (0x01 << input) & data.at(7+(column*GRID_ROWS)+row);
				}
			}
		}
    
		//TODO: Do something with this information
		return validateChecksum(data);
	}
  
      
	bool MessageHandler::handleLooperStatus(const vector<uint8_t>& data)
	{
		bool record = 0x01 & data.at(5); //Bit 0: Record
		bool play = 0x02 & data.at(5); //Bit 1: Play
		bool once = 0x04 & data.at(5); //Bit 2: Once
		bool overdub = 0x08 & data.at(5); //Bit 3: Overdub
		bool reverse = 0x10 & data.at(5); //Bit 4: Reverse
		bool half = 0x20 & data.at(5); //Bit 5: Half
		bool undo = 0x40 & data.at(5); //Bit 6: Undo
    
		uint8_t range = data.at(6);

		//TODO Do something with info
    
		return validateChecksum(data);
	}
  
	bool MessageHandler::handleSceneNumber(const vector<uint8_t>& data)
	{
		uint8_t scene = data.at(5);

		//TODO Do something with info
    
		return validateChecksum(data);
	}
  
	bool MessageHandler::handleParameter(const vector<uint8_t>& data)
	{
		uint16_t effect = processSeptets(data.at(5), data.at(6));
		uint16_t parameter = processSeptets(data.at(7), data.at(8));
		uint16_t value = processSeptets(data.at(9), data.at(10), data.at(11));    
		
		//Not sure what bytes 12-16 do!!!
		
		String text = extractString(data, 17);
		
		// TODO: Do something with this information...
		
		return validateChecksum(data);
	}	
  
	bool MessageHandler::handleModifier(const vector<uint8_t>& data)
	{
		uint16_t effect = processSeptets(data.at(5), data.at(6));
		uint16_t parameter = processSeptets(data.at(7), data.at(8));
		uint8_t selector = data.at(9);
		// byte 10 is apparently always 0
		uint16_t value = processSeptets(data.at(11), data.at(12), data.at(13));    
		
		String text = extractString(data, 14);
		
		// TODO: Do something with this information...
		
		return validateChecksum(data);
	}
	
	bool MessageHandler::handleFirmwareVersion(const vector<uint8_t>& data)
	{
		uint16_t version = processSeptets(data.at(5), data.at(6));

		// bytes 7-10 apparently are unimportant
		
		// TODO: Do something with this information...
		
		return validateChecksum(data);
	}
  
	bool MessageHandler::handleTunerInfo(const vector<uint8_t>& data)
	{
		if(data.size() >= 8)
		{
		  this->myController->displayTuner(NOTE[data.at(5)], data.at(6), data.at(7));
		  return true; //No checksum
		}
		return false;
	}  
  
	bool MessageHandler::handlePresetState(const vector<uint8_t>& data)
	{
		uint16_t i=5;
		while(i < (data.size()-1)) //For the whole data packet, except the checksum
		{
			//For each block
		  
			//Status bits
			bool enabled = data.at(i) & 0x01;
			bool yState = data.at(i) & 0x02;

			uint8_t ccNumber = (data.at(i+1) >> 1) + (data.at(i+2) << 6);
		  
			uint8_t effectId = (data.at(i+3) >> 3) + (data.at(i+4) << 4);
		  
			i+=5;
		}
		// TODO Do something with this info
		return validateChecksum(data);
	}
  
	bool MessageHandler::handlePresetName(const vector<uint8_t>& data)
	{
		String presetName = extractString(data,5);
		if(validateChecksum(data))
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
  
	bool MessageHandler::handlePresetNumber(const vector<uint8_t>& data)
	{
		if(validateChecksum(data))
		{
			this->myInterface->setBank(data.at(5));
			this->myInterface->setProgram(data.at(6));
			this->myController->displayPresetNumber(processSeptets(data.at(5), data.at(6)));
			return true;
		}
		else
		{
			return false;
		}
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
	String MessageHandler::extractString(const vector<uint8_t>& data, uint8_t start)
	{
		String text = "";
		uint8_t i = start;
		while(data.at(i) != 0x00) //Stop at null character
		{
			text += data.at(i); //Concatenate character to string
			i++;
		}
		return text;
	}
  
	/* Not all functions have checksums */
	bool MessageHandler::validateChecksum(const vector<uint8_t>& data)
	{
		uint8_t checksum = 0xF0;
		for(uint8_t i=0; i < data.size()-1; i++)
		{
			checksum ^= data.at(i);
		}
		return (checksum == data.back());
	}  
}
