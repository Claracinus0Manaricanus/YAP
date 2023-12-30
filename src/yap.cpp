#define GLFW_INCLUDE_NONE
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../tools/Window.h"
#include "../tools/SceneManager.h"
#include "../tools/U0.h"
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

#define r_DefaultCapacity 1.5f

//structs
struct Raindrop{
    ivec2 pos = {0};

    float capacity = r_DefaultCapacity;
    float sediment = 0;
    int life = 100;
};

//number generator
random_device RNG;

//analyze functions
MeshData analyzeHeight(MeshData mData, int tWidth, int tHeight, float highestPoint = 1);
MeshData analyzeSlope(MeshData mData, int tWidth, int tHeight, float threshold = 1);//threshold is an angle in radians, anything above the threshold will be dark

//simulation functions
MeshData stepErosion(MeshData mData, int tWidth, int tHeight, int steps = 1);




int main(int argc, char** argv){
    if(!glfwInit()){
        return 1;
    }

    //glfw vars
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_FLOATING,1);
	glfwWindowHint(GLFW_RESIZABLE,1);

    //window
    ivec2 res = {0,0};
    Window mWindow({800,500},"mWindow");
    glfwMakeContextCurrent(mWindow.getid());
    if(glewInit()!=GLEW_OK){
        glfwDestroyWindow(mWindow.getid());
        glfwTerminate();
        return 1;
    }

    //opengl settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);

    //camera
    Camera mCam;
    glClearColor(0.1f,0.1f,0.5f,1);
    float cSpeed = 10.0f;

    //scene
    SceneManager mScene;

    //lights
    vec3 lData = {0,1,0};
    mScene.addLight("mDirect",1,{1,1,1,0.4f},&lData);

    //objects
    ObjectData tData;
    int tWidth = 0, tHeight = 0;
    tData.trData.scale = {1,1,1};
    MeshData terrainMesh = importHeightMap("../images/heightmaps/heighmap.png", 1.0f,1,&tWidth,&tHeight);
    
    //terrainMesh = analyzeHeight(terrainMesh, tWidth, tHeight);
    //terrainMesh = analyzeSlope(terrainMesh, tWidth, tHeight);
    
    tData.mData = terrainMesh;
    tData.mData = calculateNormals(&tData.mData);

    GameObject* tObj = mScene.addObject("terrain", &tData);

    //cursor
    bool cursorActive = false, escAllow = true;
    double cursorX = 0, cursorY = 0;

    //time
    double dt = 0; 

    //main loop
    while(!glfwWindowShouldClose(mWindow.getid())){
        //time updates
        dt = glfwGetTime();
        glfwSetTime(0);

        //screen updates
        res = mWindow.getResolution();
        mCam.setAspectRatio((float)res.x/res.y);
        glViewport(0,0,res.x,res.y);

		//toggle cursor
		if(glfwGetKey(mWindow.getid(),GLFW_KEY_ESCAPE)==GLFW_PRESS&&escAllow){
			//cursor variables
			cursorActive=!cursorActive;
			escAllow=false;
		}else if(glfwGetKey(mWindow.getid(),GLFW_KEY_ESCAPE)==GLFW_RELEASE){
			//for snappier feel
			escAllow=true;
		}

        //cursor capture
        if(!cursorActive){
			//hide cursor
			glfwSetInputMode(mWindow.getid(),GLFW_CURSOR,GLFW_CURSOR_DISABLED);
			if(!escAllow)
				glfwSetCursorPos(mWindow.getid(),0,0);	

			//mouse input for roty (cursor position of the current frame)
			glfwGetCursorPos(mWindow.getid(),&cursorX,&cursorY);

			//update rot
			mCam.rotate({(float)cursorY*dt,(float)cursorX*dt,0});

			//movement variable updates (add the trig functions)
			mCam.moveForward(cSpeed*CgetAxis(mWindow.getid(),GLFW_KEY_W,GLFW_KEY_S)*dt*2);
			mCam.moveRight(cSpeed*CgetAxis(mWindow.getid(),GLFW_KEY_D,GLFW_KEY_A)*dt*2);
			mCam.moveUp(cSpeed*CgetAxis(mWindow.getid(),GLFW_KEY_E,GLFW_KEY_Q)*dt*2);

			//reset cursor position
			glfwSetCursorPos(mWindow.getid(),0,0);
		}else{
			//make cursor visible
			glfwSetInputMode(mWindow.getid(),GLFW_CURSOR,GLFW_CURSOR_NORMAL);
		}

        //simulate stuff, comment the line below to disable simulating
        terrainMesh = stepErosion(terrainMesh, tWidth, tHeight, 100);

        //analyze stuff
        //terrainMesh = analyzeHeight(terrainMesh, tWidth, tHeight);//uncomment this line to analyze height
	//terrainMesh = analyzeSlope(terrainMesh, tWidth, tHeight);//uncomment this line to analyze slope

        //reload ObjectData
        free(tData.mData.vertices);
        tData.mData = terrainMesh;
        tData.mData = calculateNormals(&tData.mData);
        tObj->setMesh(&tData.mData);

        //draw calls
        mScene.draw(&mCam, res);

        //window events
        glfwSwapBuffers(mWindow.getid());
        glfwPollEvents();

        //clearing screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwDestroyWindow(mWindow.getid());
    glfwTerminate();
    return 0;
}







