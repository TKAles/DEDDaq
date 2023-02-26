#pragma once
#include "VimbaCPP/Include/IFrameObserver.h"

using namespace AVT;
using namespace AVT::VmbAPI;

class AVFrameObserver : public IFrameObserver
{
public:
	AVFrameObserver(CameraPtr pCamera) : IFrameObserver(pCamera) {};
	void FrameReceived(const FramePtr pFrame);
};