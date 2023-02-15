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
	evo::IRImager associatedImager;
	evo::IRDeviceParams associatedConfig;
	
private:
	char c_xmlPath[];
};

