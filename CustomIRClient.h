#pragma once
#include "IRImagerClient.h"
#include <iostream>
#include <queue>
#include <mutex>
class CustomIRClient : public evo::IRImagerClient
{
public:
	void onRawFrame(unsigned char* data, int size);
	void onThermalFrame(unsigned short* data, unsigned int w, unsigned int h, evo::IRFrameMetadata meta, void* arg);
	void onThermalFrameEvent(unsigned short* thermal, unsigned short* energy, unsigned int w,
		unsigned int h, evo::IRFrameMetadata meta,
		const evo::IRArray<evo::IREventData>& events, void* arg);
	void onVisibleFrame(unsigned char* data, unsigned int w, unsigned int h,
		evo::IRFrameMetadata meta, void* arg);
	void onVisibleFrameEvent(unsigned char* data, unsigned int w, unsigned int h,
		evo::IRFrameMetadata meta, const evo::IRArray<evo::IREventData>& events, void* arg);
	void onFlagStateChange(evo::EnumFlagState flagstate, void* arg);
	void onProcessExit(void* arg);

};