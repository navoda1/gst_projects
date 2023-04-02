#ifndef __CSI_CAMERA_H__
#define __CSI_CAMERA_H__

#include <string>
#include <memory>

#include "common.h"

class CSICamera {
public:
	CSICamera();
	~CSICamera();

    /**
     * Capture a frame from the camera and save it in the given directory 
     * as a PNG file
     * 
     * @param   out_dir     Output directory for the PNG file
     * 
     * @returns status_t as success or error
    */
	status_t save_frame(std::string out_dir);

private:
    status_t build_pipeline();

    class GstImplementation;
    std::unique_ptr<GstImplementation> pimpl;
    
    uint32_t frame_idx;
};

#endif /* __CSI_CAMERA_H__ */