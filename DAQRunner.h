#pragma once
#include <string>
#include <vector>
#include "AVCameraObject.h"

class DAQRunner
{
public:
	std::vector<std::string> AVCameraIDs;
	std::vector<AVCameraObject> assocAVCameras;
	int findAVCameras();
	int makeAVC(std::string _camID);
	int setupCapture(int _ptsToCap, float _freqToCap);
};

