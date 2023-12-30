#include "U0.h"
#include "stb_image.h"

//input utility
int CgetAxis(GLFWwindow* win, int key1, int key2){//custom get axis first key is positive second is negative
	if(glfwGetKey(win,key1)==GLFW_PRESS)
		return 1;
	else if(glfwGetKey(win,key2)==GLFW_PRESS)
		return -1;
	return 0;
}


//debug features
void logInfo(const char* message){
	printf("\033[0;32mINFO:\033[0;36m%s",message);
}

void logDebug(const char* message){
	printf("\033[0;33mDEBUG:\033[0;36m%s",message);
}

void logError(const char* message){
	printf("\033[0;31mERROR:\033[0;36m%s",message);
}

void logGreen(const char* message){
	printf("\033[0;32m%s",message);
}


//number utility
int cm_rand(int seed){//not tested
	return (seed*seed<<23*seed*seed);
}


//glfw and opengl utility
void setOpenglVersion(int major, int minor, int profile){
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
	glfwWindowHint(GLFW_OPENGL_PROFILE,profile);
}

void setWindowVars(bool floating, bool resizable){
	glfwWindowHint(GLFW_FLOATING,floating);
	glfwWindowHint(GLFW_RESIZABLE,resizable);
}


//file import
vertex* importOBJ(std::string filename, int& size){//size = element number of array
	int vPosInd=0,vNorInd=0,vTexInd=0,vArrInd=0,tmpPos=0;
	//finding out array lengths
	std::string line,tmp;
	std::ifstream file(filename);
	while(getline(file,line)){
		if(line.length()>2){
			switch(line[0]+line[1]){
				case 'v'+' ':
					vPosInd++;
				break;
				case 'v'+'n':
					vNorInd++;
				break;
				case 'v'+'t':
					vTexInd++;
				break;
				case 'f'+' ':
					for(int i=0;i<line.length();i++){
						if(line[i]==' ')tmpPos++;
					}
					vArrInd+=(3*tmpPos-6);
					tmpPos=0;
				break;
			}
		}
	}file.close();

	size=vArrInd*3;//vertex array length

	//creating arrays
	vec3* vPos=new vec3[vPosInd*3];vPosInd=0;//positions
	vec3* vNor=new vec3[vNorInd*3];vNorInd=0;//normals
	vec2* vTex=new vec2[vTexInd*2];vTexInd=0;//texture coordinates
	vertex* vArr=new vertex[vArrInd*3];vArrInd=0;//vertices
	int index[9]={0};
	
	//getting array data from file
	std::ifstream fileT(filename);
	while(getline(fileT,line)){
		if(line.length()>2){
			switch(line[0]+line[1]){
				//positions
				case 'v'+' ':
					for(int i=2;i<line.length();i++){
						if(i==(line.length()-1))tmp+=line[i];
						if(line[i]!=' '&&i!=(line.length()-1)){
							tmp+=line[i];
						}else{
							switch(tmpPos){
								case 0:
									vPos[vPosInd].x=strtof(tmp.c_str(),nullptr);
									tmpPos++;
								break;
								case 1:
									vPos[vPosInd].y=strtof(tmp.c_str(),nullptr);
									tmpPos++;
								break;
								case 2:
									vPos[vPosInd].z=strtof(tmp.c_str(),nullptr);
									tmpPos=0;
								break;
							}
							tmp.clear();
						}
					}
					vPosInd++;
				break;
				//normals
				case 'v'+'n':
					for(int i=3;i<line.length();i++){
						if(i==(line.length()-1))tmp+=line[i];
						if(line[i]!=' '&&i!=(line.length()-1)){
							tmp+=line[i];
						}else{
							switch(tmpPos){
								case 0:
									vNor[vNorInd].x=strtof(tmp.c_str(),nullptr);
									tmpPos++;
								break;
								case 1:
									vNor[vNorInd].y=strtof(tmp.c_str(),nullptr);
									tmpPos++;
								break;
								case 2:
									vNor[vNorInd].z=strtof(tmp.c_str(),nullptr);
									tmpPos=0;
								break;
							}
							tmp.clear();
						}
					}
					vNorInd++;
				break;
				//texture coordinates
				case 'v'+'t':
					for(int i=3;i<line.length();i++){
						if(i==(line.length()-1))tmp+=line[i];
						if(line[i]!=' '&&i!=(line.length()-1)){
							tmp+=line[i];
						}else{
							switch(tmpPos){
								case 0:
									vTex[vTexInd].x=strtof(tmp.c_str(),nullptr);
									tmpPos++;
								break;
								case 1:
									vTex[vTexInd].y=strtof(tmp.c_str(),nullptr);
									tmpPos=0;
								break;
							}
							tmp.clear();
						}
					}
					vTexInd++;
				break;
				//faces
				case 'f'+' ':
					tmpPos=0;
					for(int i=2;i<line.length();i++){
						if(i==(line.length()-1))tmp+=line[i];
						if(line[i]!='/'&&line[i]!=' '&&i!=(line.length()-1)){
							tmp+=line[i];
						}else{
							switch(tmpPos%3){
							case 0://position
								if(tmp==""){
									index[tmpPos]=0;
									tmpPos++;
									break;
								}
								index[tmpPos]=stoi(tmp);
								tmpPos++;
						 	break;

							case 1://tex
								if(tmp==""){
									index[tmpPos]=0;
									tmpPos++;
									break;
								}
								index[tmpPos]=stoi(tmp);
								tmpPos++;
						 	break;

							case 2://normal
								if(tmp==""){
									index[tmpPos]=0;
									tmpPos++;
									break;
								}
								index[tmpPos]=stoi(tmp);
								tmpPos++;
						 	break;
							}
							tmp.clear();
						}
						//loading to vArr
						if(tmpPos==9||i==(line.length()-1)){//no index check is made
							for(int k=0;k<9;k++){
								vArr[vArrInd].pos=vPos[index[k]-1];k++;
								vArr[vArrInd].tex=vTex[index[k]-1];k++;
								vArr[vArrInd].nor=vNor[index[k]-1];
								vArrInd++;
							}
							tmpPos=6;
							index[3]=index[6];
							index[4]=index[7];
							index[5]=index[8];
						}
					}
				break;
			}
		}
	}
	fileT.close();

	//returning vertex*
	return vArr;
}
//importOBJ needs checks at loading to vArr part


