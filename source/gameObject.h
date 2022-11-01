#include "graphics.h"
#include "node.h"
#include "mesh.h"
#include "anim.h"
#include "animChannel.h"

using namespace std;

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
    public:
        Node* nodes;
        Mesh* meshes;
        Anim* anims;
        AnimChannel* channels;
        
        C3D_Tex* textures;

        Vec3 position;
        Quat rotation;
        Vec3 scale;

        void loadModel(
            int nodeCount,
            int* nodeNameLengths,
            const char** nodeNames,
            float** nodeTransformations,
            int* nodeParentIndices,
            int* nodeChildCounts,
            int** nodeChildIndices,
            int* nodeMeshCounts,
            int** nodeMeshIndices,
            int meshCount,
            int* meshVertCounts,
            float** meshVertices,
            int* meshMaterialIDs,
            int animationCount,
            int* animationNameLengths,
            const char** animationNames,
            double* animationDurations,
            double* animationTicksPerSeconds,
            int* animationChannelCounts,
            int** animationChannelIndices,
            int animationChannelCount,
            int* animationChannelNodeNameLengths,
            const char** animationChannelNodeNames,
            int* animationChannelPositionKeyCounts,
            double** animationChannelPositionKeyTimes,
            float** animationChannelPositionKeyValues,
            int* animationChannelRotationKeyCounts,
            double** animationChannelRotationKeyTimes,
            float** animationChannelRotationKeyValues,
            int* animationChannelScaleKeyCounts,
            double** animationChannelScaleKeyTimes,
            float** animationChannelScaleKeyValues
        ) {
            nodes = (Node*)linearAlloc(sizeof(Node) * nodeCount);
            meshes = (Mesh*)linearAlloc(sizeof(Mesh) * meshCount);
            anims = (Anim*)linearAlloc(sizeof(Anim) * animationCount);
            channels = (AnimChannel*)linearAlloc(sizeof(AnimChannel) * animationChannelCount);

            for(int i = 0; i < nodeCount; i++) {nodes[i].loadNode(
                nodes,
                nodeNameLengths[i],
                nodeNames[i],
                nodeTransformations[i],
                nodeParentIndices[i],
                nodeChildCounts[i],
                nodeChildIndices[i],
                nodeMeshCounts[i],
                nodeMeshIndices[i]
            );}
            for(int i = 0; i < meshCount; i++) {
                meshes[i].loadMesh(
                    meshVertCounts[i],
                    meshVertices[i],
                    meshMaterialIDs[i]
                );
                meshes[i].initVBO();
            }
            for(int i = 0; i < animationCount; i++) {anims[i].loadAnim(
                animationNameLengths[i],
                animationNames[i],
                animationDurations[i],
                animationTicksPerSeconds[i],
                animationChannelCounts[i],
                animationChannelIndices[i]
            );}
            for(int i = 0; i < animationChannelCount; i++) {channels[i].loadAnimChannel(
                animationChannelNodeNameLengths[i],
                animationChannelNodeNames[i],
                animationChannelPositionKeyCounts[i],
                animationChannelPositionKeyTimes[i],
                animationChannelPositionKeyValues[i],
                animationChannelRotationKeyCounts[i],
                animationChannelRotationKeyTimes[i],
                animationChannelRotationKeyValues[i],
                animationChannelScaleKeyCounts[i],
                animationChannelScaleKeyTimes[i],
                animationChannelScaleKeyValues[i]
            );}
        }
        void setTextures(C3D_Tex* textures) {this->textures = textures;}

        void draw() {
            for(int i = 0; i < nodeCount; i++) {
                if(nodes[i].name[0] != 'M') {nodes[i].draw(textures, meshes, anims, channels);}
            }
        }
};

#endif