#pragma once
#include "VimbaCPP/Include/VimbaCPP.h"
#include "AVCameraConfiguration.h"
#include "AVFrameObserver.h"
#include "AVFrameConsumer.h"
#include <mutex>
#include <condition_variable>

/*
	AVMonoCamera
	Thomas Ales | Iowa State Univ.
	Feb 2023
	---
	Object that represents an Allied Vision Aluvium
	camera that is connected to the system. Unique by
	CameraID.
*/

using namespace AVT::VmbAPI;
using namespace AVT;

class AVMonoCamera
{
public:
	std::string cameraID;
	std::string RunPrefix;
	std::string OutputFilename;

	bool isStreaming;

	CameraPtr monoCameraPtr;
	FeaturePtr cameraFeaturePtr;
	FramePtrVector cameraFrameBufferVector = FramePtrVector(5);
	VmbInt64_t cameraPayloadSize;
	std::mutex streamMutex;
	std::condition_variable streamStopCV;

	int changeFeature(std::string fName, std::string fValue);
	int changeFeature(std::string fName, double fValue);
	int changeFeature(std::string fName, bool fValue);
	int changeFeature(std::string fName, int fValue);

	int applyFeatureChange();

	AVMonoCamera();
	AVMonoCamera(std::string _camID, VimbaSystem& _cSys, std::string _outPrefix);

	void streamWorker();

	AVCameraConfiguration associatedConfig;
	std::mutex streamQueueMutex;
	std::queue<std::vector<VmbUchar_t>> ImageQueue;
	std::queue<std::tuple<VmbUint64_t, VmbUint64_t,
		VmbUint32_t, VmbUint32_t>> MetadataQueue;
	AVFrameConsumer ImageConsumer = AVFrameConsumer(ImageQueue, 
													streamQueueMutex, MetadataQueue);
};
