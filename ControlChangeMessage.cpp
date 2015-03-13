// 
// 
// 

#include "ControlChangeMessage.h"

namespace midi
{
	ControlChangeMessage::ControlChangeMessage(uint8_t channel) : MidiMessage::MidiMessage(PROGRAM_CHANGE)
	{
		this->myChannel = channel;
	}

	ControlChangeMessage::~ControlChangeMessage()
	{
	}

	/* If it is appropriate add data byte to myData as appropriate for the type.
	Update the status of the message accordingly */
	bool ControlChangeMessage::addData(uint8_t newData)
	{
		if (this->myStatus == INCOMPLETE)
		{
			if (this->myData.size() < 2)
			{
				this->myData.push_back(newData);
				if (this->myData.size() == 2)
				{
					this->myStatus = COMPLETE;
				}
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

	uint8_t ControlChangeMessage::getChannel()
	{
		return this->getChannel();
	}

}

