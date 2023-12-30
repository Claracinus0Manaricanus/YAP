#include "Camera.h"

//constructors
Camera::Camera(float Ifov, float inAspectRatio):
fov(Ifov*PI/180.0f),aspectRatio(inAspectRatio){}


//control
void Camera::moveForward(float forward){
	move({sin(rotation.y)*forward,0,cos(rotation.y)*forward});
}

void Camera::moveRight(float right){
	move({sin(rotation.y+(PI/2))*right,0,cos(rotation.y+(PI/2))*right});
}

void Camera::moveUp(float up){
	move({0,up,0});
}

void Camera::setAspectRatio(float inAspectRatio){
	aspectRatio=inAspectRatio;
}


//overrides
void Camera::calculateRotation(){
	float tmpC=cos(rotation.x),tmpS=sin(rotation.x);//temporary cos and sin
	float rotXM4[16]={
		1,   0,    0,0,
		0,tmpC, tmpS,0,
		0,-tmpS,tmpC,0,
		0,   0,    0,1
	};
	tmpC=cos(rotation.y);tmpS=sin(rotation.y);
	float rotYM4[16]={
		tmpC,0,-tmpS,0,
		0,   1,    0,0,
		tmpS,0, tmpC,0,
		0,   0,    0,1
	};
	tmpC=cos(rotation.z);tmpS=sin(rotation.z);
	float rotZM4[16]={
		tmpC,-tmpS,0,0,
		tmpS, tmpC,0,0,
		0,       0,1,0,
		0,       0,0,1
	};

	float* temp1=m4_multiply(rotXM4,rotYM4);
	free(rotM4);
	rotM4=m4_multiply(rotZM4,temp1);
	//cleanup
	free(temp1);
}

void Camera::calculateMatrix(){
	//setting up matrices to multiply
	if(LookingAt==NULL){
		calculateRotation();
	}else{
		lookAt(*LookingAt);
	}
	float transM4[16]={
		1,0,0,-position.x,
		0,1,0,-position.y,
		0,0,1,-position.z,
		0,0,0, 1
	};
	float proj[16]={
		1.0f/tan(fov),            0,  0,  0,
		0,(1.0f*aspectRatio)/tan(fov),  0,  0,
		0,                        0,  1, -0.01f,
		0,                        0,  1,  0
	};
	//multiplications
	float* temp1=m4_multiply(rotM4,transM4);
	free(OVM);
	OVM=m4_multiply(proj,temp1);
	free(temp1);
}