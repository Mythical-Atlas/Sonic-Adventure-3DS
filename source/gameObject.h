#include "graphics.h"
#include "node.h"
#include "mesh.h"
#include "anim.h"
#include "animChannel.h"
#include "model.h"

using namespace std;

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
    public:
        int nodeCount;
        Node* nodes;
        int meshCount;
        Mesh* meshes;
        int animCount;
        Anim* anims;
        int channelCount;
        AnimChannel* channels;
        
        C3D_Tex* textures;

        Vec3 position;
        Vec3 rotation;
        Vec3 scale;

        int currentAnimation;
        unsigned long long animStartTime;
        int frame;

        void loadModel(Model model) {
            nodeCount = model.nodeCount;
            meshCount = model.meshCount;
            animCount = model.animationCount;
            channelCount = model.animationChannelCount;

            nodes = (Node*)linearAlloc(sizeof(Node) * nodeCount);
            meshes = (Mesh*)linearAlloc(sizeof(Mesh) * meshCount);
            anims = (Anim*)linearAlloc(sizeof(Anim) * animCount);
            channels = (AnimChannel*)linearAlloc(sizeof(AnimChannel) * channelCount);

            for(int i = 0; i < nodeCount; i++) {nodes[i].loadNode(
                model.nodeNameLengths[i],
                model.nodeNames[i],
                model.nodeTransformations[i],
                model.nodeParentIndices[i],
                model.nodeChildCounts[i],
                model.nodeChildIndices[i],
                model.nodeMeshCounts[i],
                model.nodeMeshIndices[i]
            );}
            for(int i = 0; i < meshCount; i++) {
                meshes[i].loadMesh(
                    model.meshVertCounts[i],
                    model.meshVertPositions[i],
                    model.meshVertUVs[i],
                    model.meshVertNormals[i],
                    model.meshMaterialIDs[i]
                );
                meshes[i].initVBO();
            }
            for(int i = 0; i < animCount; i++) {anims[i].loadAnim(
                model.animationNameLengths[i],
                model.animationNames[i],
                model.animationDurations[i],
                model.animationTicksPerSeconds[i],
                model.animationChannelCounts[i],
                model.animationChannelIndices[i]
            );}
            for(int i = 0; i < channelCount; i++) {channels[i].loadAnimChannel(
                model.animationChannelNodeNameLengths[i],
                model.animationChannelNodeNames[i],
                model.animationChannelPositionKeyCounts[i],
                model.animationChannelPositionKeyTimes[i],
                model.animationChannelPositionKeyValues[i],
                model.animationChannelRotationKeyCounts[i],
                model.animationChannelRotationKeyTimes[i],
                model.animationChannelRotationKeyValues[i],
                model.animationChannelScaleKeyCounts[i],
                model.animationChannelScaleKeyTimes[i],
                model.animationChannelScaleKeyValues[i]
            );}

            model.freeModel();

            currentAnimation = -1;

            scale.x = 1;
            scale.y = 1;
            scale.z = 1;
        }

        void setTextures(C3D_Tex* textures) {this->textures = textures;}

        void setAnimation(int newAnim) {
            if(newAnim >= -1 && newAnim < animCount) {
                animStartTime = osGetTime();
                currentAnimation = newAnim;
            }
        }

        void updateAnimation(float animScale) {frame = (int)((osGetTime() - animStartTime) / 1000.0 * anims[currentAnimation].ticksPerSecond * animScale) % (int)anims[currentAnimation].duration;}

        // function to delay animation by adding to the animStartTime
        // useful for pausing an animation by not updating it, but you want to continue at the frame you left off at

        void draw() {
            Mat4 transform = getIdentityMat4();

            transform = multiplyMat4s(transform, getTranslationScaleMat4(position, scale));
            transform = multiplyMat4s(transform, getAngleRotationMat4(rotation));

            //printf("%i\n", nodeCount);
            for(int i = 0; i < 936; i++) {nodes[i].draw(transform, nodes, textures, meshes, anims, channels, currentAnimation, frame);}
        }
};

#endif