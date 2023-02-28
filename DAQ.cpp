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
}

DAQ::~DAQ()
{
	// Destructor.
}

// ConfigureClock is a convenience function to set the DAQmx
// counter pin. Supply the frequency in Hertz and the duty cycle
// of the waveform as a fraction of 1.0
// For the 6126, the default output pins are:
// Counter 0 - 'ctr0' - PFI 12
// Counter 1 - 'ctr1' - PFI 11
// Frequency - ?????  - PFI 14

void DAQ::ConfigureClock(float _frequency, float _duty = 0.50)
{
	// Assign input parameters to object properties.
	DAQ::Frequency = _frequency;
	DAQ::DutyCycle = _duty;
	// DAQmx Create COunter Pulse Channel, Frequency
	auto _ChanErr = DAQmxCreateCOPulseChanFreq(
		DAQ::ClockTask,				// TaskHandle
		DAQ::OutputPort.c_str(),	// Port in DevX/ctrY format
		"",							// alternative label
		DAQ::ClockUnits,			// units, Hertz
		DAQ::IdleState,				// state of the line when not running
		DAQ::InitialDelay,			// initial delay in seconds
		DAQ::Frequency,				// frequency in hertz
		DAQ::DutyCycle);			// duty cycle as fraction
	// Error Check for Channel Config
	if (_ChanErr != 0)
	{
		// Look up the NI error and print the string to stdout.
		std::cout << "There was an issue configuring the trigger clock!"
			<< std::endl << "DAQ Reports that \"" << 
			DAQ::LookupDAQError(_ChanErr) << "\"." << std::endl;
	}
	// If no error, continue with internal timing configuration
	auto _timingErr = DAQmxCfgImplicitTiming(
		DAQ::ClockTask,
		DAQ::SampleMode,
		DAQ::BufferSize);
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
	// pass NULL and 0 to get function to tell you needed buffer size
	int32 bufSize = DAQmxGetErrorString(_errorCode, NULL, 0);
	// get the error text and convert to a string and return
	// the string
	char* errorMsg = new char[bufSize];
	DAQmxGetErrorString(_errorCode, errorMsg, bufSize);
	return std::string(errorMsg);
}