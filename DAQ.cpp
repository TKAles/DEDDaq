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
	auto _err = DAQmxCreateTask("Main Clock", &ClockTask);
	if (_err != 0)
	{
		std::cout << "Something went wrong making the task!" << std::endl <<
			"DAQ Says: " << DAQ::LookupDAQError(_err) << "." << std::endl;
	}
}

DAQ::~DAQ()
{
	// Destructor.
}

// ConfigureClock is a convenience function to set the DAQmx
// up as a trigger source for the system.
// It will emit a 5V, 50% duty pulse train at the given
// frequency in Hz. ConfigureClock must be called successfully
// before StartClock is called, or else StartClock will hang.
void DAQ::ConfigureClock(float _frequency, std::string _outputPort)
{
	std::cout << "output port = " << DAQ::OutputPort << std::endl;
	DutyCycle = 0.50f;
	Frequency = _frequency;
	OutputPort = _outputPort;
	auto _err = DAQmxCreateAOFuncGenChan(
		DAQ::ClockTask,
		OutputPort.c_str(),
		"",
		DAQmx_Val_Sine,
		Frequency,
		5.0f,
		2.5f);
	if (_err != 0)
	{
		std::cout << "Something went wrong trying to configure " <<
			"the AnalogOutput channel!" << std::endl <<
			"DAQ Says: " << DAQ::LookupDAQError(_err) << "." << std::endl;
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