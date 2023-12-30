#include "Window.h"

//constructor
Window::Window(ivec2 resolution, const char* name){
	ID=glfwCreateWindow(resolution.x,resolution.y,name,NULL,NULL);
}


//extractor
GLFWwindow* Window::getid(){
	return ID;
}


//utility
int Window::changeAttrib(int attrib, int value){
	glfwSetWindowAttrib(ID,attrib,value);
	return 0;
}//error check needed

ivec2 Window::getResolution(){
	ivec2 temp;
	glfwGetFramebufferSize(ID,&temp.x,&temp.y);
	return temp;
}