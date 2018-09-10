#include <GL/glu.h>
#include <cmath>
#include "matrices.h"

HomogMatrix::HomogMatrix(){
	HomogMatrix tmp=MatFactory::sc(1.0, 1.0, 1.0); //identity matrix
	const GLfloat* vals=tmp.getData();
	for(int i=0; i<16; ++i){
		matData[i]=vals[i];
	}	
}

HomogMatrix::HomogMatrix(GLfloat* data){
	for(int i=0; i<16; ++i){
		matData[i]=data[i];
	}
}

const GLfloat* HomogMatrix::getData() const {
	return matData;
}

HomogMatrix HomogMatrix::rotate_x(GLfloat theta){
	return mat_mult(MatFactory::rotX(theta), *this);
}

HomogMatrix HomogMatrix::rotate_y(GLfloat theta){
	return mat_mult(MatFactory::rotY(theta), *this);
}

HomogMatrix HomogMatrix::rotate_z(GLfloat theta){
	return mat_mult(MatFactory::rotZ(theta), *this);
}

HomogMatrix HomogMatrix::scale(GLfloat x, GLfloat y, GLfloat z){
	return mat_mult(MatFactory::sc(x, y, z), *this);
}

HomogMatrix HomogMatrix::translate(GLfloat x, GLfloat y, GLfloat z){
	return mat_mult(MatFactory::trans(x, y, z), *this);
}

HomogMatrix HomogMatrix::view_frustum(GLfloat angle_of_view, GLfloat aspect_ratio, GLfloat z_near, GLfloat z_far){
	GLfloat matrix[16];

	matrix[0]=1.0f/tan(angle_of_view); matrix[1]=0.0f; matrix[2]=0.0f; matrix[3]=0.0f; 
	matrix[4]=0.0f; matrix[5]=aspect_ratio/tan(angle_of_view); matrix[6]=0.0f; matrix[7]=0.0f;
	matrix[8]=0.0f; matrix[9]=0.0f; matrix[10]=(z_far+z_near)/(z_far-z_near); matrix[11]=1.0f;   
	matrix[12]=0.0f; matrix[13]=0.0f; matrix[14]=-2.0f*(z_far*z_near)/(z_far-z_near); matrix[15]=0.0f;   

	return HomogMatrix(matrix);
}

HomogMatrix HomogMatrix::mat_mult(HomogMatrix MatA, HomogMatrix MatB){
	GLfloat matRes[16]; //all matrices 4x4
	for(int i=0; i<16; ++i){
		matRes[i]=0;
	}
	for(int n=0; n<16; n++){
		int row=n%4;
		int col=n/4;
		for(int t=0; t<4; ++t){
			matRes[n]+=MatA.matData[t*4+row]*MatB.matData[4*col+t];
		}
	}
	return HomogMatrix(matRes);
}

HomogMatrix HomogMatrix::MatFactory::rotX(GLfloat theta){
	GLfloat matrix[16];

	matrix[0]=1.0f; matrix[1]=0.0f; matrix[2]=0.0f; matrix[3]=0.0f;
	matrix[4]=0.0f; matrix[5]=cos(theta); matrix[6]=sin(theta); matrix[7]=0.0f;
	matrix[8]=0.0f; matrix[9]=-sin(theta); matrix[10]=cos(theta); matrix[11]=0.0f;
	matrix[12]=0.0f; matrix[13]=0.0f; matrix[14]=0.0f; matrix[15]=1.0f;

	return HomogMatrix(matrix);

}

HomogMatrix HomogMatrix::MatFactory::rotY(GLfloat theta){
	GLfloat matrix[16];

	matrix[0]=cos(theta); matrix[1]=0.0f; matrix[2]=-sin(theta); matrix[3]=0.0f;
	matrix[4]=0.0f; matrix[5]=1.0f; matrix[6]=0.0f; matrix[7]=0.0f;
	matrix[8]=sin(theta); matrix[9]=0.0f; matrix[10]=cos(theta); matrix[11]=0.0f;
	matrix[12]=0.0f; matrix[13]=0.0f; matrix[14]=0.0f; matrix[15]=1.0f;

	return HomogMatrix(matrix);
}

HomogMatrix HomogMatrix::MatFactory::rotZ(GLfloat theta){
	GLfloat matrix[16];

	matrix[0]=cos(theta); matrix[1]=sin(theta); matrix[2]=0.0f; matrix[3]=0.0f;
	matrix[4]=-sin(theta); matrix[5]=cos(theta); matrix[6]=0.0f; matrix[7]=0.0f;
	matrix[8]=0.0f; matrix[9]=0.0f; matrix[10]=1.0f; matrix[11]=0.0f;
	matrix[12]=0.0f; matrix[13]=0.0f; matrix[14]=0.0f; matrix[15]=1.0f;
	
	return HomogMatrix(matrix);
}


HomogMatrix HomogMatrix::MatFactory::sc(GLfloat x, GLfloat y, GLfloat z){
	GLfloat matrix[16];

	matrix[0]=x; matrix[1]=0.0f; matrix[2]=0.0f; matrix[3]=0.0f;
	matrix[4]=0.0f; matrix[5]=y; matrix[6]=0.0f; matrix[7]=0.0f;
	matrix[8]=0.0f; matrix[9]=0.0f; matrix[10]=z; matrix[11]=0.0f;
	matrix[12]=0.0f; matrix[13]=0.0f; matrix[14]=0.0f; matrix[15]=1.0f;

	return HomogMatrix(matrix);
}

HomogMatrix HomogMatrix::MatFactory::trans(GLfloat x, GLfloat y, GLfloat z){
	GLfloat matrix[16];

	matrix[0]=1.0f; matrix[1]=0.0f;  matrix[2]=0.0f;  matrix[3]=0.0f; 
	matrix[4]=0.0f;  matrix[5]=1.0f;  matrix[6]=0.0f;  matrix[7]=0.0f; 
	matrix[8]=0.0f;  matrix[9]=0.0f;  matrix[10]=1.0f;  matrix[11]=0.0f; 
	matrix[12]=x;  matrix[13]=y;  matrix[14]=z;  matrix[15]=1.0f; 
	 
	return HomogMatrix(matrix);
}

void HomogMatrix::copy(HomogMatrix from){
	const GLfloat* vals=from.getData();
	for(int i=0; i<16; ++i){
		matData[i]=vals[i];
	}
}
