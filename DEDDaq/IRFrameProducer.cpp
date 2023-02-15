#pragma once
#include "IRFrameProducer.h"
#include "sdk/IRDevice.h"
#include "sdk/IRImager.h"

// Constructor for class
IRFrameProducer::IRFrameProducer()
{
	// Use the XML file to configure the initial device parameters
	// struct
	evo::IRImager associatedImager;
	char c_xmlPath[] = "D:\\repos\\ded-logger\\irConfigData\\21114001.xml";
	
	auto retcode = evo::IRDeviceParamsReader::readXMLC(c_xmlPath, 
														associatedConfig);
	// See if that worked.
	if (0) {
		std::cout << "Something went wrong loading the config file!"
		<< std::endl;
	}
	// It worked!
	else 
	{
		std::cout << "Configured for SN:" << 
			associatedConfig.serial << std::endl;
	}
	IRFrameProducer::initializeImager();

}

void IRFrameProducer::initializeImager()
{
	evo::IRDevice* imagerDevice = evo::IRDevice::IRCreateDevice(associatedConfig);
	if (imagerDevice)
	{
		if (associatedImager.init(&associatedConfig, imagerDevice->getFrequency(),
			imagerDevice->getWidth(), imagerDevice->getHeight(),
			imagerDevice->controlledViaHID()))
		{
			std::cout << "Success! IR Initialized!" << std::endl;
		}
		else {
			std::cout << "Something went wrong!";
		}
	}
}
