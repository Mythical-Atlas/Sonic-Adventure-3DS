#include "graphics.h"

#ifndef ANIM_H
#define ANIM_H

class Anim {
public:
	int nameLength;
	const char* name;

	double duration;
	double ticksPerSecond;
	int channelCount;
	int* channelIndices;

    void loadAnim(
        int nameLength,
        const char* name,
        double duration,
        double ticksPerSecond,
        int channelCount,
        int* channelIndices
    ) {
        this->nameLength = nameLength;
        this->name = name;
        this->duration = duration;
        this->ticksPerSecond = ticksPerSecond;
        this->channelCount = channelCount;
        this->channelIndices = channelIndices;
    }
};

#endif