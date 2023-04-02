/* 
	Every half an hour..
	1. Grab a frame and save in `frames` directory 
	2. Stitch the frames together into a video
	3. Upload to Google Drive
*/

#include <stdint.h>
#include <signal.h>
#include <iostream>
#include <thread>

#include "common.h"
#include "CSICamera.h"

CSICamera *csi_camera;

class Config {
public:
	// duration_minutes_t period = minutes_t(15);
	duration_seconds_t period = seconds_t(1);
};

void signal_callback_handler(int signum) {
	std::cout << "Caught signal: " << signum << "\n";
	csi_camera->~CSICamera();
	std::cout << "Cleaned up. Exiting...\n";
	exit(0);
}


int main() {

	status_t status;
	time_point_t t_last_frame;
	Config config = Config();
	csi_camera = new CSICamera();

	std::cout << "System started" << std::endl;

	signal(SIGINT, signal_callback_handler);

	while (true) {
		csi_camera->save_frame("./frames");
		t_last_frame = std::chrono::system_clock::now();

		std::cout << "Waiting for the next frame..." << std::endl << std::endl;

		/* Sleep until the next frame */
		while ((std::chrono::system_clock::now() - t_last_frame) < config.period) {
			std::this_thread::sleep_for(seconds_t(2));
		}
	}

	/* Clean up */
	csi_camera->~CSICamera();
}