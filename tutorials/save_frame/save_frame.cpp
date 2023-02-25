#include <iostream>
#include <gstreamer-1.0/gst/gst.h>

int main(int arg, char *argv[]) {
    
    GstElement *pipeline = nullptr;
    GstBus *bus = nullptr;
    GstMessage *gst_msg = nullptr;
    GError *gst_error = nullptr;

    /* Initialize gstreamer. 
        Pass in `arg` and `argv` to enable using gstreamer standard command-line options */
    gst_init(&arg, &argv);

    /* Build the pipeline */
    pipeline = gst_parse_launch(
        "gst-launch-1.0 nvarguscamerasrc ! nvvidconv flip-method=vertical-flip ! pngenc snapshot=TRUE ! filesink location=test1.png",
        &gst_error
    );

    if (!pipeline) {
        std::cout << "ERROR: Could not build the pipeline" << std::endl;
        return -1;
    }

    /* Start playing */
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    /* Wait until error of end of stream */
    bus = gst_element_get_bus(pipeline);
    gst_msg = gst_bus_timed_pop_filtered(
        bus, 
        GST_CLOCK_TIME_NONE, 
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    /* Cleanup */
    if (gst_msg != nullptr) {
        gst_message_unref(gst_msg);
    }
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    std::cout << "Stream ended. Exiting" << std::endl;
    return 0;
}