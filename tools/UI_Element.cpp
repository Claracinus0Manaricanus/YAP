#include "UI_Element.h"

//constructor
UI_Element::UI_Element(vec2 iPos, vec2 iScale, const char* name):
pos(iPos),scale(iScale){
	//initialization
	NAME = (char*)calloc(strlen(name),sizeof(char));
	memcpy(NAME,name,strlen(name));
	glGenVertexArrays(1,&vArr);
	glGenBuffers(1,&posBuff);
	glGenBuffers(1,&texBuff);
	
	//tex coordinates (static)
	glBindVertexArray(vArr);
	glBindBuffer(GL_ARRAY_BUFFER,texBuff);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*12,vTex,GL_STATIC_DRAW);
	glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,(void*)0);
	glEnableVertexAttribArray(3);
	
	//vertex positions
	updateArrays();
}


//destructor
UI_Element::~UI_Element(){
	delete[] vTex;
	free(NAME);
	glDeleteBuffers(1,&posBuff);
	glDeleteBuffers(1,&texBuff);
	glDeleteVertexArrays(1,&vArr);
}


//loaders
int UI_Element::changePos(vec2 iPos){
	pos=iPos;
	updateArrays();
	return 0;
}

int UI_Element::changeScale(vec2 iScale){
	scale=iScale;
	updateArrays();
	return 0;
}

int UI_Element::loadTexture(TextureData* data){
	return mainTex.load(data);
}

int UI_Element::unloadTexture(){
	return mainTex.loadDefault();
}

int UI_Element::setActive(bool state){
	Active=state;
	return 0;
}


//extractors
bool UI_Element::isActive(){
	return Active;
}


//utility
int UI_Element::bind(){
	glBindVertexArray(vArr);
	mainTex.bind();
	return 0;
}

int UI_Element::unbind(){
	glBindVertexArray(0);
	mainTex.unbind();
	return 0;
}

int UI_Element::updateArrays(){
	vPos=new float[12]{
		pos.x+scale.x,pos.y+scale.y,
		pos.x+scale.x,pos.y-scale.y,
		pos.x-scale.x,pos.y-scale.y,
		pos.x-scale.x,pos.y-scale.y,
		pos.x-scale.x,pos.y+scale.y,
		pos.x+scale.x,pos.y+scale.y
	};
	updateBuffers();
	delete[] vPos;
	return 0;
}

int UI_Element::updateBuffers(){
	glBindVertexArray(vArr);
	glBindBuffer(GL_ARRAY_BUFFER,posBuff);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*12,vPos,GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,(void*)0);
	glEnableVertexAttribArray(0);
	return 0;
}