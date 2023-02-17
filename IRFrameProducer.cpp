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
		std::cout << "IR:\t IRImageParams configured for SN:" << 
			associatedConfig.serial << std::endl;
	}
	IRFrameProducer::initializeImager();

}

void IRFrameProducer::initializeImager()
{
	evo::IRDevice* imagerDevice = evo::IRDevice::IRCreateDevice(associatedConfig);
	if (imagerDevice)
	{
		// Initialize the IRImager object by passing the configuration
		// parameters in and device info. Assumes that the associatedConfig
		// has been populated with the correct XML file and that the 
		// associatedImager has been initialized.
		if (associatedImager.init(&associatedConfig, imagerDevice->getFrequency(),
			imagerDevice->getWidth(), imagerDevice->getHeight(),
			imagerDevice->controlledViaHID()))
		{
			std::cout << "IR:\t Imager object reports resolution of " <<
				associatedImager.getWidth() << "x" <<
				associatedImager.getHeight() << "px@" <<
				associatedImager.getMaxFramerate() << "fps." << std::endl;
		}
		else {

			// figure out how to handle errors at some point in the 
			// future
			std::cout << "Something went wrong!";
		}
	}
}

void IRFrameProducer::configureTriggering()
{
	return;
}
