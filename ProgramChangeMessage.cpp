// 
// 
// 

#include "ProgramChangeMessage.h"

namespace midi
{
	class MidiMessage;

	ProgramChangeMessage::ProgramChangeMessage(uint8_t channel) : MidiMessage::MidiMessage(PROGRAM_CHANGE)
	{
		this->myChannel = channel;
	}

	ProgramChangeMessage::~ProgramChangeMessage()
	{
	}
	/* If it is appropriate add data byte to myData as appropriate for the type.
	Update the status of the message accordingly */
	bool ProgramChangeMessage::addData(uint8_t newData)
	{
		if (this->myStatus == INCOMPLETE)
		{
			if (this->myData.size() < 1)
			{
				this->myData.push_back(newData);
				this->myStatus = COMPLETE;
				return true;
			}
			else
			{
				this->myStatus = INVALID;
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	byte ProgramChangeMessage::getChannel()
	{
		return this->getChannel();
	}
}
