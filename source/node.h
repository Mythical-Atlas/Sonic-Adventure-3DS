#include <string.h>
#include <math.h>

#include "graphics.h"
#include "mesh.h"
#include "anim.h"
#include "animChannel.h"

#ifndef NODE_H
#define NODE_H

class Node {
public:
	int nameLength;
	const char* name;

	Mat4 transformation;
	int parentIndex;

    Mat4 tran;
    Mat4 tranNoAnim;
    // each frame, recAnimTran is updated going down the hierarchy
    // it represents, essentially, the pivot point for the next node's transforms

	int childCount;
	int* childIndices;

	int meshCount;
	int* meshIndices;

    int frame = 0;
    int lastKnownGoodFrame = 0;
    int timer = 0;

	void loadNode(
        Node* prevNodes,
        int nameLength,
        const char* name,
        float* transformation,
        int parentIndex,
        int childCount,
        int* childIndices,
        int meshCount,
        int* meshIndices
    ) {
        this->nameLength = nameLength;
        this->name = name;
        this->parentIndex = parentIndex;
        this->childCount = childCount;
        this->childIndices = childIndices;
        this->meshCount = meshCount;
        this->meshIndices = meshIndices;

        this->transformation.a1 = transformation[0];
		this->transformation.a2 = transformation[1];
		this->transformation.a3 = transformation[2];
		this->transformation.a4 = transformation[3];
		this->transformation.b1 = transformation[4];
		this->transformation.b2 = transformation[5];
		this->transformation.b3 = transformation[6];
		this->transformation.b4 = transformation[7];
		this->transformation.c1 = transformation[8];
		this->transformation.c2 = transformation[9];
		this->transformation.c3 = transformation[10];
		this->transformation.c4 = transformation[11];
		this->transformation.d1 = transformation[12];
		this->transformation.d2 = transformation[13];
		this->transformation.d3 = transformation[14];
		this->transformation.d4 = transformation[15];

        /*if(parentIndex == -1) {tran = this->transformation;}
        else {tran = multiplyMat4s(prevNodes[parentIndex].tran, this->transformation);}*/
    }

    void draw(Node* prevNodes, C3D_Tex* textures, Mesh* meshes, int animCount, Anim* anims, AnimChannel* channels) {
        if(parentIndex == -1) {tran = getIdentityMat4();} // does it matter if this is transformation or identity?
        else if(parentIndex == 0) {tran = this->transformation;} // unsure if model specific, but likely universal
        else {tran = prevNodes[parentIndex].tran;}

        if(parentIndex == -1) {tranNoAnim = this->transformation;}
        else {tranNoAnim = multiplyMat4s(prevNodes[parentIndex].tranNoAnim, this->transformation);}

        Mat4 animTran = getIdentityMat4();

        int a = 1;
        for(int c = 0; c < anims[a].channelCount; c++) {
            if(compareStrings(nameLength, name, channels[anims[a].channelIndices[c]].nodeNameLength, channels[anims[a].channelIndices[c]].nodeName)) {
                if(frame == 0) {lastKnownGoodFrame = 0;}
                else {
                    for(int f = 0; f < channels[anims[a].channelIndices[c]].rotationKeyCount; f++) {
                        if(frame == channels[anims[a].channelIndices[c]].rotationKeyTimes[f]) {
                            lastKnownGoodFrame = f;
                            break;
                        }
                    }
                }

                animTran = multiplyMat4s(animTran, getTranslationScaleMat4(channels[anims[a].channelIndices[c]].positionKeyValues[lastKnownGoodFrame], channels[anims[a].channelIndices[c]].scaleKeyValues[lastKnownGoodFrame]));
                animTran = multiplyMat4s(animTran, getQuatMat4(channels[anims[a].channelIndices[c]].rotationKeyValues[lastKnownGoodFrame]));
                
                break;
            }
        }

        tran = multiplyMat4s(tran, animTran);

        timer++;
        if(timer == 4) {
            timer = 0;
            frame++;
            if(frame == 32) {frame = 0;}
        }

        for(int i = 0; i < meshCount; i++) {
            if(hidKeysHeld() & KEY_X) {meshes[meshIndices[i]].updateVertData(tranNoAnim);}
            else {meshes[meshIndices[i]].updateVertData(tran);}
            meshes[meshIndices[i]].draw(textures);
        }
    }
};

#endif