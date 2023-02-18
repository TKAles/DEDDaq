#pragma once
#include <string>
#include <vector>
#include "AVCameraObject.h"
#include "VimbaCPP/Include/VimbaSystem.h"

using namespace AVT::VmbAPI;

class DAQRunner
{
public:
	std::vector<std::string> AVCameraIDs;
	std::vector<AVCameraObject> assocAVCameras;
	VimbaSystem& cameraSystem = VimbaSystem::GetInstance();

	int startupAVSystem();
	int findAVCameras();
	int makeAVC(std::string _camID);
	int setupCapture(int _ptsToCap, float _freqToCap);
};

