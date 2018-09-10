#include <GL/glew.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include "utils.h"
std::string readFile(std::string filename){

	std::ifstream file(filename, std::ios::in);
	std::string content((std::istreambuf_iterator<char>(file) ),
			(std::istreambuf_iterator<char>() ));

	file.close();
	return content;
}

GLuint make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size){
	GLuint buffer; 
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
	return buffer;
}

void show_log_info(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog){
	GLint log_length;
	char *log;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log=(char*)malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	std::cout<<log<<std::endl;
	free(log);
}

GLuint make_shader(GLenum type, std::string filename){
	std::string filesource=readFile(filename);
	GLchar *source=(GLchar*) filesource.c_str();
	GLint length=filesource.length();
	GLuint shader;
	GLint shader_ok;
	if(!source){
		return 0;
	}
	shader=glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if(!shader_ok){
		std::cout<<"Failed to compile: "<<filename<<std::endl;
		show_log_info(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;

}

GLuint make_program(GLuint vertex_shader, GLuint fragment_shader){
	GLint program_ok;

	GLuint program=glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if(!program_ok){
		std::cout<<"Failed to link shader program: "<<std::endl;
		show_log_info(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}



bool initSDL(int screenSize){
	const SDL_VideoInfo* info=NULL;
	if(-1==SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout<<"Couldn't Initialize SDL"<<std::endl;
		return false;
	}
	info=SDL_GetVideoInfo();
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if(NULL==SDL_SetVideoMode(screenSize, screenSize, info->vfmt->BitsPerPixel, SDL_OPENGL)){
		std::cout<<"Failed to set SDL VideoMode"<<std::endl;
		return false;
	}
	glewInit();
	SDL_WM_SetCaption("OpenGL Window", NULL);
	SDL_EnableUNICODE(1);
	return true;
}


