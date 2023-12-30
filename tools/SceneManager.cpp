#include "SceneManager.h"


/*********************************************************************************/
//SceneManager


//constructors
SceneManager::SceneManager(){
	//data holders
	objects = (GameObject**)malloc(sizeof(GameObject*));
	lights = (baseLight**)malloc(sizeof(baseLight*));
	UIElements = (UI_Element**)malloc(sizeof(UI_Element*));
	skyboxes = (SkyBox**)malloc(sizeof(SkyBox*));
	
	//shader programs WIP needs uniform control
	pFB[0].load(
	"../shaders/perspective(FB)/texture/vert.sha",
	"../shaders/perspective(FB)/texture/frag.sha");

	pL[0].load(
	"../shaders/perspective(L)/point/vert.sha",
	"../shaders/perspective(L)/point/frag.sha");
	pL[1].load(
	"../shaders/perspective(L)/directional/vert.sha",
	"../shaders/perspective(L)/directional/frag.sha");

	ShadMap[1].load(
	"../shaders/perspective(L)/ShadMapD/vert.sha",
	"../shaders/perspective(L)/ShadMapD/frag.sha");

	pSky.load(
	"../shaders/perspective(FB)/skybox/vert.sha",
	"../shaders/perspective(FB)/skybox/frag.sha");

	pUI.load(
	"../shaders/orthographic/UItex/vert.sha",
	"../shaders/orthographic/UItex/frag.sha");
	
	//texture units
	pUI.setInt("textureTU",0);
	pSky.setInt("tex0",0);
	pFB[0].setInt("tex0",0);
	pL[0].setInt("tex0",0);
	pL[1].setInt("tex0",0);
}


//destructors
SceneManager::~SceneManager(){
	free(objects);
	free(lights);
	free(UIElements);
	free(skyboxes);
}


//program control
int SceneManager::setPrograms(program* in_pFB, program* in_pL, program* in_pUI,  program* in_pSky){
	if(in_pFB!=NULL){
		pFB[0] = in_pFB[0];
		pFB[1] = in_pFB[1];
	}
	if(in_pL != NULL){
		pL[0] = in_pL[0];
		pL[1] = in_pL[1];
	}
	if(in_pUI!=NULL)pUI = (*in_pUI);
	if(in_pSky!=NULL)pSky = (*in_pSky);
	return 0;
}


//array control
GameObject* SceneManager::addObject(const char* name, ObjectData* data){
	//resize array
	L_objects++;
	objects = (GameObject**)realloc(objects,sizeof(GameObject*)*L_objects);
	//element creation
	objects[L_objects-1] = new GameObject(name);
	objects[L_objects-1]->setMesh(&(data->mData));
	objects[L_objects-1]->setTransform(&(data->trData));
	if(data->texData.imageFile != NULL){
		objects[L_objects-1]->loadTexture(&(data->texData));
	}
	//return newly created element
	return objects[L_objects-1];
}

baseLight* SceneManager::addLight(const char* name, int iType, vec4 iCol, vec3* lData){
	//resize array
	L_lights++;
	//element creation
	lights = (baseLight**)realloc(lights,sizeof(baseLight*)*L_lights);
	
	switch(iType){
		case 0://pointLight
			lights[L_lights-1] = new pointLight(iCol,lData,name);
		break;

		case 1://directionalLight
			lights[L_lights-1] = new directionalLight(iCol,lData,name);
		break;
	}

	//return newly created element
	return lights[L_lights-1];
}

UI_Element* SceneManager::addUI_Element(const char* name, vec2 iPos, vec2 iScale, TextureData* data){
	//resize array
	L_UIElements++;
	UIElements = (UI_Element**)realloc(UIElements,sizeof(UI_Element*)*L_UIElements);
	//element creation
	UIElements[L_UIElements-1] = new UI_Element(iPos,iScale,name);
	if(data->imageFile != NULL)
		UIElements[L_UIElements-1]->loadTexture(data);
	//return newly created element
	return UIElements[L_UIElements-1];
}

