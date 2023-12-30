#ifndef SKYBOX
#define SKYBOX

#include <cstring>
#include <cstdlib>
#include "CubeMap.h"

class SkyBox : public CubeMap{
protected:
	char* NAME=NULL;
	//opengl
	uint32_t VAO;//vertex array
	uint32_t VBO;//indexes 0:position 1:texture coordinates
	
private:
	//initializers
	void initData();

public:
	//constructors
	SkyBox(const char* name="default");
	SkyBox(const char* sides[6], const char* name="default");

	//destructors
	~SkyBox();

	//uility
	int bind();
	int unbind();

	//friend classes
	friend class SceneManager;
};

#endif