#ifndef LIGHT
#define LIGHT

#include <cstdlib>
#include <cstring>
#include "T0.h"

class baseLight{
protected:
	char* NAME=NULL;
	vec3* data=NULL;//changes depending on light type
	vec4 color;//w used as strength
	int type;//usage for now 0=point 1=directional
	float* LVM=NULL;//light view matrix
	float* rawData=NULL;//to pass to shader (light specific)

public:
	//constructors
	baseLight(int iType=-1, const char* name="default\0");

	//destructors
	~baseLight();

	//utility
	virtual int calculateMatrix();
	virtual int buildData();

	//loaders
	int setColor(vec4 iCol);

	//extractors
	virtual void* getData();

protected:
	//friend classes
	friend class SceneManager;
};



class pointLight : public baseLight{
protected:

public:
	//constructors
	pointLight(vec4 iCol, vec3* iData, const char* name="default\0");

	//destructors
	~pointLight();

	//utility
	int buildData();

	//friend classes
	friend class SceneManager;
};



class directionalLight : public baseLight{
protected:

public:
	//constructors
	directionalLight(vec4 iCol, vec3* iData, const char* name="default\0");

	//destructors
	~directionalLight();

	//utility
	int buildData();

	//friend classes
	friend class SceneManager;
};

#endif