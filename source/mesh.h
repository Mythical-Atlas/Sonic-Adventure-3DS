#include "graphics.h"

#ifndef MESH_H
#define MESH_H

class Mesh {
public:
	int vertCount;
	Vec3* vertices;
	Vec2* uvCoords;
	Vec3* normals;
	int materialID;

    Vertex* vertexData;

    void* vboData;
    C3D_BufInfo buffer;
    bool vboAllocated;

    void loadMesh(
        int vertCount,
        Vec3* meshVertPositions,
        Vec2* meshVertUVs,
        Vec3* meshVertNormals,
        int materialID
    ) {
        this->vertCount = vertCount;
        this->materialID = materialID;

        vertices = (Vec3*)linearAlloc(sizeof(Vec3) * vertCount);
        uvCoords = (Vec2*)linearAlloc(sizeof(Vec2) * vertCount);
        normals = (Vec3*)linearAlloc(sizeof(Vec3) * vertCount);
        vertexData = (Vertex*)linearAlloc(sizeof(Vertex) * vertCount);

        for(int i = 0; i < vertCount; i++) {
            vertices[i].x = meshVertPositions[i].x;
            vertices[i].y = meshVertPositions[i].y;
            vertices[i].z = meshVertPositions[i].z;
            uvCoords[i].x = meshVertUVs[i].x;
            uvCoords[i].y = meshVertUVs[i].y;
            normals[i].x = meshVertNormals[i].x;
            normals[i].y = meshVertNormals[i].y;
            normals[i].z = meshVertNormals[i].z;
        }
    }

    void initVBO() {
        if(!vboAllocated) {
            vboData = linearAlloc(sizeof(Vertex) * vertCount);
            BufInfo_Init(&buffer);
            BufInfo_Add(&buffer, vboData, sizeof(Vertex), 3, 0x210);
            vboAllocated = true;
        }
    }

   /* void applyNodeTransform(Mat4 transformation) {
        applyMat4ToVec3(v, transformation);
    }*/

    // method to transform verts
    // origin?

    void updateVertData(Mat4 transformation) {
         for(int v = 0; v < vertCount / 3; v++) {
            Vec3 vert0Temp = multiplyMat4Vec3(transformation, vertices[v * 3 + 0]);
            Vec3 vert1Temp = multiplyMat4Vec3(transformation, vertices[v * 3 + 1]);
            Vec3 vert2Temp = multiplyMat4Vec3(transformation, vertices[v * 3 + 2]);

            float vert0[3] = {vert0Temp.x, vert0Temp.y, vert0Temp.z};
            float vert1[3] = {vert1Temp.x, vert1Temp.y, vert1Temp.z};
            float vert2[3] = {vert2Temp.x, vert2Temp.y, vert2Temp.z};

            // i guess?
            float normX = calculateNormalX(vert0, vert1, vert2);
            float normY = calculateNormalY(vert0, vert1, vert2);
            float normZ = calculateNormalZ(vert0, vert1, vert2);

            vertexData[v * 3 + 0] = {{vert0[0], vert0[1], vert0[2]}, {uvCoords[v * 3 + 0].x, uvCoords[v * 3 + 0].y}, {normX, normY, normZ}};
            vertexData[v * 3 + 1] = {{vert1[0], vert1[1], vert1[2]}, {uvCoords[v * 3 + 1].x, uvCoords[v * 3 + 1].y}, {normX, normY, normZ}};
            vertexData[v * 3 + 2] = {{vert2[0], vert2[1], vert2[2]}, {uvCoords[v * 3 + 2].x, uvCoords[v * 3 + 2].y}, {normX, normY, normZ}};
        }
    }

    void draw(C3D_Tex* textures) {
        C3D_SetBufInfo(&buffer);
        memcpy(vboData, vertexData, sizeof(Vertex) * vertCount);
        C3D_TexBind(0, &textures[materialID]);
        C3D_DrawArrays(GPU_TRIANGLES, 0, vertCount);
    }
};

#endif