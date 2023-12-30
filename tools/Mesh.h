#ifndef MESH
#define MESH

#include <cstring>
#include <cstdlib>
#include "T0.h"

class Mesh{
protected:
	bool updated=false;//if there is a change
	int vCount=0;//total vertices
	vertex* vertices=NULL;
	unsigned int fCount=0;//face count (total face times 3, array length)
	unsigned int* faces=NULL;//face data (in triangles)
	vec3* normals=NULL;//for scaling calculations (workaround)

public:
	//constructrors
	Mesh();
	Mesh(MeshData* Data);

	//destructor
	~Mesh();

	//loaders
	int setColor(vec4 inCol);
	int setMesh(MeshData* Data);

	//extractors
	int getVCount();
	unsigned int getFCount();

	//utility
	int reset();
	int addFace(uint32_t* indices);//adds a face
	int addVertex(vertex* toAdd, int count);//adds count vertices at the end of vertices array
	virtual Mesh* getPointer();
};//unsafe

#endif