#pragma once
#include <string>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Feature.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/VimbaSystem.h"

using namespace AVT;
using namespace AVT::VmbAPI;

class AVCameraObject
{
public:
	AVCameraObject(std::string _camID);
	std::string cameraID;
	bool isConnected;
	bool isStreaming;
	int connect(VimbaSystem& _camSys);
	int disconnect();

	int configureFeature(std::string _featureName,
		float _featureValue, VimbaSystem& _camSys);
	int configureFeature(std::string _featureName,
		std::string _featureValue, VimbaSystem& _camSys);
	
	int configureCameras();
	void startStream();
private:
	CameraPtr _associatedCamera;
	FeaturePtr _associatedFeature;
};

