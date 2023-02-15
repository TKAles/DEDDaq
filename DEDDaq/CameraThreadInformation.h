#pragma once

#include <string>

class CameraThreadInformation
{
public:
	bool isStreaming;
	std::string cameraID;
	CameraThreadInformation(std::string _id, bool _state)
	{
		isStreaming = _state;
		cameraID = _id;
	}
};