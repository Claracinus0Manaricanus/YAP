#ifndef SHADER
#define SHADER

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>

class shader{
public:
	uint32_t ID=0;

public:
	//constructor
	shader(){};
	shader(std::string source, uint32_t type, bool debug=true);

	//destructor
	~shader();

	//loaders
	int load(std::string source, uint32_t type, bool debug=true);

	//utility
	int Delete();
	int getError(char* ret);
};

#endif