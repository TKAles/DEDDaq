#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>

// National Instruments DAQmx 
// C Interface
#include "NIDAQmx.h"
#include "nisyscfg.h"
/*
	DAQ Class
	Container for NI USB-6216(BNC) Data Acquisition System.
	Version 0.1
	Thomas Ales | Iowa State Univ. 
	Feb 2023
*/

class DAQ
{
public:
	DAQ();
	~DAQ();

	// Handles to various DAQ 'Tasks' 
	TaskHandle ClockTask;			// Sets triggering signal for cameras
									// and DAQ internal sample clock syncs
									// to this task.
	
	// Functions to setup the trigger signal for the camera
	// systems and DAQ samples.
	void ConfigureClock(float _frequency, float _duty = 0.50);
	void StartClock();
	void StopClock();

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

	// Utilities
	std::string LookupDAQError(int32 _errorCode);
};

