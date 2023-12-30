#ifndef TEXTURE
#define TEXTURE

#include <GL/glew.h>
#include <cstdlib>
#include "T0.h"

class Texture{
protected:
	uint32_t ID;
	uint8_t* texData=NULL;
	int iHeight,iWidth,iChannel;

public:
	//constructors
	Texture(TextureData* data=NULL);

	//destructors
	~Texture();

	//loaders
	int load(TextureData* data);
	int loadDefault();

	//extractors
	int width();
	int height();
	int spectrum();

	//utility
	int bind();
	int unbind();
};

#endif