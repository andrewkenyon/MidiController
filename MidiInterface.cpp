/*!
 *  @file       MidiInterface.cpp
 *  @brief      Interface between foot controller and midi I/O
 *  @version    0.1
 *  @author     Andrew Kenyon
 *  @date       30/06/2015
 */

#include "MidiInterface.h"
#include "AxeController.h"

#include "MessageHandler.h"
#include "MessageFactory.h"

using namespace std;

namespace midi
{    
	MidiInterface::MidiInterface(int8_t channel)
	{
		this->myConnection = new MidiConnection();
		this->myController = new AxeController(this);
		this->myChannel = channel;
		
		this->myProgram = 0;
		this->myBank = 0;
		  
		this->myTempo = 60; 
		this->myPreviousTempoPulse = millis();
	}
  
	MidiInterface::MidiInterface()
	{
		this->myConnection = new MidiConnection();
		this->myController = new AxeController(this);
		this->myChannel = -1; //Listen to all

		this->myProgram = 0;
		this->myBank = 0;

		this->myTempo = 60;
		this->myPreviousTempoPulse = millis();
	}

	MidiInterface::~MidiInterface()
	{
		delete this->myConnection;
		delete this->myController;
	}
  
	//TODO
	void MidiInterface::test(uint16_t testInt)
	{
		this->changePreset(testInt);
		/*ProgramChangeMessage pc((uint8_t)0);
		if(pc.addData((uint8_t) testInt%128))
		{
			this->myConnection->sendMessage(pc);
		}*/
		delay(100);
		
		this->update();
	}
  
	void MidiInterface::update()
	{
		//MIDI input
		while(this->myConnection->checkBuffer())
		{
			const MidiMessage& msg = this->myConnection->getMsg();
			MessageHandler handler(this, this->myController);
			handler.handleMsg(msg);
		}
    
		//Foot controller input
		//this->myController->updateLeds(this->handleFootswitches(this->myController->updateFootswitches()));
		
		//MIDI output
		
		//Foot controller output
		this->pulseTempo();
	}
  
	uint16_t MidiInterface::getPreset() const
	{
		return (uint16_t)((uint16_t)this->myBank*(uint16_t)128 + (uint16_t)this->myProgram);
	}

	int8_t MidiInterface::getChannel() const
	{
		return this->myChannel;
	}	
	
	/* Sets preset regardless of whether the message is recieved by downstream devices */
	void MidiInterface::changePreset(uint16_t preset)
	{
		MessageFactory factory(this);
		
		if(this->myBank != (uint8_t)(preset/(uint16_t)128))
		{
			this->myBank = (uint8_t)(preset/(uint16_t)128);
			this->myConnection->sendMessage(factory.bankChange(this->myBank));
		}
		
		this->myProgram = (uint8_t)(preset % 128);
		this->myConnection->sendMessage(factory.programChange(this->myProgram));
	}

	/***********************************************************/
	
	void MidiInterface::updateTempo()
	{
		this->myController->displayTempoPulse(true);   
		unsigned long currentTime = millis();
		this->myTempo = (myPreviousTempoPulse - currentTime)/60;
		this->myPreviousTempoPulse = currentTime;
		this->myController->displayBpm(myTempo);

	}
  
	/* Clears tempo "!" mark if half way through pulses, 
	in order to show as 50/50 on/off pulse */
	void MidiInterface::pulseTempo()
	{
		unsigned long currentMillis = millis();
		if (currentMillis > (myPreviousTempoPulse+myTempo/2))
		{
			this->myController->displayTempoPulse(false);
		}
		else
		{
			this->myController->displayTempoPulse(true);
		}
	}
}
