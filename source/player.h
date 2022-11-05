#ifndef PLAYER_H
#define PLAYER_H

#include "graphics.h"
#include "gameObject.h"

#include "mtx_kuchinaka0_t3x.h"
#include "mtx_kuchinaka1_t3x.h"
#include "s_anakage1_t3x.h"
#include "s_hando2_t3x.h"
#include "s_hando3_t3x.h"
#include "s_testhand_t3x.h"
#include "stx_btest1_t3x.h"
#include "stx_eye2_t3x.h"
#include "stx_ha_t3x.h"
#include "stx_hara_t3x.h"
#include "stx_hoho_t3x.h"
#include "stx_itemring_t3x.h"
#include "stx_itemshoos0_t3x.h"
#include "stx_kanagu_t3x.h"
#include "stx_newspin_t3x.h"
#include "ym_sjppse_t3x.h"

class Player {
public:
    GameObject gameObject;

    Vec3 pos;
    Vec3 rot;
    Vec3 vel;

    Model sonicModel;
    C3D_Tex* textures;

    void init(Vec3 startPos, Vec3 startRot) {
        loadModel();
        loadTextures();

        gameObject.setTextures(textures);
	    gameObject.setAnimation(-1);
    }

    void loadTextures() {
        textures = new C3D_Tex[12];

        loadTextureFromMem(&textures[ 0], stx_btest1_t3x, stx_btest1_t3x_size);
        loadTextureFromMem(&textures[ 1], stx_hara_t3x, stx_hara_t3x_size);
        loadTextureFromMem(&textures[ 2], stx_eye2_t3x, stx_eye2_t3x_size);
        loadTextureFromMem(&textures[ 3], s_anakage1_t3x, s_anakage1_t3x_size);
        loadTextureFromMem(&textures[ 4], stx_hoho_t3x, stx_hoho_t3x_size);
        loadTextureFromMem(&textures[ 5], s_hando2_t3x, s_hando2_t3x_size);
        loadTextureFromMem(&textures[ 6], s_anakage1_t3x, s_anakage1_t3x_size);
        loadTextureFromMem(&textures[ 7], s_hando3_t3x, s_hando3_t3x_size);
        loadTextureFromMem(&textures[ 8], s_testhand_t3x, s_testhand_t3x_size);
        loadTextureFromMem(&textures[ 9], s_hando3_t3x, s_hando3_t3x_size);
        loadTextureFromMem(&textures[10], ym_sjppse_t3x, ym_sjppse_t3x_size);
        loadTextureFromMem(&textures[11], stx_kanagu_t3x, stx_kanagu_t3x_size);

        C3D_TexSetFilter(&textures[ 0], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 1], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 2], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 3], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 4], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 5], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 6], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 7], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 8], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[ 9], GPU_LINEAR, GPU_NEAREST);
        C3D_TexSetFilter(&textures[10], GPU_LINEAR, GPU_NEAREST);
    }
    void loadModel() {
        FILE* modelFile = fopen("romfs:/sonicModel.c3m", "rb");
        fseek(modelFile, 0, SEEK_END);
        long int modelSize = ftell(modelFile);
        rewind(modelFile);
        unsigned char* modelData = (unsigned char*)linearAlloc(modelSize);
        fread(modelData, sizeof(unsigned char), modelSize, modelFile);
        fclose(modelFile);

        sonicModel.loadModel(modelData, modelSize);
        gameObject.loadModel(sonicModel);
    }

    void update() {
        circlePosition leftStickPos;
		hidCircleRead(&leftStickPos);

		if(fabs(leftStickPos.dx) > 0.1f || fabs(leftStickPos.dy) > 0.1f) {
			vel.z = leftStickPos.dx / 2.0;
			vel.x = leftStickPos.dy / 2.0;

			rot.y = atan2(leftStickPos.dy, leftStickPos.dx) - M_PI / 2.0;

			gameObject.currentAnimation = 4;
		}
		else {
            gameObject.currentAnimation = -1;
            vel.x = 0;
            vel.z = 0;
        }

        pos.x += vel.x;
        pos.z += vel.z;

        gameObject.position.x = -pos.x;
		gameObject.position.z = -pos.z;

		gameObject.rotation.y = rot.y;
    }

    void render() {
        float speed = sqrtf(vel.x * vel.x + vel.z * vel.z);

        gameObject.updateAnimation(speed / 70.0f);
	    gameObject.draw();
    }
};

#endif