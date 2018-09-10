#ifndef MATRICES_H
#define MATRICES_H
#include <GL/glu.h>
#include <cmath>

const double pi=std::acos(-1);

inline GLfloat deg2rad(GLfloat deg){
	return (pi/180.0)*deg;
}

inline GLfloat rad2deg(GLfloat rad){
	return (180.0/pi)*rad;
}


class HomogMatrix{
	private:
		GLfloat matData[16];
		static HomogMatrix mat_mult(HomogMatrix MatA, HomogMatrix MatB);
		HomogMatrix(GLfloat initData[16]); //only called by this class
		class MatFactory{
			public:
				static HomogMatrix rotX(GLfloat theta);
				static HomogMatrix rotY(GLfloat theta);
				static HomogMatrix rotZ(GLfloat theta);
				static HomogMatrix trans(GLfloat x, GLfloat y, GLfloat z);
				static HomogMatrix sc(GLfloat x, GLfloat y, GLfloat z);
		};

	public:
		HomogMatrix(); //returns identity HomogMatrix
		HomogMatrix rotate_x(GLfloat theta);
		HomogMatrix rotate_y(GLfloat theta);
		HomogMatrix rotate_z(GLfloat theta);
		HomogMatrix translate(GLfloat x, GLfloat y, GLfloat z);
		HomogMatrix scale(GLfloat x, GLfloat y, GLfloat z);
		HomogMatrix view_frustum(GLfloat angle_of_view, GLfloat aspect_ratio, GLfloat z_near, GLfloat z_far);
		const GLfloat* getData() const ;
		void copy(HomogMatrix from);
};
#endif
