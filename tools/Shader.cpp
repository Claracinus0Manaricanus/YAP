#include "Shader.h"

//constructor
shader::shader(std::string source, unsigned int type, bool debug){
	//creating shader
	ID=glCreateShader(type);

	//getting shader from source file
	std::string line,total;
	std::ifstream sourceIn(source);
	while(getline(sourceIn,line)){
		total+=line+'\n';
	}sourceIn.close();

	const char* src=total.c_str();

	//compiling shader
	glShaderSource(ID,1,&src,NULL);
	glCompileShader(ID);

	char inf[1024];
	if(debug&&getError(inf)>0){
		std::cout<<std::endl<<inf;
	}
}//needs error checking (fstream)


//destructor
shader::~shader(){
	if(ID!=0)
		glDeleteShader(ID);
}


//loaders
int shader::load(std::string source, uint32_t type, bool debug){
	//deleting old shader
	if(ID!=0)
		glDeleteShader(ID);

	//creating new shader
	ID=glCreateShader(type);

	//getting shader source file
	std::string line,total;
	std::ifstream sourceIn(source);
	while(getline(sourceIn,line)){
		total+=line+'\n';
	}sourceIn.close();

	const char* src=total.c_str();

	//compiling shader
	glShaderSource(ID,1,&src,NULL);
	glCompileShader(ID);

	char inf[1024];
	if(debug&&getError(inf)>0){
		std::cout<<std::endl<<inf;
	}
	return 0;
}


//utility
int shader::Delete(){
	glDeleteShader(ID);
	ID=0;
	return 0;
}//check needed

int shader::getError(char ret[1024]){
	int tempLength;
	glGetShaderInfoLog(ID,1024,&tempLength,ret);
	return tempLength;
}//error check needed (glGetShaderInfoLog())