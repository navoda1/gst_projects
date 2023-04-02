# Timelapse Capture with GStreamer
This project was created for me to get familiar with the GStreamer on C++. The `CSICamera.cpp` class instantiates the CSI Camera on an Nvidia Jetson Nano and provides a `save_frame()` function to continuously save image frames in a given location.

The GST pipeline for the image capture is:
```
gst-launch-1.0 nvarguscamerasrc ! nvvidconv flip-method=vertical-flip ! pngenc snapshot=TRUE ! filesink location=frames/frame%d.png
```

A separate script is used to combine the images into a video file:
```
gst-launch-1.0 -e multifilesrc location="frames/frame_%04d.png" caps="image/png,framerate=25/1,width=1920,height=1080" ! pngdec ! videoconvert ! queue ! x264enc ! queue ! mp4mux ! filesink location=video/timelapse.mp4
```