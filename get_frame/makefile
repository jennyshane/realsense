OBJS=get_frame.o matrices.o utils.o
CC=g++
CFLAGS=-std=c++0x -c
LFLAGS=-lGL -lGLEW -lSDL -lrealsense2

get_frame: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -oget_frame

get_frame.o : get_frame.cpp utils.h matrices.h 
	$(CC) $(CFLAGS) get_frame.cpp

utils.o : utils.cpp utils.h 
	$(CC) $(CFLAGS) utils.cpp

matrices.o : matrices.cpp matrices.h 
	$(CC) $(CFLAGS) matrices.cpp
