#include "Texture.h"
#include "stb_image.h"

//constructors
Texture::Texture(TextureData* data){
	glGenTextures(1,&ID);
	
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	unbind();
	
	if(data == NULL)
		loadDefault();
	else
		load(data);
}


//destructors
Texture::~Texture(){
	glDeleteTextures(1,&ID);
}


//loaders
int Texture::load(TextureData* data){
	if(data == NULL){
		printf("can't load image, data is NULL!\n");
		return -1;
	}
	
	texData = stbi_load(data->imageFile,&iWidth,&iHeight,&iChannel,0);

	if(texData == NULL){
		printf("Failed to load image: %s\n",data->imageFile);
		return -1;
	}

	if(iChannel < 3)
		return -1;

	bind();

	if(iChannel == 3)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,iWidth,iHeight,0,GL_RGB,GL_UNSIGNED_BYTE,texData);
	if(iChannel == 4)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,iWidth,iHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,texData);

	if(data->useMipmap){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	unbind();
	stbi_image_free(texData);
	return 0;
}

int Texture::loadDefault(){
	bind();
	texData = (uint8_t*)malloc(32);
	texData[0] = 255;
	texData[1] = 255;
	texData[2] = 255;
	texData[3] = 255;
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,texData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unbind();
	free(texData);
	return 0;
}


//extractors
int Texture::width(){
	return iWidth;
}

int Texture::height(){
	return iHeight;
}

int Texture::spectrum(){
	return iChannel;
}


//utility
int Texture::bind(){
	glBindTexture(GL_TEXTURE_2D,ID);
	return 0;
}

int Texture::unbind(){
	glBindTexture(GL_TEXTURE_2D,0);
	return 0;
}