#ifndef PROGRAM
#define PROGRAM

#include <GL/glew.h>
#include "Shader.h"
#include "T0.h"

class program{
protected://make protected
	uint32_t ID=0;
	bool loaded;

public:
	//constructor
	program();
	program(shader* vertex, shader* fragment);
	program(const char* vName, const char* fName);

	//loaders
	int load(shader* vertex, shader* fragment);
	int load(const char* vName, const char* fName);
	
	//uniform utilities
	int setInt(const char* u_name, int data);
	int setVec2i(const char* u_name, ivec2 data);
	int setVec2(const char* u_name, vec2 data);
	int setVec3(const char* u_name, vec3 data);
	int setVec4Array(const char* u_name, int dataLength,const float* data);
	int setMat4(const char* u_name, const float* data);

	//extractors
	uint32_t getid();

	//destructor
	~program();

	//binder
	int use();
};//if ID=0 then error

#endif