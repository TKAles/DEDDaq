#include <chrono>
#include <iostream>
#include <thread>
#include "MonoCamProducer.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/VimbaCPPCommon.h"
#include "VimbaCPP/Include/VimbaCPP.h"

int times_ran = 0;
bool count_complete;
std::string camera_name;

using namespace std::chrono_literals;
using namespace AVT::VmbAPI;

void MonoCamProducer()
{
	count_complete = false;
	times_ran = 0;
}
void MonoCamProducer::increment_run()
{
	int dumb_count = 10;
	while (count_complete == false)
	{
		if (times_ran > dumb_count)
		{
			count_complete = true;
			std::cout << "count completed.";
			return;
		}
		else {
			times_ran++;
			std::this_thread::sleep_for(
					std::chrono::milliseconds(250));
			std::cout << "Sleeping...\n";
		}
	}
}