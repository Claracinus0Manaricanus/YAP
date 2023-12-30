#ifndef TRANSFORM
#define TRANSFORM

#include <cstdlib>
#include <cstring>
#include "T0.h"
#include "cmMath/matrix4.h"
#include "cmMath/vectors.h"

class Transform{//add scale //needs better lookAt compatability
protected:
	char* NAME=NULL;
	vec3* LookingAt=NULL;
	bool updateNormals=true;

public:
	vec3 position;
	vec3 rotation;
	vec3 scale;
	float* OVM=NULL;//view matrix for the transform
	float* rotM4=NULL;//rotation matrix (for lookAt compatability)

public:
	//constructor
	Transform(const char* name="default", vec3 inPos={0}, vec3 inRot={0}, vec3 inSca={1});

	//destructor
	~Transform();

	//loaders
	void setPosition(vec3 inPos);
	void setRotation(vec3 inRot);
	void setScale(vec3 inSca);
	void setTransform(TransformData* Data);
	
	//transformations
	void move(vec3 movement);
	void rotate(vec3 inRot);

	//state info
	void useLookAt(bool state);

	//calculators
	virtual void lookAt(vec3 lookey);
protected:
	virtual void calculateRotation();
	virtual void calculateMatrix();
};

#endif