//analyze functions
MeshData analyzeHeight(MeshData mData, int tWidth, int tHeight, float highestPoint){
    
    float vHeight = 0;
    for(int k = 0; k < tHeight; k++){
        for(int i = 0; i < tWidth; i++){
            vHeight = mData.vertices[k*tWidth+i].pos.y / 51.0f;
            printf("height: %f\n",vHeight);
            mData.vertices[k*tWidth+i].col={vHeight,0.6f,0,1};
        }
    }

    return mData;
}



MeshData analyzeSlope(MeshData mData, int tWidth, int tHeight, float threshold){

    int count = 0;
    float total = 0;
    float approx = 0;
    float diff = 0;

    for(int k = 0; k < tHeight; k++){
        for(int i = 0; i < tWidth; i++){
            if(k < (tHeight-1)){//up
                count++;
                total += mData.vertices[(k+1)*tWidth+i].pos.y;
            }
            if(i < (tWidth-1)){//right
                count++;
                total += mData.vertices[k*tWidth+i+1].pos.y;
            }
            if(k > 0){//down
                count++;
                total += mData.vertices[(k-1)*tWidth+i].pos.y;
            }
            if(i > 0){//left
                count++;
                total += mData.vertices[k*tWidth+i-1].pos.y;
            }
            approx = total/count;
            diff = abs(approx - mData.vertices[k*tWidth+i].pos.y)/51.0f;
            mData.vertices[k*tWidth+i].col = {0,diff,0,1};
        }
    }
    
    return mData;
}





//simulation functions
MeshData stepErosion(MeshData mData, int tWidth, int tHeight, int steps){
    Raindrop rDrop;

    int tX=0,tY=0;
    float lowest=0;
    float oldLow;

    float power;
    float hDif;

    float tmpSed = 0;

    for(int cStep = 0; cStep < steps; cStep++){
        rDrop.pos.x = RNG()%(tWidth-1);
        rDrop.pos.y = RNG()%(tHeight-1);

        rDrop.sediment = 0;

        tX=rDrop.pos.x;
        tY=rDrop.pos.y;

        lowest = mData.vertices[tWidth*(tY)+tX].pos.y;
        printf("1) lowest: %f\n",lowest);

        for(int i = 0; i < rDrop.life; i++){
            oldLow = lowest;
            //determine lowest point
            if(tY < (tHeight-1)){//up
                if(lowest > mData.vertices[tWidth*(tY+1)+tX].pos.y){
                    lowest = mData.vertices[tWidth*(tY+1)+tX].pos.y;
                    rDrop.pos.x = tX;
                    rDrop.pos.y = tY+1;
                }
            }
        printf("2) lowest: %f\n",lowest);
            if(tX < (tWidth-1)){//right
                if(lowest > mData.vertices[tWidth*(tY)+tX+1].pos.y){
                    lowest = mData.vertices[tWidth*(tY)+tX+1].pos.y;
                    rDrop.pos.x = tX+1;
                    rDrop.pos.y = tY;
                }
            }
        printf("3) lowest: %f\n",lowest);
            if(tY > 0){//down
                if(lowest > mData.vertices[tWidth*(tY-1)+tX].pos.y){
                    lowest = mData.vertices[tWidth*(tY-1)+tX].pos.y;
                    rDrop.pos.x = tX;
                    rDrop.pos.y = tY-1;
                }
            }
        printf("4) lowest: %f\n",lowest);
            if(tX > 0){//left
                if(lowest > mData.vertices[tWidth*tY+tX-1].pos.y){
                    lowest = mData.vertices[tWidth*tY+tX-1].pos.y;
                    rDrop.pos.x = tX-1;
                    rDrop.pos.y = tY;
                }
            }
        printf("5) lowest: %f\n",lowest);
            
            //update
            tX=rDrop.pos.x;
            tY=rDrop.pos.y;
            hDif = (oldLow - lowest)/255.0f;
            if(hDif == 0)break;
            power = (rDrop.capacity - rDrop.sediment)/rDrop.capacity;//capacity left (negative if over capacity)

            printf("%d) tX: %d, tY: %d\n",i,tX,tY);

            //sediment calc
            tmpSed = hDif * power;
            printf("sediment: %f, hDif: %f, power: %f, ",tmpSed,hDif,power);
            printf("totalSed: %f\n",rDrop.sediment);
            rDrop.sediment += tmpSed;
            mData.vertices[tWidth*(tY)+tX].pos.y -= tmpSed;
        }
        //dropoff all sediment
        printf("out) tX: %d, tY: %d\n",tX,tY);
        mData.vertices[tWidth*(tY)+tX].pos.y += rDrop.sediment;
        mData.vertices[tWidth*(tY)+tX].col.z += 0.02f;
    }

    return mData;
}
