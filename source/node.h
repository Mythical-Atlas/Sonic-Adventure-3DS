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
    Mat4 recTran;
	int parentIndex;

	int childCount;
	int* childIndices;

	int meshCount;
	int* meshIndices;

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

        if(parentIndex == -1) {recTran = this->transformation;}
        else {recTran = applyMat4ToMat4(prevNodes[parentIndex].recTran, this->transformation);}
    }

    //Vec3 applyParentTransformToVert(Vec3 vert, Mat4 transformation) {}

    //Vec3 applyAnimationToVert(Vec3 vert, )

    void draw(C3D_Tex* textures, Mesh* meshes, Anim* anims, AnimChannel* channels) {
        for(int i = 0; i < meshCount; i++) {
            meshes[meshIndices[i]].updateVertData(recTran);
            meshes[meshIndices[i]].draw(textures);
        }

        // apply anims
        // draw meshes
    }
};

#endif