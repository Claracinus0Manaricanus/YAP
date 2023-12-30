#include "Mesh.h"

//constructors
Mesh::Mesh(){}

Mesh::Mesh(MeshData* data){
	setMesh(data);
}


//destructors
Mesh::~Mesh(){
	free(vertices);
	free(faces);
	free(normals);
}


//loaders  functions need checking
int Mesh::setColor(vec4 inCol){
	for(int i=0;i<vCount;i++){
		vertices[i].col=inCol;
	}
	
	updated=true;
	return 0;
}

int Mesh::setMesh(MeshData* data){
	//vertices
	vCount=data->vCount;
	vertices=(vertex*)realloc(vertices,sizeof(vertex)*vCount);
	memcpy(vertices,data->vertices,sizeof(vertex)*vCount);
	
	//normals
	normals=(vec3*)realloc(normals,sizeof(vec3)*vCount);
	for(int i=0;i<vCount;i++){
		normals[i]=data->vertices[i].nor;
	}
	
	//faces
	fCount = data->fCount;
	if(fCount != 0){
		faces = (unsigned int*)realloc(faces, sizeof(int)*fCount);
		memcpy(faces, data->faces, sizeof(int)*fCount);
	}

	updated=true;
	return 0;
}

	
//extractors
int Mesh::getVCount(){
	return vCount;
}

unsigned int Mesh::getFCount(){
	return fCount;
}


//utility //WIP
int Mesh::reset(){
	vCount=0;
	free(vertices);
	vertices=NULL;
	
	fCount=0;
	free(faces);
	faces=NULL;
	
	updated=true;
	return 0;
}

int Mesh::addFace(uint32_t* indices){
	for(int i=0;i<3;i++){
		if(indices[i]>vCount)
			return 1;
	}

	fCount+=3;
	faces=(uint32_t*)realloc(faces,sizeof(int)+fCount);
	memcpy(faces+(sizeof(int)*(fCount-4)),indices,sizeof(int)*3);

	updated=true;
	return 0;
}

Mesh* Mesh::getPointer(){
	return this;
}