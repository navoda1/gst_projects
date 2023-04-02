
CC 				:= g++
CPP 			:= src/Timelapse.cpp
OBJ				:= bin/timelapse
INCLUDE_DIRS	:= -I../include -I../CSICamera/include
CFLAGS			:= `pkg-config gstreamer-1.0 --cflags`
LIBS 			:= -L../CSICamera/lib/ -lCSICamera `pkg-config gstreamer-1.0 --libs`

# run:
# 	bin/timelapse

all:
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $(OBJ) $(CPP) $(LIBS)
