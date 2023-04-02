#include <iostream>
#include <string>

#include <gstreamer-1.0/gst/gst.h>

#include "CSICamera.h"

#define VERTICAL_FLIP 6

#define MAX_OUT_DIR_LEN     30
#define FNAME_LEN           15 /* "/frame_0000.png" */

struct CSICamera::GstImplementation {
    GstImplementation() :
        pipeline (NULL),
        csi_cam_source (NULL),
        vid_conv (NULL),
        png_enc (NULL),
        filesink (NULL),
        gst_bus (NULL),
        gst_msg (NULL)
    {}

    ~GstImplementation() {}

    /* Pipeline */
    GstElement *pipeline;

    /* Elements */
    GstElement *csi_cam_source;
    GstElement *vid_conv;
    GstElement *png_enc;
    GstElement *filesink;

    GstBus *gst_bus;
    GstMessage *gst_msg;
};


CSICamera::CSICamera() :
    pimpl(new GstImplementation())
{

    status_t status;

    /* Initialize gstreamer */
    gst_init(NULL, NULL);

    status = build_pipeline();

    if (status != success) {
        std::cout << "ERROR: Could not build the pipeline" << std::endl;
    }

    frame_idx = 0;
}

status_t CSICamera::build_pipeline() {
    /* gst-launch-1.0 nvarguscamerasrc ! nvvidconv flip-method=vertical-flip ! pngenc snapshot=TRUE ! filesink location=frames/frame%d.png  */

    bool success;
    
    /* Create the pipeline elements */
    pimpl->csi_cam_source = gst_element_factory_make("nvarguscamerasrc", "cam_source");
    pimpl->vid_conv = gst_element_factory_make("nvvidconv", "vid_conv");
    pimpl->png_enc = gst_element_factory_make("pngenc", "png_enc");
    pimpl->filesink = gst_element_factory_make("filesink", "file_sink");

    /* Create the empty pipeline */
    pimpl->pipeline = gst_pipeline_new("SaveFrame");

    if (!pimpl->csi_cam_source || !pimpl->vid_conv || !pimpl->png_enc || !pimpl->filesink) {
        std::cout << "ERROR: Could not create all pipeline elements" << std::endl;
        return error;
    }

     /* Add the elements and build */
    gst_bin_add_many(GST_BIN(pimpl->pipeline), pimpl->csi_cam_source, pimpl->vid_conv, pimpl->png_enc, pimpl->filesink, NULL);
    success = gst_element_link_many(pimpl->csi_cam_source, pimpl->vid_conv, pimpl->png_enc, pimpl->filesink, NULL);
    if (!success) {
        std::cout << "ERROR: Elements could not be linked" << std::endl;
        return error;
    }

    /* Update settings */
    g_object_set(G_OBJECT(pimpl->png_enc), "snapshot", (gboolean)true, NULL);
    g_object_set(G_OBJECT(pimpl->vid_conv), "flip-method", VERTICAL_FLIP, NULL);
}

CSICamera::~CSICamera() {
    /* Cleanup */
    if (pimpl->gst_bus != NULL) {
        gst_object_unref(pimpl->gst_bus);
    }

    if (pimpl->pipeline != NULL) {
        gst_element_set_state(pimpl->pipeline, GST_STATE_NULL);
        gst_object_unref(pimpl->pipeline);
    }
}

status_t CSICamera::save_frame(std::string out_dir) {

    GstStateChangeReturn ret;
    char fname[MAX_OUT_DIR_LEN + FNAME_LEN];
    
    std::cout << "*************** Capturing Frame: " << frame_idx << " ***************" <<std::endl;

    /* Construct the output file name */
    if (out_dir.length() > 15) {
        std::cout << "Error: Output directory name too long. Max allowed: " << MAX_OUT_DIR_LEN << std::endl;
        return error;
    }
    memcpy(fname, out_dir.c_str(), out_dir.length());
    sprintf(fname + out_dir.length(), "/frame_%04d.png", frame_idx++);

    /* Update filesink location */
    g_object_set(G_OBJECT(pimpl->filesink), "location", fname, NULL);

    /* Run the pipeline */   
    ret = gst_element_set_state(pimpl->pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cout << "ERROR: Could not set the pipeline state to GST_STATE_PLAYING" << std::endl;
        return error;
    }

    /* Wait until error of end of stream */
    pimpl->gst_bus = gst_element_get_bus(pimpl->pipeline);
    pimpl->gst_msg = gst_bus_timed_pop_filtered(
        pimpl->gst_bus, 
        GST_CLOCK_TIME_NONE, 
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    /* Parse the message */
    if (pimpl->gst_msg != NULL) {
        GError *gst_error = nullptr;
        char *debug_info;

        switch (GST_MESSAGE_TYPE(pimpl->gst_msg)) {
            case GST_MESSAGE_ERROR: {
                gst_message_parse_error (pimpl->gst_msg, &gst_error, &debug_info);
                std::cout << "ERROR: Error from GST element: " << GST_OBJECT_NAME(pimpl->gst_msg->src) << " | " << gst_error->message << std::endl;
                std::cout << "Debug info: " << debug_info << std::endl;
                g_clear_error(&gst_error);
                g_free(debug_info);
            }
            break;
            case GST_MESSAGE_EOS: {
                std::cout << "End of stream reached" << std::endl;
            }
            break;
            default: {
                std::cout << "ERROR: Unexpected message received" << std::endl;
            }
            break;
        }
        gst_message_unref(pimpl->gst_msg);
    }

    /* Reset the pipeline */
    gst_element_set_state(pimpl->pipeline, GST_STATE_NULL);

}