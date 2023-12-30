#include "Program.h"

//constructor
program::program(){
	ID=glCreateProgram();
	loaded=false;
}

program::program(shader* vertex, shader* fragment){
	ID=glCreateProgram();
	glAttachShader(ID,vertex->ID);
	glAttachShader(ID,fragment->ID);
	glLinkProgram(ID);
	loaded=true;
}

program::program(const char* vName, const char* fName){
	//create program
	ID=glCreateProgram();
	//load shaders
	shader vertex(vName,GL_VERTEX_SHADER);
	shader fragment(fName,GL_FRAGMENT_SHADER);
	//attaching
	glAttachShader(ID,vertex.ID);
	glAttachShader(ID,fragment.ID);
	glLinkProgram(ID);
	loaded=true;
	//cleaning
	vertex.Delete();
	fragment.Delete();
}


//loaders
int program::load(shader* vertex, shader* fragment){
	if(loaded)
		return -1;
	glAttachShader(ID,vertex->ID);
	glAttachShader(ID,fragment->ID);
	glLinkProgram(ID);
	loaded=true;
	return 0;
}

int program::load(const char* vName, const char* fName){
	if(loaded)
		return -1;
	//load shaders
	shader vertex(vName,GL_VERTEX_SHADER);
	shader fragment(fName,GL_FRAGMENT_SHADER);
	//attaching
	glAttachShader(ID,vertex.ID);
	glAttachShader(ID,fragment.ID);
	glLinkProgram(ID);
	loaded=true;
	//cleaning
	vertex.Delete();
	fragment.Delete();
	return 0;
}


//uniform utilities
int program::setInt(const char* u_name, int data){
	use();//program needs to be in use before uniform stuff
	glUniform1i(glGetUniformLocation(ID,u_name),data);
	return 0;//todo: return if any opengl errors (for all)
}

int program::setVec2i(const char* u_name, ivec2 data){
	use();
	glUniform2i(glGetUniformLocation(ID,u_name),data.x,data.y);
	return 0;
}

int program::setVec2(const char* u_name, vec2 data){
	use();
	glUniform2f(glGetUniformLocation(ID,u_name),data.x,data.y);
	return 0;
}

int program::setVec3(const char* u_name, vec3 data){
	use();
	glUniform3f(glGetUniformLocation(ID,u_name),data.x,data.y,data.z);
	return 0;
}

int program::setVec4Array(const char* u_name, int dataLength, const float* data){
	use();
	glUniform4fv(glGetUniformLocation(ID,u_name),dataLength,data);
	return 0;
}

int program::setMat4(const char* u_name, const float* data){
	use();
	glUniformMatrix4fv(glGetUniformLocation(ID,u_name),1,GL_TRUE,data);
	return 0;
}


//extractors
uint32_t program::getid(){
	return ID;
}


//destructor
program::~program(){
	glDeleteProgram(ID);
}


//binder
int program::use(){
	glUseProgram(ID);
	return 0;
}