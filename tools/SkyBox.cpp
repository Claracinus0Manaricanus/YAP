#include "SkyBox.h"

//initializers
void SkyBox::initData(){
	float posses[108]={
		//Z+
		-1,-1, 1,-1, 1, 1, 1,-1, 1,//1 face
		 1,-1, 1,-1, 1, 1, 1, 1, 1,
		//Z-
		 1,-1,-1, 1, 1,-1,-1,-1,-1,
		-1,-1,-1, 1, 1,-1,-1, 1,-1,
		//Y+
		-1,	1, 1,-1, 1,-1, 1, 1,-1,
		 1, 1,-1, 1, 1, 1,-1, 1, 1,
		//Y-
		-1,-1,-1,-1,-1, 1, 1,-1,-1,
		 1,-1,-1,-1,-1, 1, 1,-1, 1,
		//X+
		 1,-1, 1, 1, 1, 1, 1,-1,-1,
		 1,-1,-1, 1, 1, 1, 1, 1,-1,
		//X-
		-1,-1,-1,-1, 1,-1,-1,-1, 1,
		-1,-1, 1,-1, 1,-1,-1, 1, 1
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*108,posses,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,(void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}


//constructors
SkyBox::SkyBox(const char* name){
	NAME=(char*)calloc(strlen(name)+1,sizeof(char));
	memcpy(NAME,name,strlen(name));
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	initData();
}

SkyBox::SkyBox(const char* sides[6], const char* name):
CubeMap(sides){
	NAME=(char*)calloc(strlen(name)+1,sizeof(char));
	memcpy(NAME,name,strlen(name));
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	initData();
}


//destructors
SkyBox::~SkyBox(){
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
	free(NAME);
}


//uility
int SkyBox::bind(){
	glBindVertexArray(VAO);
	bindCM();
	return 0;
}

int SkyBox::unbind(){
	glBindVertexArray(0);
	unbindCM();
	return 0;
}