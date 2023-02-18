#pragma once
#include <string>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Feature.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/VimbaSystem.h"

class AVCameraObject
{
public:
	std::string cameraID;
	bool isConnected;
	bool isStreaming;
	AVCameraObject(std::string _camID);
	int connect();
	int disconnect();
	void startStream();
};

