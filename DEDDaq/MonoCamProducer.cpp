#include <chrono>
#include <iostream>
#include <thread>
#include "MonoCamProducer.h"
#include "MonoFrameObserver.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/VimbaCPPCommon.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace std::chrono_literals;
using namespace AVT;
using namespace AVT::VmbAPI;

int timesRan = 0;
bool countCompleted;

CameraPtrVector foundCameras;
VimbaSystem& monocamSystem = VimbaSystem::GetInstance();

std::string cameraName;
std::string cameraID;
std::vector<std::string> foundCameraNames;
std::vector<std::string> foundCameraIDs;

std::vector<CameraPtr> activeCameras;


MonoCamProducer::MonoCamProducer() : monocamSystem (VimbaSystem::GetInstance())
{
	// Constructor
}

// Starts the API up. Any calls will fail if this isn't
// called before anything else.
VmbErrorType MonoCamProducer::startupCamera()
{
	return monocamSystem.Startup();
}
// Shuts the API down. 
VmbErrorType MonoCamProducer::shutdownCamera()
{
	return monocamSystem.Shutdown();
}
// Looks for any connected cameras and grabs the model
// name and device id. Stores them in the foundCameraNames
// and foundCameraIDs vectors as strings. 
void MonoCamProducer::registerCameras()
{
	if (VmbErrorSuccess == monocamSystem.GetCameras(foundCameras))
	{
		if (foundCameras.size() == 0) {
			std::cout << "No cameras detected. If cameras are plugged in, check the USB cables.\n";
		}

		else {
			for (CameraPtrVector::iterator iter = foundCameras.begin();
				foundCameras.end() != iter; iter++)
			{
				(*iter)->GetName(cameraName);
				(*iter)->GetID(cameraID);
				foundCameraNames.push_back(cameraName);
				foundCameraIDs.push_back(cameraID);
				MonoCamProducer::activateCamera(cameraID);
			}
			std::cout << "Found " << foundCameraIDs.size() << " cameras.\n";
		}

	} else {
		std::cout << "Something went wrong while detecting the cameras!";
	}
}
// Configures a camera and drops the camera into streaming
// mode. Designed to be launched in it's own thread.
void MonoCamProducer::activateCamera(std::string _cameraID)
{
	CameraPtr _selectedCamera;
	monocamSystem.OpenCameraByID(_cameraID.c_str(), VmbAccessModeFull, _selectedCamera);
	setFeature(_selectedCamera, "ExposureMode", "Timed");
	setFeature(_selectedCamera, "ExposureTime", 5000.0);
	setFeature(_selectedCamera, "AcquisitionMode", "Continuous");
	std::cout << "Camera configured.\n";
	



}

void MonoCamProducer::setFeature(CameraPtr _camera, const char* const& _featureString,
								 float _featureValue)
{
	FeaturePtr _feature;
	std::string _camid;
	SP_ACCESS(_camera)->GetFeatureByName(_featureString, _feature);
	SP_ACCESS(_feature)->SetValue(_featureValue);
	SP_ACCESS(_camera)->GetID(_camid);
	std::cout << "Set feature: " << _featureString << " of camera " << _camid << 
		" to a value of " << _featureValue << ".\n";
}

void MonoCamProducer::setFeature(CameraPtr _camera, const char* const& _featureString,
								 const char* const& _featureValue)
{
	FeaturePtr _feature;
	std::string _camid;
	SP_ACCESS(_camera)->GetFeatureByName(_featureString, _feature);
	SP_ACCESS(_feature)->SetValue(_featureValue);
	SP_ACCESS(_camera)->GetID(_camid);
	std::cout << "Set feature: " << _featureString << " of camera " << _camid <<
		" to a value of " << _featureValue << ".\n";
}




void MonoCamProducer::incrementRun()
{
	int dumbCount = 10;
	while (countCompleted== false)
	{
		if (timesRan > dumbCount)
		{
			countCompleted= true;
			std::cout << "count completed.";
			return;
		}
		else {
			timesRan++;
			std::this_thread::sleep_for(
					std::chrono::milliseconds(250));
			std::cout << "Sleeping...\n";
		}
	}
}