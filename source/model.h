#ifndef MODEL_H
#define MODEL_H

#include "graphics.h"
#include "node.h"
#include "mesh.h"
#include "anim.h"
#include "animChannel.h"

union intBytes {
	int value;
	unsigned char bytes[4];
};
union floatBytes {
	float value;
	unsigned char bytes[4];
};
union doubleBytes {
	double value;
	unsigned char bytes[8];
};

int loadInt(unsigned char* data) {
    intBytes ib;
    ib.bytes[0] = data[0];
    ib.bytes[1] = data[1];
    ib.bytes[2] = data[2];
    ib.bytes[3] = data[3];
    return ib.value;
}
float loadFloat(unsigned char* data) {
    floatBytes fb;
    fb.bytes[0] = data[0];
    fb.bytes[1] = data[1];
    fb.bytes[2] = data[2];
    fb.bytes[3] = data[3];
    return fb.value;
}
double loadDouble(unsigned char* data) {
    doubleBytes db;
    db.bytes[0] = data[0];
    db.bytes[1] = data[1];
    db.bytes[2] = data[2];
    db.bytes[3] = data[3];
    db.bytes[4] = data[4];
    db.bytes[5] = data[5];
    db.bytes[6] = data[6];
    db.bytes[7] = data[7];
    return db.value;
}
Vec2 loadVec2(unsigned char* data) {
    Vec2 output;
    output.x = loadFloat(data);
    output.y = loadFloat(&data[4]);
    return output;
}
Vec3 loadVec3(unsigned char* data) {
    Vec3 output;
    output.x = loadFloat(data);
    output.y = loadFloat(&data[4]);
    output.z = loadFloat(&data[8]);
    return output;
}
Quat loadQuat(unsigned char* data) {
    Quat output;
    output.w = loadFloat(data);
    output.x = loadFloat(&data[4]);
    output.y = loadFloat(&data[8]);
    output.z = loadFloat(&data[12]);
    return output;
}
Mat4 loadMat(unsigned char* data) {
    Mat4 output;
    output.a1 = loadFloat(&data[4 *  0]); output.a2 = loadFloat(&data[4 *  1]); output.a3 = loadFloat(&data[4 *  2]); output.a4 = loadFloat(&data[4 *  3]);
    output.b1 = loadFloat(&data[4 *  4]); output.b2 = loadFloat(&data[4 *  5]); output.b3 = loadFloat(&data[4 *  6]); output.b4 = loadFloat(&data[4 *  7]);
    output.c1 = loadFloat(&data[4 *  8]); output.c2 = loadFloat(&data[4 *  9]); output.c3 = loadFloat(&data[4 * 10]); output.c4 = loadFloat(&data[4 * 11]);
    output.d1 = loadFloat(&data[4 * 12]); output.d2 = loadFloat(&data[4 * 13]); output.d3 = loadFloat(&data[4 * 14]); output.d4 = loadFloat(&data[4 * 15]);
    return output;
}

class Model {
public:
    int nodeCount;
    int* nodeNameLengths;
    const char** nodeNames;
    Mat4* nodeTransformations;
    int* nodeParentIndices;
    int* nodeChildCounts;
    int** nodeChildIndices;
    int* nodeMeshCounts;
    int** nodeMeshIndices;

    int meshCount;
    int* meshVertCounts;
    Vec3** meshVertPositions;
    Vec2** meshVertUVs;
    Vec3** meshVertNormals;
    int* meshMaterialIDs;

    int animationCount;
    int* animationNameLengths;
    const char** animationNames;
    double* animationDurations;
    double* animationTicksPerSeconds;
    int* animationChannelCounts;
    int** animationChannelIndices;

    int animationChannelCount;
    int* animationChannelNodeNameLengths;
    const char** animationChannelNodeNames;
    int* animationChannelPositionKeyCounts;
    double** animationChannelPositionKeyTimes;
    Vec3** animationChannelPositionKeyValues;
    int* animationChannelRotationKeyCounts;
    double** animationChannelRotationKeyTimes;
    Quat** animationChannelRotationKeyValues;
    int* animationChannelScaleKeyCounts;
    double** animationChannelScaleKeyTimes;
    Vec3** animationChannelScaleKeyValues;

    Node* nodes;
    Mesh* meshes;
    Anim* anims;
    AnimChannel* channels;

