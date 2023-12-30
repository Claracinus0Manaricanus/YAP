#include "Transform.h"

//constructors
Transform::Transform(const char* name, vec3 inPos, vec3 inRot, vec3 inSca):
position(inPos),rotation(inRot),scale(inSca){
	NAME=(char*)calloc(strlen(name)+1,sizeof(char));
	memcpy(NAME,name,strlen(name));
	calculateMatrix();
}


//destructor
Transform::~Transform(){
	free(NAME);
	free(OVM);
	free(rotM4);
	free(LookingAt);
}


//loaders
void Transform::setPosition(vec3 inPos){
	position=inPos;
	calculateMatrix();
}

void Transform::move(vec3 movement){
	position+=movement;
	calculateMatrix();
}


void Transform::setRotation(vec3 inRot){
	rotation=inRot;
	calculateMatrix();
}

void Transform::rotate(vec3 inRot){
	rotation+=inRot;

	int result=(int)(rotation.x/PI2);
	rotation.x-=(result*PI2);
	
	result=(int)(rotation.y/PI2);
	rotation.y-=(result*PI2);

	result=(int)(rotation.z/PI2);
	rotation.z-=(result*PI2);
	
	calculateMatrix();
}

void Transform::setScale(vec3 inSca){
	scale=inSca;
	updateNormals=true;
	calculateMatrix();
}

void Transform::setTransform(TransformData* Data){
	position=Data->position;
	rotation=Data->rotation;
	scale=Data->scale;
	updateNormals=true;
	calculateMatrix();
}


//state info
void Transform::useLookAt(bool state){
	if(state){
		if(LookingAt==NULL){
			LookingAt=new vec3{0,0,0};
		}
	}else{
		free(LookingAt);
		LookingAt=NULL;
	}
}


//calculators
void Transform::lookAt(vec3 lookey){
	if(LookingAt==NULL){
		LookingAt=new vec3{lookey};
	}
	free(rotM4);
	rotM4=LookAt(position,lookey);
}

void Transform::calculateRotation(){
	float tmpC=cos(rotation.x),tmpS=sin(rotation.x);//temporary cos and sin
	float rotXM4[16]={
		1,   0,    0,0,
		0,tmpC, tmpS,0,
		0,-tmpS,tmpC,0,
		0,   0,    0,1
	};
	tmpC=cos(rotation.y);tmpS=sin(rotation.y);
	float rotYM4[16]={
		tmpC,0,-tmpS,0,
		0,   1,    0,0,
		tmpS,0, tmpC,0,
		0,   0,    0,1
	};
	tmpC=cos(rotation.z);tmpS=sin(rotation.z);
	float rotZM4[16]={
		tmpC, tmpS,0,0,
		-tmpS,tmpC,0,0,
		0,       0,1,0,
		0,       0,0,1
	};

	float* temp1=m4_multiply(rotYM4,rotXM4);
	free(rotM4);
	rotM4=m4_multiply(rotZM4,temp1);
	//cleanup
	free(temp1);
}

void Transform::calculateMatrix(){
	//setting up matrices to multiply
	if(LookingAt==NULL){
		calculateRotation();
	}else{
		lookAt(*LookingAt);
	}

	float scaleM4[16]{
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};

	float transM4[16]={
		1,0,0,position.x,
		0,1,0,position.y,
		0,0,1,position.z,
		0,0,0,1
	};

	//final multiplication
	free(OVM);
	float* tmp4=m4_multiply(rotM4,scaleM4);
	OVM=m4_multiply(transM4,tmp4);
	free(tmp4);
}