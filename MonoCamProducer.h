#pragma once
#include <mutex>
#include <chrono>
#include <thread>
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT::VmbAPI;
using namespace AVT;

class MonoCamProducer
{
public:
	MonoCamProducer();
	void incrementRun();
	void activateCamera(std::string _cameraID);

	void setFeature(CameraPtr _camera, const char* const& _featureString,
		float _featureValue);
	void setFeature(CameraPtr _camera, const char* const& _featureString,
		const char* const& _featureValue);
	
	VmbErrorType startupCamera();
	VmbErrorType shutdownCamera();
	
	void registerCameras();

private:

	CameraPtrVector foundCameras;
	VimbaSystem& monocamSystem;
	std::vector<std::string> foundCameraNames;
	std::vector<std::string> foundCameraIDs;
	std::vector<bool> cameraAcquisitionActive;
	int times_ran;
	bool countCompleted;
	
};