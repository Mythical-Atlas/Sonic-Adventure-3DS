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
        Vec3* positionKeyValues,
        int rotationKeyCount,
        double* rotationKeyTimes,
        Quat* rotationKeyValues,
        int scaleKeyCount,
        double* scaleKeyTimes,
        Vec3* scaleKeyValues
    ) {
        this->nodeNameLength = nodeNameLength;
        this->nodeName = nodeName;
        this->positionKeyCount = positionKeyCount;
        this->positionKeyTimes = positionKeyTimes;
        this->positionKeyValues = positionKeyValues;
        this->rotationKeyCount = rotationKeyCount;
        this->rotationKeyTimes = rotationKeyTimes;
        this->rotationKeyValues = rotationKeyValues;
        this->scaleKeyCount = scaleKeyCount;
        this->scaleKeyTimes = scaleKeyTimes;
        this->scaleKeyValues = scaleKeyValues;
    }
};

#endif