#ifndef CUBEMAP
#define CUBEMAP

#include <GL/glew.h>
#include "T0.h"

class CubeMap{
protected:
	uint32_t ID;
	int chan;

	//cubemap side textures data holders
	int iWidth,iHeight;//general purpose temporary
	uint8_t* iData=NULL;//general purpose temporary

public:
	//constructor
	CubeMap(const char* sides[6]=NULL);

	//destructor
	~CubeMap();
	
	//loaders
	int loadAllSides(const char* sides[6]);//starts from X+

	//utility
	int bindCM();//binding ID as GL_TEXTURE_CUBE_MAP
	int unbindCM();//binding 0 as GL_TEXTURE_CUBE_MAP

};

#endif