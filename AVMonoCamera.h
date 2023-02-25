#pragma once
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/VimbaCPP.h"
#include <mutex>
#include <condition_variable>
using namespace AVT::VmbAPI;
using namespace AVT;

class AVMonoCamera
{
public:
	std::string cameraID;

	bool isStreaming;

	CameraPtr monoCameraPtr;
	FeaturePtr cameraFeaturePtr;
	
	std::mutex streamMutex;
	std::condition_variable streamStopCV;

	int changeFeature(std::string fName, std::string fValue);
	int changeFeature(std::string fName, double fValue);
	int changeFeature(std::string fName, bool fValue);
	int changeFeature(std::string fName, int fValue);

	AVMonoCamera();
	AVMonoCamera(std::string _camID, VimbaSystem& _cSys);

	void streamWorker(std::mutex& _lockMutex);

};

