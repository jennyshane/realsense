#include <GL/glew.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <cassert>
#include <iostream>
#include <librealsense2/rs.hpp>
#include "matrices.h"
#include "utils.h"

struct rspoint{
	GLfloat x, y, z, w;
	GLfloat s, t;
};

struct app_state{
	GLfloat yaw, pitch;
	GLfloat last_x, last_y;
	bool click;
	app_state(): yaw(15.0), pitch(15.0), last_x(0.0), last_y(0.0), click(false){}
};
int screensize=800;

int main(void){

	app_state state;
	if(initSDL(screensize)==false){
		return 1;
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//setup realsense camera
	rs2::context ctx;
	auto devices=ctx.query_devices(); //get available devices
	std::cout<<"There are "<<devices.size()<<" devices connected."<<std::endl; //print number of devices
	for (auto dev: devices){
		std::cout<<dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER)<<std::endl; //print serial number of each
	}
	rs2::device dev;
	if (devices.size()>0){ 
		dev=devices.front(); //get first device
	}else{
		return 1;
	}

	//setup realsense pipeline 
	rs2::pipeline p;
	rs2::config c;
	c.enable_device(dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));
	rs2::pipeline_profile profile=p.start(c);

	rs2::pointcloud pc;
	rs2::points points;

	//NOTE: We need to do this procedure at least twice or the texture coordinates will be zero
	auto frames=p.wait_for_frames();
	auto depth=frames.get_depth_frame();
	points=pc.calculate(depth);
	auto color=frames.get_color_frame(); 
	pc.map_to(color);
	
	//create shaders
	GLuint v_shader, f_shader;
	GLuint program;
	v_shader=make_shader(GL_VERTEX_SHADER, "tex.v.glsl"); //generate vertex shader from text in .glsl file
	assert(v_shader!=0);
	f_shader=make_shader(GL_FRAGMENT_SHADER, "tex.f.glsl"); //generate fragment shader from text in .glsl file
	assert(f_shader!=0);
	program=make_program(v_shader, f_shader); //compile and link shaders 
	assert(program!=0);
	//find and set locations of variables in shader program
	GLint position, texcoord;
	GLint perspective_mat, modelview_mat, object_mat;
	//varying input attributes -- "per-vertex"
	position=glGetAttribLocation(program, "position");
	texcoord=glGetAttribLocation(program, "texcoord");
	//uniform variables-- constant per render call
	perspective_mat=glGetUniformLocation(program, "perspective_mat");
	modelview_mat=glGetUniformLocation(program, "modelview_mat");
	object_mat=glGetUniformLocation(program, "object_mat");

	//Create matrices
	HomogMatrix p_matrix=HomogMatrix().view_frustum(deg2rad(60), 1.0, 0.01, 10);
	HomogMatrix m_matrix=HomogMatrix().rotate_y(deg2rad(180)).rotate_z(deg2rad(180));
	HomogMatrix o_matrix=HomogMatrix().rotate_x(deg2rad(state.pitch)).rotate_y(deg2rad(state.yaw));
	
	//get info about the color frame
	auto format=color.get_profile().format();
	int width=color.get_width();
	int height=color.get_height();

	//create texture from rs video frame
	glEnable(GL_TEXTURE_2D);
	GLint tex_loc;
	tex_loc=glGetUniformLocation(program, "texmap");
	GLuint tex=0;
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex); 
	std::cout<<"RGB format"<<std::endl;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	//get vertex data (3d point and texture coord) and put into vbo
	glPointSize(2);

	GLuint vao;
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//create vao and setup vertex attrib data
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(position);
	glEnableVertexAttribArray(texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, sizeof(rspoint), (void*)0);
	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(rspoint), (void*)(offsetof(rspoint, s)));
	

	int n_vertices=points.size();
	rspoint* point_array=(rspoint*)malloc(n_vertices*sizeof(rspoint));
	//render
	SDL_Event event;
	bool quit=false;
	while(false==quit){
		while(SDL_PollEvent(&event)){
			if(SDL_QUIT==event.type){
				quit=true;
			}else if(SDL_MOUSEMOTION==event.type){
				if(state.click){
					state.yaw-=(state.last_x-event.motion.x);
					state.yaw=(state.yaw<-120)?-120:state.yaw;
					state.yaw=(state.yaw>120)?120:state.yaw;
					state.pitch-=(state.last_y-event.motion.y);
					state.pitch=(state.pitch<-120)?-120:state.pitch;
					state.pitch=(state.pitch>120)?120:state.pitch;
					o_matrix=HomogMatrix().rotate_x(deg2rad(state.pitch)).rotate_y(deg2rad(state.yaw));
				}
				state.last_x=event.motion.x;
				state.last_y=event.motion.y;
			}else if(SDL_MOUSEBUTTONDOWN==event.type and event.button.button==SDL_BUTTON_LEFT){
				state.click=true;
			}else if(SDL_MOUSEBUTTONUP==event.type and event.button.button==SDL_BUTTON_LEFT){
				state.click=false;
			}
		}

		frames=p.wait_for_frames();
		depth=frames.get_depth_frame();
		points=pc.calculate(depth);
		color=frames.get_color_frame(); 
		pc.map_to(color);

		n_vertices=points.size();
		const rs2::vertex *vertices=points.get_vertices();
		const rs2::texture_coordinate *texture_coords=points.get_texture_coordinates();
		int npoints=0;
		for(int i=0; i<n_vertices; ++i){
			if (vertices[i].z){
				point_array[npoints].x=vertices[i].x;
				point_array[npoints].y=vertices[i].y;
				point_array[npoints].z=vertices[i].z;
				point_array[npoints].w=1.0f;
				point_array[npoints].s=texture_coords[i].u;
				point_array[npoints].t=texture_coords[i].v;
				npoints++;
			}
		}

		glBufferData(GL_ARRAY_BUFFER, npoints*sizeof(rspoint), point_array, GL_DYNAMIC_DRAW); //note we only copy first npoints elements 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, color.get_data());
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);
		glUniformMatrix4fv(perspective_mat, 1, GL_FALSE, p_matrix.getData());
		glUniformMatrix4fv(modelview_mat, 1, GL_FALSE, m_matrix.getData());
		glUniformMatrix4fv(object_mat, 1, GL_FALSE, o_matrix.getData());
		glUniform1i(tex_loc, 0);// because we called glActiveTexture(GL_TEXTURE0)
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, npoints);
		SDL_GL_SwapBuffers();
	}
	SDL_Quit();


	return 0;
}
