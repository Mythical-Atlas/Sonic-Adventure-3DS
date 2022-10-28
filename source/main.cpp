#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>
#include <string.h>

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

#include "vshader_shbin.h"
#include "teapot.h"
#include "graphics.h"
#include "gameObject.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//using namespace std;

float angleX = 0.0, angleY = 0.0;

C3D_Tex* sonicTextures;

int sonicMeshCount;
GameObject* sonicMeshes;
Vertex** sonicVertices;
int* sonicVertexCounts;
int** sonicTextureIDs;

std::string inputfile = "romfs:/sonic.obj";
std::string mtlDir = "romfs:/";
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
std::string warn;
std::string err;

/*tinyobj::ObjReaderConfig reader_config;
tinyobj::ObjReader reader;*/

void sceneInit() {
	romfsInit();

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), mtlDir.c_str(), true);

	int meshIndex = 0;
	sonicMeshCount = 28;
	sonicMeshes = (GameObject*)linearAlloc(sizeof(GameObject) * sonicMeshCount);
	sonicVertices = new Vertex*[sonicMeshCount];
	sonicVertexCounts = new int[sonicMeshCount];
	sonicTextureIDs = new int*[sonicMeshCount];

	for (int s = 0; s < sonicMeshCount; s++) {
		//if(shapes[s].name.at(0) == 'M') {
			sonicVertexCounts[meshIndex] = shapes[s].mesh.num_face_vertices.size() * 3;
			sonicVertices[meshIndex] = new Vertex[sonicVertexCounts[meshIndex]];

			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				for (size_t v = 0; v < fv; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

					float vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
					float vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
					float vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

					float nx = 0;
					float ny = 0;
					float nz = 0;
					float tx = 0;
					float ty = 0;

					if (idx.normal_index >= 0) {
						nx = attrib.normals[3*size_t(idx.normal_index)+0];
						ny = attrib.normals[3*size_t(idx.normal_index)+1];
						nz = attrib.normals[3*size_t(idx.normal_index)+2];
					}

					if (idx.texcoord_index >= 0) {
						tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
						ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
					}

					sonicVertices[meshIndex][index_offset + v] = {{vx, vy, vz}, {tx, ty}, {nx, ny, nz}};
				}
				index_offset += fv;
			}

			sonicTextureIDs[meshIndex] = new int[shapes[s].mesh.material_ids.size()];
			for(size_t f = 0; f < shapes[s].mesh.material_ids.size(); f++) {sonicTextureIDs[meshIndex][f] = shapes[s].mesh.material_ids[f];}

			meshIndex++;
		//}
	}

	initGraphics();

	Mtx_PerspTilt(&projection, C3D_AngleFromDegrees(80.0f), C3D_AspectRatioTop, 0.01f, 1000.0f, false);

	sonicTextures = new C3D_Tex[16];

	loadTextureFromMem(&sonicTextures[ 0], stx_btest1_t3x, stx_btest1_t3x_size);
	loadTextureFromMem(&sonicTextures[ 1], stx_hara_t3x, stx_hara_t3x_size);
	loadTextureFromMem(&sonicTextures[ 2], ym_sjppse_t3x, ym_sjppse_t3x_size);
	loadTextureFromMem(&sonicTextures[ 3], stx_kanagu_t3x, stx_kanagu_t3x_size);
	loadTextureFromMem(&sonicTextures[ 4], stx_eye2_t3x, stx_eye2_t3x_size);
	loadTextureFromMem(&sonicTextures[ 5], s_anakage1_t3x, s_anakage1_t3x_size);
	loadTextureFromMem(&sonicTextures[ 6], stx_hoho_t3x, stx_hoho_t3x_size);
	loadTextureFromMem(&sonicTextures[ 7], s_hando2_t3x, s_hando2_t3x_size);
	loadTextureFromMem(&sonicTextures[ 8], s_anakage1_t3x, s_anakage1_t3x_size);
	loadTextureFromMem(&sonicTextures[ 9], s_hando3_t3x, s_hando3_t3x_size); // dupe?
	loadTextureFromMem(&sonicTextures[10], s_testhand_t3x, s_testhand_t3x_size);
	loadTextureFromMem(&sonicTextures[11], s_hando3_t3x, s_hando3_t3x_size); // dupe?

	// unused:
	//loadTextureFromMem(&sonicTextures[12], mtx_kuchinaka0_t3x, mtx_kuchinaka0_t3x_size);
	//loadTextureFromMem(&sonicTextures[13], mtx_kuchinaka1_t3x, mtx_kuchinaka1_t3x_size);
	//loadTextureFromMem(&sonicTextures[ 7], stx_ha_t3x, stx_ha_t3x_size);
	//loadTextureFromMem(&sonicTextures[11], stx_itemring_t3x, stx_itemring_t3x_size);
	//loadTextureFromMem(&sonicTextures[14], stx_newspin_t3x, stx_newspin_t3x_size);
	//loadTextureFromMem(&sonicTextures[15], stx_itemshoos0_t3x, stx_itemshoos0_t3x_size);

	C3D_TexSetFilter(&sonicTextures[ 0], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 1], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 2], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 3], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 4], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 5], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 6], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 7], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 8], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[ 9], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[10], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[11], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[12], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[13], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[14], GPU_LINEAR, GPU_NEAREST);
	C3D_TexSetFilter(&sonicTextures[15], GPU_LINEAR, GPU_NEAREST);

	printf("\x1b[0;0HsonicMeshCount: %i", sonicMeshCount);
	for (int s = 0; s < sonicMeshCount; s++) {
		sonicMeshes[s].loadVertices(sonicVertices[s], sonicVertexCounts[s]);
		sonicMeshes[s].setTextures(sonicTextures, 16, sonicTextureIDs[s]);
		sonicMeshes[s].initialize(GetVector3(new float[]{0, 0, 0}), GetVector3(new float[]{0, 0, 0}), GetVector3(new float[]{0, 0, 0}));
	}
}

void sceneRender(void) {
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -5.0 + 0.5*sinf(angleX), true);
	Mtx_RotateX(&modelView, angleX, true);
	Mtx_RotateY(&modelView, angleY, true);

    updateUniforms(&modelView);

	for(int s = 0; s < sonicMeshCount; s++) {
		sonicMeshes[s].updateVertices();
		sonicMeshes[s].draw();
	}
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

	while (aptMainLoop()) {
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		if(hidKeysHeld() & KEY_LEFT) {angleY -= M_PI / 180;}
		if(hidKeysHeld() & KEY_RIGHT) {angleY += M_PI / 180;}
		if(hidKeysHeld() & KEY_UP) {angleX -= M_PI / 180;}
		if(hidKeysHeld() & KEY_DOWN) {angleX += M_PI / 180;}

	//angleY += M_PI / 360;}

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW); // can be 0
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
