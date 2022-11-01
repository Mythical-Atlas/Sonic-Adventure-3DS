#include "graphics.h"

#ifndef ANIM_CHANNEL_H
#define ANIM_CHANNEL_H

class AnimChannel {
public:
	int nodeNameLength;
	const char* nodeName;

	int positionKeyCount;
	double* positionKeyTimes;
	Vec3* positionKeyValues;

	int rotationKeyCount;
	double* rotationKeyTimes;
	Quat* rotationKeyValues;

	int scaleKeyCount;
	double* scaleKeyTimes;
	Vec3* scaleKeyValues;

    void loadAnimChannel(
        int nodeNameLength,
        const char* nodeName,
        int positionKeyCount,
        double* positionKeyTimes,
        float* positionKeyFloatValues,
        int rotationKeyCount,
        double* rotationKeyTimes,
        float* rotationKeyFloatValues,
        int scaleKeyCount,
        double* scaleKeyTimes,
        float* scaleKeyFloatValues
    ) {
        this->nodeNameLength = nodeNameLength;
        this->nodeName = nodeName;
        this->positionKeyCount = positionKeyCount;
        this->positionKeyTimes = positionKeyTimes;
        this->rotationKeyCount = rotationKeyCount;
        this->rotationKeyTimes = rotationKeyTimes;
        this->scaleKeyCount = scaleKeyCount;
        this->scaleKeyTimes = scaleKeyTimes;

        positionKeyValues = (Vec3*)linearAlloc(sizeof(Vec3) * positionKeyCount);
        rotationKeyValues = (Quat*)linearAlloc(sizeof(Quat) * rotationKeyCount);
        scaleKeyValues = (Vec3*)linearAlloc(sizeof(Vec3) * scaleKeyCount);

        for(int i = 0; i < positionKeyCount; i++) {
            positionKeyValues[i].x = positionKeyFloatValues[i * 3 + 0];
            positionKeyValues[i].y = positionKeyFloatValues[i * 3 + 1];
            positionKeyValues[i].z = positionKeyFloatValues[i * 3 + 2];
        }
        for(int i = 0; i < rotationKeyCount; i++) {
            rotationKeyValues[i].w = rotationKeyFloatValues[i * 4 + 0];
            rotationKeyValues[i].x = rotationKeyFloatValues[i * 4 + 1];
            rotationKeyValues[i].y = rotationKeyFloatValues[i * 4 + 2];
            rotationKeyValues[i].z = rotationKeyFloatValues[i * 4 + 3];
        }
        for(int i = 0; i < scaleKeyCount; i++) {
            scaleKeyValues[i].x = scaleKeyFloatValues[i * 3 + 0];
            scaleKeyValues[i].y = scaleKeyFloatValues[i * 3 + 1];
            scaleKeyValues[i].z = scaleKeyFloatValues[i * 3 + 2];
        }
    }
};

#endif