#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <mutex>

// National Instruments DAQmx 
// C Interface
#include "NIDAQmx.h"
#include "nisyscfg.h"
class DAQ
{
public:
	DAQ();
	~DAQ();
	
	TaskHandle ClockTask;
	
	void ConfigureClock(float _frequency, float _duty = 0.50);
	void StartClock();
	void StopClock();
};

