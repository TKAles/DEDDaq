#include "DAQ.h"

DAQ::DAQ()
{
	// Constructor.
}

DAQ::~DAQ()
{
	// Destructor.
}

void DAQ::ConfigureClock(float _frequency, float _duty = 0.50)
{

}

void DAQ::StartClock()
{
	// TODO: Write function to spawn thread and wait
	//       for condition variable to stop the clock
}

void DAQ::StopClock()
{
	// TODO: Trigger condition variable to shutdown
	//       the clock task.
}

// Takes the cryptic error code and returns a slightly less cryptic 
// error message from the National Instruments DAQmx library.
std::string LookupDAQError(int32 _errorCode)
{
	// get the required buffer size
	int32 bufSize = DAQmxGetErrorString(_errorCode, NULL, 0);
	// get the error text and convert to a string and return
	// the string
	char* errorMsg = new char[bufSize];
	DAQmxGetErrorString(_errorCode, errorMsg, bufSize);
	return std::string(errorMsg);
}