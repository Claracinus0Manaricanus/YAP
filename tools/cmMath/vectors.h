#ifndef VECTORS
#define VECTORS

#include "../T0.h"
#include <math.h>

//vector utilities
float Dot(vec3 a, vec3 b);
vec3 Cross(vec3 a, vec3 b);//a X b
float Length(vec3 a);
float Length(vec2 a);
vec3 Normalize(vec3 a);
vec2 Normalize(vec2 a);

//rotation related
float* LookAt(vec3 start, vec3 end, vec3 up = {0,1,0});

#endif