MeshData importHeightMap(const char* filename, float division_constant, int terrainResolution, int* WIDTH, int* LENGTH){
	int width = 0, length = 0, channels = 0;

	//reading file
	stbi_set_flip_vertically_on_load(1);
	uint8_t* data = stbi_load(filename, &width, &length, &channels, 1);
	stbi_set_flip_vertically_on_load(0);
	width/=terrainResolution;
	length/=terrainResolution;

	MeshData m_data;

	if(data == NULL)
		return m_data;

	//creating mesh
	m_data.vCount = width*length;
	m_data.vertices = (vertex*)calloc(m_data.vCount, sizeof(vertex));
	m_data.fCount = (width-1)*(length-1)*6;
	m_data.faces = (uint32_t*)calloc(m_data.fCount, sizeof(int));

	//vertex data
	for(int k = 0; k < length; k++){//X
		for(int i = 0; i < width; i++){//Y
			//positions
			m_data.vertices[k*width+i].pos.x = i * terrainResolution / division_constant;
			m_data.vertices[k*width+i].pos.y = (float)data[(k*width*terrainResolution+i)*terrainResolution]*0.2f;
			m_data.vertices[k*width+i].pos.z = k * terrainResolution / division_constant;

			//m_data.vertices[k*width+i].nor = {1,1,1};

			//colors (BadAppleFormat currently)
			m_data.vertices[k*width+i].col={1,1,1,1};

			//UV coordinates
			m_data.vertices[k*width+i].tex.x = (float)i / width;
			m_data.vertices[k*width+i].tex.y = 1.0f - (float)k / length;
		}
	}

	//index buffer data (faces)
	for(int k = 0; k < (length - 1); k++){
		for(int i = 0; i < (width - 1); i++){
			m_data.faces[(k*(width-1)+i)*6] = (k*width+i);
			m_data.faces[(k*(width-1)+i)*6+1] = ((k+1)*width+i);
			m_data.faces[(k*(width-1)+i)*6+2] = ((k+1)*width+i+1);
			m_data.faces[(k*(width-1)+i)*6+3] = ((k+1)*width+i+1);
			m_data.faces[(k*(width-1)+i)*6+4] = (k*width+i+1);
			m_data.faces[(k*(width-1)+i)*6+5] = (k*width+i);
		}
	}

	//color test
	/*float mid = 0, totalDiff = 0, threshold = 0.3f;
	for(int k = 1; k < length - 1; k++){
		for(int i = 1; i < width - 1; i++){
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k+1)*width+i].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k+1)*width+i+1].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k)*width+i+1].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k-1)*width+i+1].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k-1)*width+i].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k-1)*width+i-1].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k)*width+i-1].pos.y);
			totalDiff += abs(m_data.vertices[k*width+i].pos.y - m_data.vertices[(k+1)*width+i-1].pos.y);
			mid = totalDiff / 8.0f;
			if(mid == 0) mid = threshold;
			totalDiff = 0;
			m_data.vertices[k*width+i].col = {0,threshold/mid,0,1};
		}
	}*/

	//pass length and width
	if(WIDTH != NULL) (*WIDTH) = width;
	if(LENGTH != NULL) (*LENGTH) = length;

	//free memory
	stbi_image_free(data);
	return m_data;
}


//calculation
MeshData calculateNormals(MeshData* buffer){
	//allocations
	vertex* vBuff=(vertex*)calloc(buffer->fCount, sizeof(vertex));
	
	//temporary variables
	vec3 V1 = {0}, V2 = {0}, Nor = {0};

	//calculations
	for(int i = 0; i < buffer->fCount; i+=3){
		//creates vectors from first vertex of a face to third and second vertices
		V1 = buffer->vertices[buffer->faces[i+2]].pos - buffer->vertices[buffer->faces[i]].pos;
		V2 = buffer->vertices[buffer->faces[i+1]].pos - buffer->vertices[buffer->faces[i]].pos;

		Nor = Normalize(Cross(V2, V1));

		//trasnferring to vBuff vertex array with new normals
		memcpy(&vBuff[i], &buffer->vertices[buffer->faces[i]], sizeof(vertex));
		vBuff[i].nor = Nor;
		memcpy(&vBuff[i+1], &buffer->vertices[buffer->faces[i+1]], sizeof(vertex));
		vBuff[i+1].nor = Nor;
		memcpy(&vBuff[i+2], &buffer->vertices[buffer->faces[i+2]], sizeof(vertex));
		vBuff[i+2].nor = Nor;

	}

	//constructing return data
	MeshData toRet;
	toRet.vCount = buffer->fCount;
	toRet.vertices = vBuff;
	toRet.fCount = 0;
	free(toRet.faces);
	toRet.faces = NULL;

	return toRet;
}