#include "CubeMap.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//constructors
CubeMap::CubeMap(const char* sides[6]){
	//texture generation opengl
	glGenTextures(1,&ID);
	bindCM();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	unbindCM();
	if(sides!=NULL)
		loadAllSides(sides);
}


//destructor
CubeMap::~CubeMap(){
	glDeleteTextures(1,&ID);
}


//loaders
int CubeMap::loadAllSides(const char* sides[6]){
	bindCM();
	for(int i=0;i<6;i++){
		if(sides[i]!=NULL){
			iData=stbi_load(sides[i],&iWidth,&iHeight,&chan,3);
			if(iData!=NULL){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,iWidth,iHeight,0,GL_RGB,GL_UNSIGNED_BYTE,iData);
				stbi_image_free(iData);
			}
		}
	}
	unbindCM();
	return 0;
}


//utility
int CubeMap::bindCM(){
	glBindTexture(GL_TEXTURE_CUBE_MAP,ID);
	return 0;
}

int CubeMap::unbindCM(){
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	return 0;
}