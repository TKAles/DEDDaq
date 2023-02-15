#pragma once
#include <mutex>
#include <queue>
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT;
using namespace AVT::VmbAPI;

class MonoFrameObserver : public IFrameObserver
{

public:
	~MonoFrameObserver();
};

