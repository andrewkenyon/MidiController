// 
// 
// 

#include "ControlChangeMessage.h"

namespace midi
{
	ControlChangeMessage::ControlChangeMessage(uint8_t channel) : MidiMessage::MidiMessage(CONTROL_CHANGE)
	{
		this->myChannel = channel;
		this->myDataCounter = 0;
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
			if (this->myDataCounter < 1)
			{
				this->myControllerNumber == newData;
			}
			else if (this->myDataCounter < 2)
			{
				this->myControllerValue == newData;
				if (this->myDataCounter == 2)
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

	uint8_t ControlChangeMessage::getChannel() const
	{
		return this->myChannel;
	}

	uint8_t ControlChangeMessage::getControllerNumber() const
	{
		return this->myControllerNumber;
	}

	uint8_t ControlChangeMessage::getControllerValue() const
	{
		return this->myControllerValue;
	}
}

