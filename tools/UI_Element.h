#ifndef UI_ELEMENT
#define UI_ELEMENT

#include <GL/glew.h>
#include <cstring>
#include <cstdlib>
#include "Texture.h"
#include "T0.h"

class UI_Element{//has 6 vertices
protected:
	bool Active=true;
	char* NAME;
	uint32_t vArr;//opengl vertex array
	uint32_t posBuff,texBuff;//opengl buffers
	float* vPos=NULL;//vertex positions
	float* vTex=new float[12]{1,0,1,1,0,1,0,1,0,0,1,0};//tex coords
	vec2 pos={0,0},scale={1,1};//screen coordinates and scale
	Texture mainTex;//texture to use

public:
	//constructor
	UI_Element(vec2 iPos={0,0}, vec2 iScale={1,1}, const char* name="default");

	//destructor
	~UI_Element();

	//loaders
	int changePos(vec2 iPos);
	int changeScale(vec2 iScale);
	int loadTexture(TextureData* data);
	int unloadTexture();
	int setActive(bool state);

	//extractors
	bool isActive();

	//utility
	int bind();
	int unbind();
	int updateArrays();
	int updateBuffers();
	
	//friend classes
	friend class SceneManager;
};//unsafe
//NEEDS FIX:
//unsafe usage without loaded textures

#endif