#include "common.h"
#include "Manager/Render/GuiMgr.h"

GuiMgr *guiMgr = new GuiMgr();

int main(int, char **)
{
	guiMgr->init();
	std::chrono::duration<double> frameTime(0.0);
	std::chrono::duration<double> sleepAdjust(0.0);

	while (!glfwWindowShouldClose(guiMgr->g_mainWindow))
	{
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		guiMgr->process(frameTime.count());

		// Throttle to cap at 60Hz. This adaptive using a sleep adjustment. This could be improved by
		// using mm_pause or equivalent for the last millisecond.
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> target(1.0 / 60.0);
		std::chrono::duration<double> timeUsed = t2 - t1;
		std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
		if (sleepTime > std::chrono::duration<double>(0))
		{
			std::this_thread::sleep_for(sleepTime);
		}

		std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
		frameTime = t3 - t1;

		// Compute the sleep adjustment using a low pass filter
		sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - frameTime);
	}

	guiMgr->end();
	return 0;
}
