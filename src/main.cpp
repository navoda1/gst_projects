/* 
	Every half an hour..
	1. Grab a frame and save in `frames` directory 
	2. Stitch the frames together into a video
	3. Upload to Google Drive
*/

#include <stdint.h>
#include <iostream>
#include <thread>

#include "common.h"
#include "CSICamera.h"

time_point_t system_start;

class Config {
public:
	duration_seconds_t period = seconds_t(3);
};

int main() {

	status_t status;
	Config config = Config();
	CSICamera csi_camera = CSICamera();

	system_start = std::chrono::system_clock::now();

	std::cout << "System started" << std::endl;

	csi_camera.save_frame(0);

	/* Sleep until the next frame */
	while ((std::chrono::system_clock::now() - system_start) < config.period) {
		std::this_thread::sleep_for(seconds_t(1));
	}

	csi_camera.save_frame(1);

	/* Clean up */
	csi_camera.~CSICamera();
}