#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cstring>
#include "T0.h"
#include "cmMath/vectors.h"


//input utility
int CgetAxis(GLFWwindow* win, int key1, int key2);//custom get axis first key is positive second is negative


//debug features
void logInfo(const char* message);
void logDebug(const char* message);
void logError(const char* message);
void logGreen(const char* message);


//number utility
int cm_rand(int seed);//not tested


//glfw and opengl utility
void setOpenglVersion(int major, int minor, int profile);
void setWindowVars(bool floating, bool resizable);


//file import
vertex* importOBJ(std::string filename, int& size);//size = element number of array
//importOBJ needs checks at loading to vArr part
MeshData importHeightMap(const char* filename, float division_constant = 1.0f, int terrainResolution = 1, int* WIDTH = NULL, int* LENGTH = NULL);
//accepts 16-bit/color RGB, non-interlaced WIP

//calculation
MeshData calculateNormals(MeshData* buffer);//expects a valid index buffer


#endif