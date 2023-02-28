if (DAQ_TEST)
{

	// prototyping for DAQ connection
	// Get handle for DAQ. 0 = successful
	TaskHandle _daqitydaq;
	if (!DAQmxCreateTask("ClockSignal", &_daqitydaq))
	{
		std::cout << "Created test task ClockSignal" << std::endl;
		// Create a test clock signal
		char _outPort[] = "Dev1/ctr0";
		int32 _pulseUnits = DAQmx_Val_Hz;
		int32 _idlState = DAQmx_Val_High;
		float64 _initDelay = 0.0;
		float64 _freq = 80.0;
		float64 _duty = 0.5;
		// Returns zero if successful.
		// Positive return = Warning. Negative Return = Error.
		// TODO: Add DAQmx error codes!

		auto retval = DAQmxCreateCOPulseChanFreq(_daqitydaq,
			_outPort,	 /* physical output port */
			"",			 /* friendly name */
			_pulseUnits, /* always Hertz */
			_idlState,   /* idle state of pin */
			_initDelay,  /* initial delay in seconds */
			_freq,		 /* frequency in hertz */
			_duty);		 /* duty cycle as fraction of 1.0 */
		if (retval == 0)
		{
			std::cout << "Port has been configured!" << std::endl;
			// Setup the internal timing (required)
			DAQmxCfgImplicitTiming(_daqitydaq, DAQmx_Val_ContSamps,
				1000);
			if (!DAQmxStartTask(_daqitydaq))
			{
				std::string bs;
				std::cout << "Press enter to stop";
				std::cin >> bs;
				DAQmxStopTask(_daqitydaq);
			}
			else {
				std::cout << "Something went wrong!" << std::endl;
			}

		}
		else if (retval > 0)
		{

			std::cout << "A warning was generated during trigger signal configuration!" << std::endl;
			std::cout << "Error Code was " << retval << "." << std::endl;
		}
		else if (retval < 0)
		{
			std::cout << "An error occurred during trigger signal configuration!" << std::endl;
			std::cout << "Error Code was " << retval << "." << std::endl;
			int _toBuffer = DAQmxGetErrorString(retval, NULL, 0);
			char* _errMsg = new char[_toBuffer];
			auto retcode = DAQmxGetErrorString(retval, _errMsg, _toBuffer);
			if (retcode == 0)
			{
				std::cout << "Error message is " << std::endl <<
					_errMsg << "." << std::endl;
			}
		}
		else {
			std::cout << "Something went wrong trying to create the trigger signal!" << std::endl;
		}
	}
	else {
		std::cout << "Something went wrong creating the ClockSignal task!" << std::endl;
		return;
	}
}