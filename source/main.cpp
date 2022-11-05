#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>

#include "model.h"

#include "testGround_t3x.h"

#include "vshader_shbin.h"
#include "graphics.h"
#include "gameObject.h"
#include "plane.h"
#include "player.h"

C3D_Tex groundTexture;

GameObject sonicObject;
Plane groundObj;

Player sonic;

void sceneInit() {
	romfsInit();

	sonic.init(getVec3(0, 0, 0), getVec3(0, 0, 0));
	groundObj.init(getTranslationScaleMat4(getVec3(0, -535, 0), getVec3(10000, 1, 10000)));

	initGraphics();

	Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(70.0f), C3D_AspectRatioTop, 0.01f, 100000.0f, false);

	loadTextureFromMem(&groundTexture, testGround_t3x, testGround_t3x_size);
	C3D_TexSetFilter(&groundTexture, GPU_LINEAR, GPU_NEAREST);

	groundObj.setTexture(&groundTexture);
}

void sceneRender(void) {
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);

	Mtx_RotateX(&modelView, M_PI / 8, true);
	Mtx_RotateY(&modelView, -M_PI / 2, true);

	Mtx_Translate(&modelView, -2000, -1000, 0, true);
	Mtx_Translate(&modelView, sonic.pos.x, sonic.pos.y, sonic.pos.z, true);

    updateUniforms(&modelView);

	groundObj.draw();

	sonic.render();
}

void sceneExit() {
	/*C3D_TexDelete(&kitten_tex);
	C3D_TexDelete(&eyeTexture);*/

	//teapot1.free();
	//linearFree(vbo1Data);
	//linearFree(vbo2Data);

	shaderProgramFree(&shaderProgram);
	DVLB_Free(vshader_dvlb);
}

int main() {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	PrintConsole bottomScreen;
	consoleInit(GFX_BOTTOM, &bottomScreen);
	consoleSelect(&bottomScreen);

	sceneInit();

	progStartTime = osGetTime();
	while (aptMainLoop()) {
		hidScanInput();

		if(hidKeysDown() & KEY_START) {break;}

		sonic.update();

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW); // can be 0
		C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
		C3D_FrameDrawOn(target);
		sceneRender();
		C3D_FrameEnd(0);
	}

	sceneExit();

	C3D_Fini();
	gfxExit();
	return 0;
}
