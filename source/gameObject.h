#include "graphics.h"

using namespace std;

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
    public:
        Vertex* modelVertices;
        Vertex* vertices;
        int vertexCount;
        
        C3D_Tex* textures;
        int textureCount;
        //int* triTextureIDs;

        Vector3 position;
        Vector3 rotation;
        Vector3 scale;

        void* vboData;
        C3D_BufInfo buffer;
        bool vboAllocated;

        void loadVertices(Vertex* vertices, int vertexCount) {
            modelVertices = vertices;
            this->vertexCount = vertexCount;
        }
        void setTextures(C3D_Tex* textures, int textureCount/*, int* triTextureIDs*/) {
            this->textures = textures;
            this->textureCount = textureCount;
        }

        void initialize(Vector3 position, Vector3 rotation, Vector3 scale) {
            if(!vboAllocated) {
                vboData = linearAlloc(sizeof(Vertex) * vertexCount);
                BufInfo_Init(&buffer);
                BufInfo_Add(&buffer, vboData, sizeof(Vertex), 3, 0x210);
                vboAllocated = true;

                vertices = (Vertex*)linearAlloc(sizeof(Vertex) * vertexCount);
            }

            this->position = position;
            this->rotation = rotation;
            this->scale = scale;
        }

        void updateVertices() {
            for(int v = 0; v < vertexCount / 3; v++) {
                float vert0[3] = {modelVertices[v * 3 + 0].position[0], modelVertices[v * 3 + 0].position[1], modelVertices[v * 3 + 0].position[2]};
                float vert1[3] = {modelVertices[v * 3 + 1].position[0], modelVertices[v * 3 + 1].position[1], modelVertices[v * 3 + 1].position[2]};
                float vert2[3] = {modelVertices[v * 3 + 2].position[0], modelVertices[v * 3 + 2].position[1], modelVertices[v * 3 + 2].position[2]};

                float normX = calculateNormalX(vert0, vert1, vert2);
                float normY = calculateNormalY(vert0, vert1, vert2);
                float normZ = calculateNormalZ(vert0, vert1, vert2);

                vertices[v * 3 + 0] = {{vert0[0], vert0[1], vert0[2]}, {modelVertices[v * 3 + 0].texcoord[0], modelVertices[v * 3 + 0].texcoord[1]}, {normX, normY, normZ}};
                vertices[v * 3 + 1] = {{vert1[0], vert1[1], vert1[2]}, {modelVertices[v * 3 + 1].texcoord[0], modelVertices[v * 3 + 1].texcoord[1]}, {normX, normY, normZ}};
                vertices[v * 3 + 2] = {{vert2[0], vert2[1], vert2[2]}, {modelVertices[v * 3 + 2].texcoord[0], modelVertices[v * 3 + 2].texcoord[1]}, {normX, normY, normZ}};
            }
        }

        void draw() {
            C3D_SetBufInfo(&buffer);
            memcpy(vboData, vertices, sizeof(Vertex) * vertexCount);

            C3D_TexBind(0, &textures[0]);
            C3D_DrawArrays(GPU_TRIANGLES, 0, vertexCount);
        }

        void free() {
            if(vboAllocated) {
                linearFree(vboData);
                vboAllocated = false;
            }
        }
};

#endif