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
	std::string LookupDAQError(int32 _errorCode);

	// These parameters are for configuring the
	// 'counter' output channel. This counter output
	// will generate the trigger signal for the cameras.
	std::string OutputPort;
	int32 ClockUnits = DAQmx_Val_Hz;
	int32 IdleState = DAQmx_Val_Low;
	float64 InitialDelay = 0.0;
	float64 Frequency = 0.0;
	float64 DutyCycle = 0.0;
	// These are some 'implicit timing' paramaters
	// that need to be set in order to use the counter.
	int32 SampleMode = DAQmx_Val_ContSamps;
	uInt64 BufferSize = 250;

};