SkyBox* SceneManager::addSkyBox(const char* name, const char* sides[6]){
	//resize array
	L_skyboxes++;
	skyboxes = (SkyBox**)realloc(skyboxes,sizeof(SkyBox*)*L_skyboxes);
	//element creation
	skyboxes[L_skyboxes-1] = new SkyBox(sides,name);
	//return newly created element
	return skyboxes[L_skyboxes-1];
}


//element selector
GameObject* SceneManager::getObject(const char* name, int* index){
	for(int i=0;i<L_objects;i++){
		if(strcmp(objects[i]->NAME,name) == 0){
			if(index!=NULL)(*index) = i;
			return objects[i];
		}
	}
	if(index!=NULL)(*index) = -1;
	return NULL;
}

GameObject* SceneManager::getObject(int index){
	if(index<L_objects)
		return objects[index];
	return NULL;
}

baseLight* SceneManager::getLight(const char* name, int* index){
	for(int i=0;i<L_lights;i++){
		if(strcmp(lights[i]->NAME,name)==0){
			if(index!=NULL)(*index)=i;
			return lights[i];
		}
	}
	if(index!=NULL)(*index)=-1;
	return NULL;
}

baseLight* SceneManager::getLight(int index){
	if(index<L_lights)
		return lights[index];
	return NULL;
}

UI_Element* SceneManager::getUI_Element(const char* name, int* index){
	for(int i=0;i<L_UIElements;i++){
		if(strcmp(UIElements[i]->NAME,name)==0){
			if(index!=NULL)(*index)=i;
			return UIElements[i];
		}
	}
	if(index!=NULL)(*index)=-1;
	return NULL;
}

UI_Element* SceneManager::getUI_Element(int index){
	if(index<L_UIElements)
		return UIElements[index];
	return NULL;
}

SkyBox* SceneManager::getSkyBox(const char* name, int* index){
	for(int i=0;i<L_skyboxes;i++){
		if(strcmp(skyboxes[i]->NAME,name)==0){
			if(index!=NULL)(*index)=i;
			return skyboxes[i];
		}
	}
	if(index!=NULL)(*index)=-1;
	return NULL;
}

SkyBox* SceneManager::getSkyBox(int index){
	if(index<L_skyboxes)
		return skyboxes[index];
	return NULL;
}


//element deletors WIP
int SceneManager::deleteObject(const char* name){
	int index;
	getObject(name,&index);
	return deleteObject((uint32_t)index);
}

int SceneManager::deleteObject(uint32_t index){
	if(index<L_objects){
		free(objects[index]);
		L_objects--;
		for(int i=index;i<L_objects;i++){
			objects[i]=objects[i+1];
		}
		objects=(GameObject**)realloc(objects,sizeof(GameObject*)*L_objects);
		return index;
	}
	return -1;
}

int SceneManager::deleteLight(const char* name){
	int index;
	getLight(name,&index);
	return deleteLight((uint32_t)index);
}

int SceneManager::deleteLight(int index){
	if(index<L_lights){
		free(lights[index]);
		L_lights--;
		for(int i=index;i<L_lights;i++){
			lights[i]=lights[i+1];
		}
		lights=(baseLight**)realloc(lights,sizeof(baseLight*)*L_lights);
		return index;
	}
	return -1;
}

int SceneManager::deleteUI_Element(const char* name){
	int index;
	getUI_Element(name,&index);
	return deleteUI_Element((uint32_t)index);
}

int SceneManager::deleteUI_Element(int index){
	if(index<L_UIElements){
		free(UIElements[index]);
		L_UIElements--;
		for(int i=index;i<L_UIElements;i++){
			UIElements[i]=UIElements[i+1];
		}
		UIElements=(UI_Element**)realloc(UIElements,sizeof(UI_Element*)*L_UIElements);
		return index;
	}
	return -1;
}

