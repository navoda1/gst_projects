#include <iostream>
#include <gstreamer-1.0/gst/gst.h>

int main(int arg, char *argv[]) {
    
    /* Pipeline */
    GstElement *pipeline = nullptr;

    /* Elements */
    GstElement *csi_cam_source = nullptr;
    GstElement *vid_conv = nullptr;
    GstElement *png_enc = nullptr;
    GstElement *filesink = nullptr;

    GstBus *gst_bus = nullptr;
    GstMessage *gst_msg = nullptr;
    GstStateChangeReturn ret;
    int status;
    bool success;

    /* Initialize gstreamer. 
        Pass in `arg` and `argv` to enable using gstreamer standard command-line options */
    gst_init(&arg, &argv);

    /* Build the pipeline */

    csi_cam_source = gst_element_factory_make("nvarguscamerasrc", "cam_source");
    vid_conv = gst_element_factory_make("nvvidconv", "vid_conv");
    png_enc = gst_element_factory_make("pngenc", "png_enc");
    filesink = gst_element_factory_make("filesink", "file_sink");

    /* Create the empty pipeline */
    pipeline = gst_pipeline_new("SaveFrame");

    if (!csi_cam_source || !vid_conv || !png_enc || !filesink) {
        std::cout << "ERROR: Could not create all pipeline elements" << std::endl;
        return -1;
    }

    /* Add the elements and build */
    gst_bin_add_many(GST_BIN(pipeline), csi_cam_source, vid_conv, png_enc, filesink, NULL);
    success = gst_element_link_many(csi_cam_source, vid_conv, png_enc, filesink, NULL);
    if (!success) {
        std::cout << "ERROR: Elements could not be linked" << std::endl;
        return -1;
    }

    /* Update settings */
    g_object_set(G_OBJECT(png_enc), "snapshot", (gboolean)true, NULL);
    g_object_set(G_OBJECT(filesink), "location", "./test.png", NULL);

    /* Start playing */
    ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cout << "ERROR: Could not set the pipeline state to GST_STATE_PLAYING" << std::endl;
        return -1;
    }

    /* Wait until error of end of stream */
    gst_bus = gst_element_get_bus(pipeline);
    gst_msg = gst_bus_timed_pop_filtered(
        gst_bus, 
        GST_CLOCK_TIME_NONE, 
        static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS)
    );

    /* Parse the message */
    if (gst_msg != NULL) {
        GError *gst_error = nullptr;
        char *debug_info;

        switch (GST_MESSAGE_TYPE(gst_msg)) {
            case GST_MESSAGE_ERROR: 
                gst_message_parse_error (gst_msg, &gst_error, &debug_info);
                std::cout << "ERROR: Error from GST element: " << GST_OBJECT_NAME(gst_msg->src) << " | " <<
                    gst_error->message << std::endl;
                std::cout << "Debug info: " << debug_info << std::endl;
                g_clear_error(&gst_error);
                g_free(debug_info);
                break;
            case GST_MESSAGE_EOS:
                std::cout << "End of stream reached" << std::endl;
                break;
            default:
                std::cout << "ERROR: Unexpected message received" << std::endl;
                break;
        }
        gst_message_unref(gst_msg);
    }

    /* Cleanup */
    gst_object_unref(gst_bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    
    std::cout << "Exiting" << std::endl;
    return 0;
}