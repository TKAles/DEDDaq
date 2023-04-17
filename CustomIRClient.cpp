
#include "CustomIRClient.h"

void CustomIRClient::onRawFrame(unsigned char* data, int size)
{
	std::cout << "onRawFrame called!" << std::endl;
	return;
}

void CustomIRClient::onThermalFrame(unsigned short* data, unsigned int w, 
	unsigned int h, evo::IRFrameMetadata meta, void* arg)
{
	std::cout << "onThermalFrame callback has been triggered." << std::endl;
	return;
}

void CustomIRClient::onThermalFrameEvent(unsigned short* thermal, unsigned short* energy, 
	unsigned int w, unsigned int h, evo::IRFrameMetadata meta, 
	const evo::IRArray<evo::IREventData>& events, void* arg)
{
	std::cout << "onThermalFrameEvent callback has been triggered." << std::endl;
	return;
}

void CustomIRClient::onVisibleFrame(unsigned char* data, unsigned int w, unsigned int h, 
	evo::IRFrameMetadata meta, void* arg)
{
	return;
}

void CustomIRClient::onVisibleFrameEvent(unsigned char* data, unsigned int w, unsigned int h, 
	evo::IRFrameMetadata meta, const evo::IRArray<evo::IREventData>& events, void* arg)
{
	return;
}

void CustomIRClient::onFlagStateChange(evo::EnumFlagState flagstate, void* arg)
{
	std::cout << "Flag state has changed!" << std::endl;
	return;
}

void CustomIRClient::onProcessExit(void* arg)
{
	return;
}
