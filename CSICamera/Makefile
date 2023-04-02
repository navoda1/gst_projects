
CC 				:= g++

PWD 			:= $(shell pwd)
INCLUDE_DIRS 	:= -I../include -I./include 

# GST
GST_CFLAGS		:= `pkg-config gstreamer-1.0 --cflags`
GST_LIBS 		:= `pkg-config gstreamer-1.0 --libs`

# Lib build
LIB_CPP 		:= src/CSICamera.cpp
LIB_OBJ 		:= build/CSICamera.o
LIB_OUT 		:= lib/libCSICamera.so

# Test build
TEST_CPP 		:= test/TestCSICamera.cpp
TEST_OBJ		:= capture_test
TEST_INCLUDES	:= $(INCLUDE_DIRS) -Isrc


all:
	$(CC) $(GST_CFLAGS) $(INCLUDE_DIRS) $(LIB_CPP) -c -o $(LIB_OBJ) $(GST_LIBS)
	$(CC) -shared -o $(LIB_OUT) $(LIB_OBJ)
	
capture-test:
	$(CC) $(TEST_INCLUDES) $(TEST_CPP) -o $(TEST_OBJ) -L./lib/ -lCSICamera $(GST_LIBS)

run-capture-test:
	LD_LIBRARY_PATH=./lib ./capture_test