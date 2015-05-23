// 
// 
// 

#include "SysExMessage.h"

using namespace std;

namespace midi
{

	SysExMessage::SysExMessage() : MidiMessage::MidiMessage(SYSTEM_MESSAGE)
	{
	}
	SysExMessage::~SysExMessage()
	{
	}

	/* If it is appropriate add data byte to myData as appropriate for the type.
	Update the status of the message accordingly */
	bool SysExMessage::addData(uint8_t newData)
	{
		if (this->myStatus == INCOMPLETE)
		{
			if (newData == 0x7F)
			{
				this->myStatus = COMPLETE;
			}
			else
			{
				this->myData.push_back(newData);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	const vector<uint8_t>& SysExMessage::getData() const
	{
		return this->myData;
	}


}