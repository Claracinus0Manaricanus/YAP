#ifndef WINDOW
#define WINDOW

#include <GLFW/glfw3.h>
#include "T0.h"

class Window{
private:
	GLFWwindow *ID;

public:
	//constructor
	Window(ivec2 resolution, const char* name);

	//extractor
	GLFWwindow* getid();

	//utility
	int changeAttrib(int attrib, int value);
	//int setResolution(int x, int y);//not implemented do not use
	ivec2 getResolution();
};

#endif