int SceneManager::deleteSkyBox(const char* name){
	int index;
	getSkyBox(name,&index);
	return deleteSkyBox((uint32_t)index);
}

int SceneManager::deleteSkyBox(int index){
	if(index<L_skyboxes){
		free(skyboxes[index]);
		L_skyboxes--;
		for(int i=index;i<L_skyboxes;i++){
			skyboxes[i]=skyboxes[i+1];
		}
		C_SKYBOX--;
		skyboxes=(SkyBox**)realloc(skyboxes,sizeof(SkyBox*)*L_skyboxes);
		return index;
	}
	return -1;
}


//drawing utility
int SceneManager::draw(Camera* cam, ivec2 resolution){
	//textures dont bind to a sepcific unit so we specify it
	glActiveTexture(GL_TEXTURE0);

	//GameObject
	if(S_FULLBRIGHT || L_lights==0){
		//update camerauniforms
		pFB[0].setMat4("CVM",cam->OVM);

		//ObjectCount X LightCount calls
		for(int i=0;i<L_objects;i++){
			//set functions already call use
			pFB[0].setMat4("OVM",objects[i]->OVM);//object view matrix
			objects[i]->bind();
			if(objects[i]->fCount == 0){//without index buffer
				glDrawArrays(GL_TRIANGLES, 0, objects[i]->vCount);
			}else{//with index buffer
				glDrawElements(GL_TRIANGLES, objects[i]->fCount, GL_UNSIGNED_INT, (void*)0);
			}
			objects[i]->unbind();
		}

	}else{
		//update camera uniforms
		for(int i=0;i<C_LightTypes;i++){
			pL[i].setMat4("CVM",cam->OVM);//camera view matrix
		}

		//ObjectCount X LightCount calls
		for(int i=0;i<L_lights;i++){//render with lights
			//create shadow map (per light)
			pL[lights[i]->type].setVec4Array("light",2,lights[i]->rawData);//WIP
			
			//render using shadow map (per light)
			for(int k=0;k<L_objects;k++){
				pL[lights[i]->type].setMat4("OVM",objects[k]->OVM);//object view matrix
				objects[k]->bind();
				if(objects[k]->fCount == 0){//without index buffer
					glDrawArrays(GL_TRIANGLES, 0, objects[k]->vCount);
				}else{//with index buffer
					glDrawElements(GL_TRIANGLES, objects[k]->fCount, GL_UNSIGNED_INT, (void*)0);
				}
				objects[k]->unbind();
			}
			glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
		}
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	}

	//SkyBox
	pSky.setVec2("rot",{cam->rotation.x,cam->rotation.y});
	pSky.setVec2i("resolution",resolution);
	if(C_SKYBOX>=0&&C_SKYBOX<L_skyboxes){//can optimize
		skyboxes[C_SKYBOX]->bind();
		glDrawArrays(GL_TRIANGLES,0,36);
		skyboxes[C_SKYBOX]->unbind();
	}

	//UI_Element
	pUI.use();
	for(int i=0;i<L_UIElements;i++){
		if(UIElements[i]->isActive()){
			UIElements[i]->bind();
			glDrawArrays(GL_TRIANGLES,0,6);
			UIElements[i]->unbind();
		}
	}

	return 0;
}


//state controllers
int SceneManager::setFullbright(bool state){
	S_FULLBRIGHT=state;
	return 0;
}
int SceneManager::setSkyBox(int index){
	if(index<L_skyboxes){
		C_SKYBOX=index;
		return index;
	}
	return -1;
}

int SceneManager::setSkyBox(const char* name){
	for(int i=0;i<L_skyboxes;i++){
		if(strcmp(skyboxes[i]->NAME,name)==0){
			C_SKYBOX=i;
			return i;
		}
	}
	return -1;
}


/*********************************************************************************/