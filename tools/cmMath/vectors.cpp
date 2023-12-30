#include "vectors.h"

//vector utilities
float Dot(vec3 a, vec3 b){
    float ret=0;
    
    for(int i=0;i<3;i++){
        ret+=a[i]*b[i];
    }

    return ret;
}

vec3 Cross(vec3 a, vec3 b){
    return {
        (a.y*b.z)-(a.z*b.y),
        (a.z*b.x)-(a.x*b.z),
        (a.x*b.y)-(a.y*b.x)
    };
}

float Length(vec3 a){
    return sqrt((a.x*a.x)+(a.y*a.y)+(a.z*a.z));
}

float Length(vec2 a){
    return sqrt((a.x*a.x)+(a.y*a.y));
}

vec3 Normalize(vec3 a){
    float aLength=Length(a);
    vec3 ret;
    if(aLength == 0){
        ret = {0,0,0};
    }else{
        ret = a/aLength;
    }
    return ret;
}

vec2 Normalize(vec2 a){
    float aLength=Length(a);
    vec2 ret;
    if(aLength == 0){
        ret = {0,0};
    }else{
        ret = a/aLength;
    }
    return ret;
}

//rotation related
float* LookAt(vec3 start, vec3 end, vec3 up){//needs optimization
    float* rotMat=(float*)malloc(sizeof(float)*16);//rotation matrix

    vec3 Z=Normalize(end-start);
    vec3 X=Normalize(Cross(Z,up));//to find X direction and unit that bitch
    vec3 Y=Cross(Z,X);

    for(int i=0;i<3;i++){
        rotMat[i]=X[i];
        rotMat[i+4]=Y[i];
        rotMat[i+8]=Z[i];
        rotMat[i+12]=0;
    }
    rotMat[12]=0;
    rotMat[13]=0;
    rotMat[14]=0;
    rotMat[15]=1;

    return rotMat;
}