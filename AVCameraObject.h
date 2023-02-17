#pragma once
#include <string>

class AVCameraObject
{
public:
	std::string cameraID;
	bool isConnected;
	bool isStreaming;
	int connect();
	int disconnect();
	void startStream();
};

