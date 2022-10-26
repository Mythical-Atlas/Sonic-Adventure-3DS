#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>
#include "vshader_shbin.h"
#include "kitten_t3x.h"
#include "eye_t3x.h"
#include "teapot.h"
#include "graphics.h"

using namespace std;

C3D_Tex kitten_tex;
C3D_Tex eyeTexture;
float angleX = 0.0, angleY = 0.0;

vertex teapot1VertexList[teapot_count / 3];
vertex teapot2VertexList[teapot_count / 3];

void* vbo1Data;
void* vbo2Data;
C3D_BufInfo buf1Info;
C3D_BufInfo buf2Info;

void sceneInit() {
	initGraphics();

	for(int v = 0; v < teapot_count / 9; v++) {
		float vert0[3] = {teapot[v * 9 + 0], teapot[v * 9 + 1], teapot[v * 9 + 2]};
		float vert1[3] = {teapot[v * 9 + 3], teapot[v * 9 + 4], teapot[v * 9 + 5]};
		float vert2[3] = {teapot[v * 9 + 6], teapot[v * 9 + 7], teapot[v * 9 + 8]};

		float normX = calculateNormalX(vert0, vert1, vert2);
		float normY = calculateNormalY(vert0, vert1, vert2);
		float normZ = calculateNormalZ(vert0, vert1, vert2);

		teapot1VertexList[v * 3 + 0] = {{vert0[0], vert0[1], vert0[2]}, {0.0f, 0.0f}, {normX, normY, normZ}};
		teapot1VertexList[v * 3 + 1] = {{vert1[0], vert1[1], vert1[2]}, {1.0f, 1.0f}, {normX, normY, normZ}};
		teapot1VertexList[v * 3 + 2] = {{vert2[0], vert2[1], vert2[2]}, {0.0f, 1.0f}, {normX, normY, normZ}};

		teapot2VertexList[v * 3 + 0] = {{vert0[0] + 2, vert0[1], vert0[2]}, {0.0f, 0.0f}, {normX, normY, normZ}};
		teapot2VertexList[v * 3 + 1] = {{vert1[0] + 2, vert1[1], vert1[2]}, {1.0f, 1.0f}, {normX, normY, normZ}};
		teapot2VertexList[v * 3 + 2] = {{vert2[0] + 2, vert2[1], vert2[2]}, {0.0f, 1.0f}, {normX, normY, normZ}};
	}

	Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);

	vbo1Data = linearAlloc(sizeof(teapot1VertexList));
	BufInfo_Init(&buf1Info);
	BufInfo_Add(&buf1Info, vbo1Data, sizeof(vertex), 3, 0x210);

	vbo2Data = linearAlloc(sizeof(teapot1VertexList));
	BufInfo_Init(&buf2Info);
	BufInfo_Add(&buf2Info, vbo2Data, sizeof(vertex), 3, 0x210);

	if(!loadTextureFromMem(&kitten_tex, kitten_t3x, kitten_t3x_size)) {svcBreak(USERBREAK_PANIC);}
	C3D_TexSetFilter(&kitten_tex, GPU_LINEAR, GPU_NEAREST);

	if(!loadTextureFromMem(&eyeTexture, eye_t3x, eye_t3x_size)) {svcBreak(USERBREAK_PANIC);}
	C3D_TexSetFilter(&eyeTexture, GPU_LINEAR, GPU_NEAREST);
}

static void sceneRender(void) {
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -2.0 + 0.5*sinf(angleX), true);
	Mtx_RotateX(&modelView, angleX, true);
	Mtx_RotateY(&modelView, angleY, true);

	angleX += M_PI / 180;
	angleY += M_PI / 360;

    updateUniforms(&modelView);

	C3D_SetBufInfo(&buf1Info);
	memcpy(vbo1Data, teapot1VertexList, sizeof(teapot1VertexList));

	C3D_TexBind(0, &kitten_tex);
	C3D_DrawArrays(GPU_TRIANGLES, 0, teapot_count / 3 / 3 * 2);

	C3D_TexBind(0, &eyeTexture);
	C3D_DrawArrays(GPU_TRIANGLES, teapot_count / 3 / 3 * 2, teapot_count / 3 / 3);

	C3D_SetBufInfo(&buf2Info);
	memcpy(vbo2Data, teapot2VertexList, sizeof(teapot2VertexList));

	C3D_TexBind(0, &kitten_tex);
	C3D_DrawArrays(GPU_TRIANGLES, 0, teapot_count / 3 / 3 * 2);

	C3D_TexBind(0, &eyeTexture);
	C3D_DrawArrays(GPU_TRIANGLES, teapot_count / 3 / 3 * 2, teapot_count / 3 / 3);
}

static void sceneExit(void) {
	C3D_TexDelete(&kitten_tex);
	C3D_TexDelete(&eyeTexture);

	linearFree(vbo1Data);
	linearFree(vbo2Data);

	shaderProgramFree(&shaderProgram);
	DVLB_Free(vshader_dvlb);
}

int main() {
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	sceneInit();

	while (aptMainLoop()) {
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW); // used to be C3D_FRAME_SYNCDRAW instead of 0
			C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(target);
			sceneRender();
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	sceneExit();

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
