// 
// 
// 

#include "ProgramChangeMessage.h"

namespace midi
{
	ProgramChangeMessage::ProgramChangeMessage(uint8_t channel) : MidiMessage::MidiMessage(PROGRAM_CHANGE)
	{
		this->myChannel = channel;
	}

	ProgramChangeMessage::~ProgramChangeMessage()
	{
	}
	/* If it is appropriate add data byte to myData as appropriate for the type.
	Update the status of the message accordingly */
	bool ProgramChangeMessage::addData(const uint8_t newData)
	{
		if (this->myStatus == INCOMPLETE)
		{
				this->myProgram = newData;
				this->myStatus = COMPLETE;
				return true;
		}
		else
		{
			this->myStatus = INVALID;
			return false;
		}
	}

	uint8_t ProgramChangeMessage::getChannel() const
	{
		return this->getChannel();
	}

	uint8_t ProgramChangeMessage::getProgram() const
	{
		return this->myProgram;
	}
}
