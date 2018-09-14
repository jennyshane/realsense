OBJS=get_frame.o 
CC=g++
CFLAGS=-std=c++0x -c -I/home/jenny/prac/opengl/include
LFLAGS=-L/home/jenny/prac/opengl/lib -lGL -lGLEW -lSDL -lrealsense2 -lGLutils -lmatrices

get_frame: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -oget_frame

get_frame.o : get_frame.cpp 
	$(CC) $(CFLAGS) get_frame.cpp

