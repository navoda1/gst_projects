
CC 				:= g++
CPP 			:= main.cpp CSICamera.cpp
OBJ				:= run_timelapse
INCLUDE_DIRS	:= .
CFLAGS			:= `pkg-config gstreamer-1.0 --cflags`
LIBS 			:= `pkg-config gstreamer-1.0 --libs`

all:
	$(CC) $(CFLAGS) -I$(INCLUDE_DIRS)  -o $(OBJ) $(CPP) $(LIBS)