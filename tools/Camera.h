#ifndef CAMERA
#define CAMERA

#include "Transform.h"

class Camera : public Transform{
protected:
	float fov;//field of view (60=default) //add changeFov
	float aspectRatio;

public:
//constructors
	Camera(float Ifov=60, float inAspectRatio=1);

//control
	void moveForward(float forward);
	void moveRight(float right);
	void moveUp(float up);
	void setAspectRatio(float inAspectRatio);

protected:
//overrides
	void calculateRotation();
	void calculateMatrix();
};

#endif