# Various Projects Tutorials using GStreamer

## Pipelines

### Capture a frame every second and write to separate files
```
gst-launch-1.0 nvarguscamerasrc ! queue ! nvvidconv flip-method=vertical-flip ! videorate ! video/x-raw,framerate=1/1 ! jpegenc ! multifilesink location="frames/frame%06d.jpg"
```


## Tutorials

### `tutorials/save_frame_01`
Simple pipeline to take a screenshot. Using `gst_parse_launch()` to build the pipeline from a string

### `tutorials/save_frame_02`
Simple pipeline to take a screenshot. Using gst API functions to build the pipeline.