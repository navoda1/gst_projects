
CC 		:= g++
CPP 	:= gst_timelapse.cpp
OBJ		:= gst_timelapse
CFLAGS	:= `pkg-config gstreamer-1.0 --cflags`
LIBS 	:= `pkg-config gstreamer-1.0 --libs`

all:
	$(CC) $(CFLAGS) $(CPP) -o $(OBJ) $(LIBS)