    void loadModel(unsigned char* data, int dsize) {
        int di = 16;

        nodeCount = loadInt(&data[di]); di += 4;
        nodeNameLengths = (int*)linearAlloc(sizeof(int) * nodeCount);
        nodeNames = (const char**)linearAlloc(sizeof(const char*) * nodeCount);
        nodeTransformations = (Mat4*)linearAlloc(sizeof(Mat4) * nodeCount);
        nodeParentIndices = (int*)linearAlloc(sizeof(int) * nodeCount);
        nodeChildCounts = (int*)linearAlloc(sizeof(int) * nodeCount);
        nodeChildIndices = (int**)linearAlloc(sizeof(int*) * nodeCount);
        nodeMeshCounts = (int*)linearAlloc(sizeof(int) * nodeCount);
        nodeMeshIndices = (int**)linearAlloc(sizeof(int*) * nodeCount);

        for(int i = 0; i < nodeCount; i++) {
            nodeNameLengths[i] = loadInt(&data[di]); di += 4;
            nodeNames[i] = (const char*)&data[di]; di += nodeNameLengths[i];

            nodeTransformations[i] = loadMat(&data[di]); di += 4 * 16;
            
            nodeParentIndices[i] = loadInt(&data[di]); di += 4;

            nodeChildCounts[i] = loadInt(&data[di]); di += 4;
            if(nodeChildCounts[i] > 0) {
                nodeChildIndices[i] = (int*)linearAlloc(sizeof(int) * nodeChildCounts[i]);
                for(int n = 0; n < nodeChildCounts[i]; n++) {nodeChildIndices[i][n] = loadInt(&data[di]); di += 4;}
            }

            nodeMeshCounts[i] = loadInt(&data[di]); di += 4;
            if(nodeMeshCounts[i] > 0) {
                nodeMeshIndices[i] = (int*)linearAlloc(sizeof(int) * nodeMeshCounts[i]);
                for(int n = 0; n < nodeMeshCounts[i]; n++) {nodeMeshIndices[i][n] = loadInt(&data[di]); di += 4;}
            }
        }

        meshCount = loadInt(&data[di]); di += 4;
        meshVertCounts = (int*)linearAlloc(sizeof(int) * meshCount);
        meshVertPositions = (Vec3**)linearAlloc(sizeof(Vec3*) * meshCount);
        meshVertUVs = (Vec2**)linearAlloc(sizeof(Vec2*) * meshCount);
        meshVertNormals = (Vec3**)linearAlloc(sizeof(Vec3*) * meshCount);
        meshMaterialIDs = (int*)linearAlloc(sizeof(int) * meshCount);

        for(int i = 0; i < meshCount; i++) {
            meshVertCounts[i] = loadInt(&data[di]); di += 4;
            if(meshVertCounts[i] > 0) {
                meshVertPositions[i] = (Vec3*)linearAlloc(sizeof(Vec3) * meshVertCounts[i]);
                meshVertUVs[i] = (Vec2*)linearAlloc(sizeof(Vec2) * meshVertCounts[i]);
                meshVertNormals[i] = (Vec3*)linearAlloc(sizeof(Vec3) * meshVertCounts[i]);
                for(int n = 0; n < meshVertCounts[i]; n++) {
                    meshVertPositions[i][n] = loadVec3(&data[di]); di += 4 * 3;
                    meshVertUVs[i][n] = loadVec2(&data[di]); di += 4 * 2;
                    meshVertNormals[i][n] = loadVec3(&data[di]); di += 4 * 3;
                }
            }

            meshMaterialIDs[i] = loadInt(&data[di]); di += 4;
        }

        animationCount = loadInt(&data[di]); di += 4;
        animationNameLengths = (int*)linearAlloc(sizeof(int) * animationCount);
        animationNames = (const char**)linearAlloc(sizeof(const char*) * animationCount);
        animationDurations = (double*)linearAlloc(sizeof(double) * animationCount);
        animationTicksPerSeconds = (double*)linearAlloc(sizeof(double) * animationCount);
        animationChannelCounts = (int*)linearAlloc(sizeof(int) * animationCount);
        animationChannelIndices = (int**)linearAlloc(sizeof(int*) * animationCount);

        for(int i = 0; i < animationCount; i++) {
            animationNameLengths[i] = loadInt(&data[di]); di += 4;
            animationNames[i] = (const char*)&data[di]; di += animationNameLengths[i];

            animationDurations[i] = loadDouble(&data[di]); di += 8;
            animationTicksPerSeconds[i] = loadDouble(&data[di]); di += 8;

            animationChannelCounts[i] = loadInt(&data[di]); di += 4;
            if(animationChannelCounts[i] > 0) {
                animationChannelIndices[i] = (int*)linearAlloc(sizeof(int) * animationChannelCounts[i]);
                for(int n = 0; n < animationChannelCounts[i]; n++) {
                    animationChannelIndices[i][n] = loadInt(&data[di]); di += 4;
                }
            }
        }

        animationChannelCount = loadInt(&data[di]); di += 4;
        animationChannelNodeNameLengths = (int*)linearAlloc(sizeof(int) * animationChannelCount);
        animationChannelNodeNames = (const char**)linearAlloc(sizeof(const char*) * animationChannelCount);
        animationChannelPositionKeyCounts = (int*)linearAlloc(sizeof(int) * animationChannelCount);
        animationChannelPositionKeyTimes = (double**)linearAlloc(sizeof(double*) * animationChannelCount);
        animationChannelPositionKeyValues = (Vec3**)linearAlloc(sizeof(Vec3*) * animationChannelCount);
        animationChannelRotationKeyCounts = (int*)linearAlloc(sizeof(int) * animationChannelCount);
        animationChannelRotationKeyTimes = (double**)linearAlloc(sizeof(double*) * animationChannelCount);
        animationChannelRotationKeyValues = (Quat**)linearAlloc(sizeof(Quat*) * animationChannelCount);
        animationChannelScaleKeyCounts = (int*)linearAlloc(sizeof(int) * animationChannelCount);
        animationChannelScaleKeyTimes = (double**)linearAlloc(sizeof(double*) * animationChannelCount);
        animationChannelScaleKeyValues = (Vec3**)linearAlloc(sizeof(Vec3*) * animationChannelCount);

        for(int i = 0; i < animationChannelCount; i++) {
            animationChannelNodeNameLengths[i] = loadInt(&data[di]); di += 4;
            animationChannelNodeNames[i] = (const char*)&data[di]; di += animationChannelNodeNameLengths[i];

            animationChannelPositionKeyCounts[i] = loadInt(&data[di]); di += 4;
            if(animationChannelPositionKeyCounts[i] > 0) {
                animationChannelPositionKeyTimes[i] = (double*)linearAlloc(sizeof(double) * animationChannelPositionKeyCounts[i]);
                animationChannelPositionKeyValues[i] = (Vec3*)linearAlloc(sizeof(Vec3) * animationChannelPositionKeyCounts[i]);
                for(int n = 0; n < animationChannelPositionKeyCounts[i]; n++) {animationChannelPositionKeyTimes[i][n] = loadDouble(&data[di]); di += 8;}
                for(int n = 0; n < animationChannelPositionKeyCounts[i]; n++) {animationChannelPositionKeyValues[i][n] = loadVec3(&data[di]); di += 4 * 3;}
            }

            animationChannelRotationKeyCounts[i] = loadInt(&data[di]); di += 4;
            if(animationChannelRotationKeyCounts[i] > 0) {
                animationChannelRotationKeyTimes[i] = (double*)linearAlloc(sizeof(double) * animationChannelRotationKeyCounts[i]);
                animationChannelRotationKeyValues[i] = (Quat*)linearAlloc(sizeof(Quat) * animationChannelRotationKeyCounts[i]);
                for(int n = 0; n < animationChannelRotationKeyCounts[i]; n++) {animationChannelRotationKeyTimes[i][n] = loadDouble(&data[di]); di += 8;}
                for(int n = 0; n < animationChannelRotationKeyCounts[i]; n++) {animationChannelRotationKeyValues[i][n] = loadQuat(&data[di]); di += 4 * 4;}
            }

            animationChannelScaleKeyCounts[i] = loadInt(&data[di]); di += 4;
            if(animationChannelScaleKeyCounts[i] > 0) {
                animationChannelScaleKeyTimes[i] = (double*)linearAlloc(sizeof(double) * animationChannelScaleKeyCounts[i]);
                animationChannelScaleKeyValues[i] = (Vec3*)linearAlloc(sizeof(Vec3) * animationChannelScaleKeyCounts[i]);
                for(int n = 0; n < animationChannelScaleKeyCounts[i]; n++) {animationChannelScaleKeyTimes[i][n] = loadDouble(&data[di]); di += 8;}
                for(int n = 0; n < animationChannelScaleKeyCounts[i]; n++) {animationChannelScaleKeyValues[i][n] = loadVec3(&data[di]); di += 4 * 3;}
            }
        }
    }

    void freeModel() {
        /*for(int i = 0; i < nodeCount; i++) {
            free(nodeChildIndices[i]);
            free(nodeMeshIndices[i]);
        }*/

        /*free(nodeNameLengths);
        free(nodeNames);
        free(nodeTransformations);
        free(nodeParentIndices);
        free(nodeChildCounts);
        free(nodeChildIndices);
        free(nodeMeshCounts);
        free(nodeMeshIndices);*/
    }
};

#endif