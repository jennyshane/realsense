#ifndef UTILS_H
#define UTILS_H
#include <GL/glu.h>
#include <string>
#include <cmath>

//struct used to store points in vertex array buffers

struct pointxy{
	float x, y;
	pointxy(float X, float Y): x(X), y(Y) {}
};
struct glPointStr{
	GLfloat x, y, z, w;
	GLfloat r, g, b, a;
	glPointStr(float X, float Y, float R, float G, float B): x(X), y(Y), r(R), g(G), b(B)
	{
		z=0.0f;
		w=1.0f;
		a=1.0f;
	}

	glPointStr(float X, float Y, float Z, float R, float G, float B): x(X), y(Y), z(Z), r(R), g(G), b(B)
	{
		w=1.0f;
		a=1.0f;
	}

	glPointStr(){ //default constructor
		x=0.0f;
		y=0.0f;
		z=0.0f;
		w=1.0f;
		r=0.0f;
		g=0.0f;
		b=0.0f;
		a=1.0f;
	}

};
std::string readFile(std::string filename);
GLuint make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size);
void show_log_info(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
GLuint make_shader(GLenum type, std::string filename);
GLuint make_program(GLuint vertex_shader, GLuint fragment_shader);
bool initSDL(int screenSize);

#endif
