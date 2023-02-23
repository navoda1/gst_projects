#include <iostream>
#include <gstreamer-1.0/gst/gst.h>

int main(int arg, char *argv[]) {
    
    GstElement *gst_pipeline = nullptr;
    GstElement *gst_csi_cam_source = nullptr;
    GstElement *gst_vid_conv = nullptr;
    GstElement *gst_png_enc = nullptr;
    GstElement *gst_filesink = nullptr;
    GstBus *gst_bus = nullptr;
    GstMessage *gst_msg = nullptr;
    GError *gst_error = nullptr;

    /* Initialize gstreamer. 
        Pass in `arg` and `argv` to enable using gstreamer standard command-line options */
    gst_init(&arg, &argv);

    /* Create the elements */
    gst_csi_cam_source = gst_element_factory_make("nvarguscamerasrc", "source");
    gst_vid_conv = gst_element_factory_make("nvvidconv", "convert");
    gst_png_enc = gst_element_factory_make("pngenc", "convert");
    gst_filesink = gst_element_factory_make("filesink", "sink");

    /* Build the pipeline */
    gst_pipeline = gst_parse_launch(
        "gst-launch-1.0 nvarguscamerasrc ! nvvidconv flip-method=vertical-flip ! pngenc snapshot=TRUE ! filesink location=test1.png",
        &gst_error
    );

    if (!gst_pipeline) {
        std::cout << "ERROR: Could not build the pipeline" << std::endl;
        return -1;
    }

    /* Start playing */
    gst_element_set_state(gst_pipeline, GST_STATE_PLAYING);

    /* Wait until error of end of stream */
    gst_bus = gst_element_get_bus(gst_pipeline);
    gst_msg = gst_bus_timed_pop_filtered(
        gst_bus, 
        GST_CLOCK_TIME_NONE, 
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    /* Cleanup */
    if (gst_msg != nullptr) {
        gst_message_unref(gst_msg);
    }
    gst_object_unref(gst_bus);
    gst_element_set_state(gst_pipeline, GST_STATE_NULL);
    gst_object_unref(gst_pipeline);

    std::cout << "Stream ended. Exiting" << std::endl;
    return 0;
}