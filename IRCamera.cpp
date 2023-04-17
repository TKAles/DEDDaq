#include "IRCamera.h"

IRCam::IRCam()
{
	// Constructor
	LogFilePath = std::string("C:\\ded-test\\file.log");
	CalibrationPath = std::string("C:\\Users\\Thomas Ales\\AppData\\Roaming\\Imager\\Cali");
	ConfigXMLPath = std::string("C:\\ded-test\\Xi400Config.xml");
}

IRCam::~IRCam()
{
	// Destructor
}

int IRCam::StartupIR()
{
	evo::IRLogger::setVerbosity(evo::IRLOG_DEBUG, evo::IRLOG_OFF, this->LogFilePath.c_str());
	evo::IRDeviceParamsReader::readXMLC(this->ConfigXMLPath.c_str(), this->CameraParameters);
	evo::IRDeviceParams_Print(this->CameraParameters);
	this->CameraDevice = evo::IRDevice::IRCreateDevice(this->CameraParameters);
	if (this->CameraDevice)
	{
		this->cManager = new evo::IRCalibrationManager(this->CameraParameters.caliPath,
			this->CameraParameters.formatsPath);

		return 0;
		
	}
	else {
		std::cout << "Something went wrong trying to get the camera!" << std::endl;
		std::cout << this->CameraDevice << " hmm." << std::endl;
		return -1;
	}
}

int IRCam::StartIRCamera()
{
	return 0;
}

int IRCam::StopIRCamera()
{
	return 0;
}

void IRCam::IRWorker()
{
	this->ThermalImageHeight = CameraDevice->getHeight();
	this->ThermalImageWidth = CameraDevice->getWidth();
	this->FrameBufferSize = CameraDevice->getRawBufferSize();
	while (isStreaming)
	{

	}
}

