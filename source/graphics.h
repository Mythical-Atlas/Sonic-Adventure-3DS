/*
VBO notes to keep in mind:
- a VBO cannot be rewritten mid-frame
- multiples VBOs can be used in one frame
- one big VBO can be used for a frame
- VBOs shouldn't really be reallocated often (slow)
    - best practice is to allocate exactly as much as you need per VBO at the beginning of a scene, and you don't have to use all of it
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>
#include "vshader_shbin.h"

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

typedef struct {
    float position[3];
    float texcoord[2];
    float normal[3];
} Vertex;

typedef struct {
    float x;
    float y;
    float z;
} Vector3;

Vector3 GetVector3(float input[3]) {
    Vector3 output;
    output.x = input[0];
    output.y = input[1];
    output.z = input[2];
    return output;
}

DVLB_s* vshader_dvlb;
shaderProgram_s shaderProgram;
int uLoc_projection, uLoc_modelView;
int uLoc_lightVec, uLoc_lightHalfVec, uLoc_lightClr, uLoc_material;
C3D_Mtx projection;
C3D_Mtx material = {{
	{{0.0f, 0.2f, 0.2f, 0.2f}}, // ambient
	{{0.0f, 0.4f, 0.4f, 0.4f}}, // diffuse
	{{0.0f, 0.8f, 0.8f, 0.8f}}, // specular
	{{1.0f, 0.0f, 0.0f, 0.0f}}, // emission
}};

bool loadTextureFromMem(C3D_Tex* tex, const void* data, size_t size) {
	Tex3DS_Texture t3x = Tex3DS_TextureImport(data, size, tex, NULL, false);
	if(!t3x) {return false;}

	Tex3DS_TextureFree(t3x);

	return true;
}

float lineX(float vert1[3], float vert0[3]) {return vert1[0] - vert0[0];}
float lineY(float vert1[3], float vert0[3]) {return vert1[1] - vert0[1];}
float lineZ(float vert1[3], float vert0[3]) {return vert1[2] - vert0[2];}

float calculateNormalX(float vert0[3], float vert1[3], float vert2[3]) {return lineY(vert1, vert0) * lineZ(vert2, vert0) - lineZ(vert1, vert0) * lineY(vert2, vert0);}
float calculateNormalY(float vert0[3], float vert1[3], float vert2[3]) {return lineZ(vert1, vert0) * lineX(vert2, vert0) - lineX(vert1, vert0) * lineZ(vert2, vert0);}
float calculateNormalZ(float vert0[3], float vert1[3], float vert2[3]) {return lineX(vert1, vert0) * lineY(vert2, vert0) - lineY(vert1, vert0) * lineX(vert2, vert0);}

void initGraphics() {
	vshader_dvlb = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);
	shaderProgramInit(&shaderProgram);
	shaderProgramSetVsh(&shaderProgram, &vshader_dvlb->DVLE[0]);
	C3D_BindProgram(&shaderProgram);

	uLoc_projection   = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "projection");
	uLoc_modelView    = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "modelView");
	uLoc_lightVec     = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "lightVec");
	uLoc_lightHalfVec = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "lightHalfVec");
	uLoc_lightClr     = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "lightClr");
	uLoc_material     = shaderInstanceGetUniformLocation(shaderProgram.vertexShader, "material");

	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1=texcoord
	AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // v2=normal

	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);
}

void updateUniforms(C3D_Mtx* modelView) {
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_projection, &projection);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_modelView,  modelView);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLoc_material,   &material);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightVec,     0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightHalfVec, 0.0f, 0.0f, -1.0f, 0.0f);
	C3D_FVUnifSet(GPU_VERTEX_SHADER, uLoc_lightClr,     1.0f, 1.0f,  1.0f, 1.0f);
}

#endif