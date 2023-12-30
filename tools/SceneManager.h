#ifndef SCENEMANAGER
#define SCENEMANAGER

#include "Program.h"
#include "SkyBox.h"
#include "GameObject.h"
#include "UI_Element.h"
#include "Light.h"
#include "Camera.h"

class SceneManager{
protected:
	//state identifiers and constants
	bool S_FULLBRIGHT=false;
	int C_SKYBOX=-1;//-1 means no skybox otherwise index of skybox to use
	int C_LightTypes=2;
	//data holders L_ means array length of
	int L_objects=0;
	GameObject** objects=NULL;
	int L_lights=0;
	baseLight** lights=NULL;
	int L_UIElements=0;
	UI_Element** UIElements=NULL;
	int L_skyboxes=0;
	SkyBox** skyboxes=NULL;
	//programs
	program pFB[1];//perspective fullbright
	program pL[2];//perspective light //point 0, directional 1
	program ShadMap[2];//shadowMap calculator //point 0, directional 1
	program pSky;//skybox renderer
	program pUI;//UI_Element renderer

public:
	//constructors
	SceneManager();

	//destructor
	~SceneManager();

	//program control
	int setPrograms(program* in_pFB, program* in_pL, program* in_pUI,  program* in_pSky);

	//array control
	GameObject* addObject(const char* name, ObjectData* Data);
	baseLight* addLight(const char* name, int iType, vec4 iCol, vec3* lData);
	UI_Element* addUI_Element(const char* name, vec2 iPos, vec2 iScale, TextureData* data);
	SkyBox* addSkyBox(const char* name, const char* sides[6]);

	//element selector
	GameObject* getObject(const char* name, int* index=NULL);//get GameObject from name
	GameObject* getObject(int index);//get GameObject from index
	baseLight* getLight(const char* name, int* index=NULL);//get baseLight from name
	baseLight* getLight(int index);//get baseLight from index
	UI_Element* getUI_Element(const char* name, int* index=NULL);//get UI_Element from name
	UI_Element* getUI_Element(int index);//get UI_Element from index
	SkyBox* getSkyBox(const char* name, int* index=NULL);//get SkyBox from name
	SkyBox* getSkyBox(int index);//get SkyBox from index

	//element deletors WIP
	int deleteObject(const char* name);
	int deleteObject(uint32_t index);
	int deleteLight(const char* name);
	int deleteLight(int index);
	int deleteUI_Element(const char* name);
	int deleteUI_Element(int index);
	int deleteSkyBox(const char* name);
	int deleteSkyBox(int index);

	//drawing utility
	int draw(Camera* cam, ivec2 resolution);//draws scene to screen

	//state controllers
	int setFullbright(bool state);
	int setSkyBox(int index);
	int setSkyBox(const char* name);
};

//freetype library

#endif