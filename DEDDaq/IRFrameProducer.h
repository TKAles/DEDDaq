#pragma once
#include "sdk/IRDevice.h"
#include "sdk/IRImager.h"
#include "sdk/IRLogger.h"
#include "sdk/IRFileWriter.h"
#include "sdk/Timer.h"

class IRFrameProducer
{
public:
	IRFrameProducer();
	void initializeImager();
	void configureTriggering();
	evo::IRImager associatedImager;
	evo::IRDeviceParams associatedConfig;
	evo::IRDevice* associatedDevice;

private:
	char c_xmlPath[];
};

