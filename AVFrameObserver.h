#pragma once
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaCPP/Include/Camera.h"

using namespace AVT::VmbAPI;

class AVFrameObserver : IFrameObserver
{
public:
	AVFrameObserver(CameraPtr pCamera);
	void FrameReceived(const FramePtr pFrame);
	~AVFrameObserver();
	int framesSeen;

};

