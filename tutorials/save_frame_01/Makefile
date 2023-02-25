
CC 		:= g++
CPP 	:= save_frame.cpp
OBJ		:= save_frame
CFLAGS	:= `pkg-config gstreamer-1.0 --cflags`
LIBS 	:= `pkg-config gstreamer-1.0 --libs`

all:
	$(CC) $(CFLAGS) $(CPP) -o $(OBJ) $(LIBS)