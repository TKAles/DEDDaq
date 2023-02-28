#include "DAQ.h"
/*
	DAQ Class
	Container for NI USB-6216(BNC) Data Acquisition System.
	Version 0.1
	Thomas Ales | Iowa State Univ.
	Feb 2023
*/

DAQ::DAQ()
{
	// Constructor.
	DAQ::stopTheClock = false;

}

DAQ::~DAQ()
{
	// Destructor.
}

// ConfigureClock is a convenience function to set the DAQmx
// counter pin. Supply the frequency in Hertz and the duty cycle
// of the waveform as a fraction of 1.0
// For the 6126, the default output pins are:
// Counter 0 - 'ctr0' - PFI 12 / P2.4
// Counter 1 - 'ctr1' - PFI 11 / P2.3
// Frequency - ?????  - PFI 14 / P2.6
void DAQ::ConfigureClock(float _frequency, float _duty)
{
	// Assign input parameters to object properties.
	DAQ::Frequency = _frequency;
	DAQ::DutyCycle = _duty;
	auto _createRet = DAQmxCreateTask("TriggerSignal", &ClockTask);
	if (_createRet != 0)
	{
	std::cout << "Creation: " 
		<< DAQ::LookupDAQError(_createRet) << std::endl;
	}
	// DAQmx Create COunter Pulse Channel, Frequency
	auto _ChanErr = DAQmxCreateCOPulseChanFreq(
		DAQ::ClockTask,	
		DAQ::OutputPort.c_str(),
		"",
		DAQ::ClockUnits,
		DAQ::IdleState,
		DAQ::InitialDelay,
		DAQ::Frequency,
		DAQ::DutyCycle);			
	// Error Check for Channel Config
	if (_ChanErr != 0)
	{
		// Look up the NI error and print the string to stdout.
		std::cout << "There was an issue configuring the trigger clock!"
			<< std::endl << "DAQ Reports that \"" << 
			DAQ::LookupDAQError(_ChanErr) << "\"." << std::endl;
		return;
	}
	// If no error, continue with internal timing configuration
	auto _timingErr = DAQmxCfgImplicitTiming(
		DAQ::ClockTask,				// TaskHandle
		DAQ::SampleMode,			// Sampling Mode 
		DAQ::BufferSize);			// Buffer Size or # of Samples

	// Error Check for Timing Config
	if (_timingErr != 0)
	{
		// Same deal as 46-9
		std::cout << "There was an issue configuring the trigger clock!"
			<< std::endl << "DAQ Reports that \"" <<
			DAQ::LookupDAQError(_timingErr) << "\"." << std::endl;
	}
}

void DAQ::StartClock()
{
	// TODO: Write function to spawn thread and wait
	//       for condition variable to stop the clock
	
	// Take the lock and start the clock signal
	std::cout << "Taking DAQ::_ClockMutex and Locking!!" << std::endl;
	std::unique_lock _clockLock(DAQ::_ClockMutex);
	auto _taskErr = DAQmxStartTask(DAQ::ClockTask);
	if (_taskErr != 0)
	{
		std::cout << "An error occurred starting the trigger signal!" <<
			std::endl << "DAQ Error is \"" << DAQ::LookupDAQError(_taskErr)
			<< "\"." << std::endl;
	}
	// Wait for the condition variable to trip
	// Need to pass the local environment into the lambda
	// so it can see if the stopTheStream flag has been tripped yet
	std::cout << "Waiting for CV DAQ::_ClockSynch!" << std::endl;
	DAQ::_ClockSynch.wait(_clockLock,
		[&] { return DAQ::stopTheClock == true;});
	std::cout << "Recieved request to stop trigger signal!" <<
		std::endl;
	DAQ::stopTheClock = false;	// reset signal
	auto _stopTaskErr = DAQmxStopTask(DAQ::ClockTask);
	if (_stopTaskErr != 0)
	{
		std::cout << "Something went wrong trying to stop the trigger!" <<
			std::endl << "DAQ Reported \"" << DAQ::LookupDAQError(_stopTaskErr) <<
			"\"." << std::endl;
	}
}

void DAQ::StopClock()
{
	// TODO: Trigger condition variable to shutdown
	//       the clock task.
	DAQ::stopTheClock = true;
	DAQ::_ClockSynch.notify_one();
	return;
}

// Takes the cryptic error code and returns a slightly less cryptic 
// error message from the National Instruments DAQmx library.
std::string DAQ::LookupDAQError(int32 _errorCode)
{
	// get the required buffer size
	// pass NULL and 0 to get function to tell you needed buffer size
	int32 bufSize = DAQmxGetErrorString(_errorCode, NULL, 0);
	// get the error text and convert to a string and return
	// the string
	char* errorMsg = new char[bufSize];
	DAQmxGetErrorString(_errorCode, errorMsg, bufSize);
	return std::string(errorMsg);
}