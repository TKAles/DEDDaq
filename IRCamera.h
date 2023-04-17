#pragma once
#include <queue>
#include <mutex>
#include "IRImager.h"
#include "IRImagerClient.h"
#include "IRCommonTypes.h"
#include "IRCalibrationManager.h"
#include "IRDeviceParams.h"
#include "IRCamera.h"
#include "IRDevice.h"
#include "IRLogger.h"
#include "ImageBuilder.h"
#include "CustomIRClient.h"

class IRCam
{

public:
	std::string LogFilePath;
	std::string CalibrationPath;
	std::string ConfigXMLPath;
	evo::IRDeviceParams CameraParameters;
	evo::IRDevice* CameraDevice;
	evo::IRImager* CameraImager;
	evo::IRCalibrationManager* cManager;
	CustomIRClient myIRClient;
	int ThermalImageWidth;
	int ThermalImageHeight;
	int FrameBufferSize;
	bool isStreaming;

	std::queue<std::vector<unsigned char>> ThermalImageQueue;
	std::mutex ThermalImageQueueMutex;

	IRCam();
	~IRCam();
	
	int StartupIR();
	int StartIRCamera();
	int StopIRCamera();
	void IRWorker();
};

