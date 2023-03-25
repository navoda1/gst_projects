#ifndef __CSI_CAMERA_H__
#define __CSI_CAMERA_H__

#include <gstreamer-1.0/gst/gst.h>

#include "common.h"

class CSICamera {
public:
	CSICamera();
	~CSICamera();

	status_t save_frame(int i);

private:
    status_t build_pipeline();

    /* Pipeline */
    GstElement *pipeline;

    /* Elements */
    GstElement *csi_cam_source;
    GstElement *vid_conv;
    GstElement *png_enc;
    GstElement *filesink;

    GstBus *gst_bus;
    GstMessage *gst_msg;

    uint32_t frame_idx;
};

#endif /* __CSI_CAMERA_H__ */