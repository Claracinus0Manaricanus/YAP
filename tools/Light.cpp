#include "Light.h"



/*baseLight*/

//constructors
baseLight::baseLight(int iType, const char* name):
type(iType){
	NAME=(char*)calloc(strlen(name)+1,sizeof(char));
	memcpy(NAME,name,strlen(name));
}


//destructors
baseLight::~baseLight(){
	free(NAME);
	free(LVM);
	free(data);
	free(rawData);
}


//utility
int baseLight::calculateMatrix(){//gets overriden
	return 0;
}

int baseLight::buildData(){
	return 0;
}


//loaders
int baseLight::setColor(vec4 iColor){
	color=iColor;
	return 0;
}


//extractors
void* baseLight::getData(){//gets overriden
	return NULL;
}



/*pointLight*/

//constructors
pointLight::pointLight(vec4 iCol, vec3* iData, const char* name):
baseLight(0,name){
	color=iCol;
	data=(vec3*)malloc(sizeof(vec3));
	memcpy(data,iData,sizeof(vec3));
	rawData=(float*)calloc(8,sizeof(float));
	buildData();
}

//destructors
pointLight::~pointLight(){

}

//utility
int pointLight::buildData(){
	memcpy(rawData,data,sizeof(float)*3);
	memcpy(&rawData[4],&color,sizeof(float)*4);
	return 0;
}



/*directionalLight*/

//constructors
directionalLight::directionalLight(vec4 iCol, vec3* iData, const char* name):
baseLight(1,name){
	color=iCol;
	data=(vec3*)malloc(sizeof(vec3));
	memcpy(data,iData,sizeof(vec3));
	rawData=(float*)calloc(8,sizeof(float));
	buildData();
}

//destructors
directionalLight::~directionalLight(){

}

//utility
int directionalLight::buildData(){
	memcpy(rawData,data,sizeof(float)*3);
	memcpy(&rawData[4],&color,sizeof(float)*4);
	return 0;
}