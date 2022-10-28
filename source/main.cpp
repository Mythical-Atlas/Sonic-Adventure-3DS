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

float angleX = 0.0, angleY = 0.0;

C3D_Tex* sonicTextures;

int sonicMeshCount;
GameObject* sonicMeshes;
Vertex** sonicVertices;
int* sonicVertexCounts;
int** sonicTextureIDs;

Vertex** sonicVertices0;
Vertex** sonicVertices1;
Vertex** sonicVertices2;
Vertex** sonicVertices3;
Vertex** sonicVertices4;
Vertex** sonicVertices5;
Vertex** sonicVertices6;
Vertex** sonicVertices7;
Vertex** sonicVertices8;
Vertex** sonicVertices9;
Vertex** sonicVertices10;
Vertex** sonicVertices11;
Vertex** sonicVertices12;
Vertex** sonicVertices13;
Vertex** sonicVertices14;
Vertex** sonicVertices15;
Vertex** sonicVertices16;
Vertex** sonicVertices17;
Vertex** sonicVertices18;
Vertex** sonicVertices19;
Vertex** sonicVertices20;
Vertex** sonicVertices21;
Vertex** sonicVertices22;
Vertex** sonicVertices23;
Vertex** sonicVertices24;
Vertex** sonicVertices25;
Vertex** sonicVertices26;
Vertex** sonicVertices27;
Vertex** sonicVertices28;
Vertex** sonicVertices29;
Vertex** sonicVertices30;
Vertex** sonicVertices31;

std::string inputfile0 = "romfs:/sonic_000000.obj";
std::string inputfile1 = "romfs:/sonic_000001.obj";
std::string inputfile2 = "romfs:/sonic_000002.obj";
std::string inputfile3 = "romfs:/sonic_000003.obj";
std::string inputfile4 = "romfs:/sonic_000004.obj";
std::string inputfile5 = "romfs:/sonic_000005.obj";
std::string inputfile6 = "romfs:/sonic_000006.obj";
std::string inputfile7 = "romfs:/sonic_000007.obj";
std::string inputfile8 = "romfs:/sonic_000008.obj";
std::string inputfile9 = "romfs:/sonic_000009.obj";
std::string inputfile10 = "romfs:/sonic_000010.obj";
std::string inputfile11 = "romfs:/sonic_000011.obj";
std::string inputfile12 = "romfs:/sonic_000012.obj";
std::string inputfile13 = "romfs:/sonic_000013.obj";
std::string inputfile14 = "romfs:/sonic_000014.obj";
std::string inputfile15 = "romfs:/sonic_000015.obj";
std::string inputfile16 = "romfs:/sonic_000016.obj";
std::string inputfile17 = "romfs:/sonic_000017.obj";
std::string inputfile18 = "romfs:/sonic_000018.obj";
std::string inputfile19 = "romfs:/sonic_000019.obj";
std::string inputfile20 = "romfs:/sonic_000020.obj";
std::string inputfile21 = "romfs:/sonic_000021.obj";
std::string inputfile22 = "romfs:/sonic_000022.obj";
std::string inputfile23 = "romfs:/sonic_000023.obj";
std::string inputfile24 = "romfs:/sonic_000024.obj";
std::string inputfile25 = "romfs:/sonic_000025.obj";
std::string inputfile26 = "romfs:/sonic_000026.obj";
std::string inputfile27 = "romfs:/sonic_000027.obj";
std::string inputfile28 = "romfs:/sonic_000028.obj";
std::string inputfile29 = "romfs:/sonic_000029.obj";
std::string inputfile30 = "romfs:/sonic_000030.obj";
std::string inputfile31 = "romfs:/sonic_000031.obj";

std::string inputfile = "romfs:/sonic.obj";
std::string mtlDir = "romfs:/";
tinyobj::attrib_t attrib;
std::vector<tinyobj::shape_t> shapes;
std::vector<tinyobj::material_t> materials;
std::string warn;
std::string err;

int frame;

void loadVerts(Vertex** verts) {
	for (int s = 0; s < sonicMeshCount; s++) {
		verts[s] = new Vertex[sonicVertexCounts[s]];

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

				verts[s][index_offset + v] = {{vx, vy, vz}, {tx, ty}, {nx, ny, nz}};
			}
			index_offset += fv;
		}
	}
}

void sceneInit() {
	romfsInit();

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str(), mtlDir.c_str(), true);

	sonicMeshCount = 28;
	sonicMeshes = (GameObject*)linearAlloc(sizeof(GameObject) * sonicMeshCount);
	sonicVertices = new Vertex*[sonicMeshCount];
	sonicVertexCounts = new int[sonicMeshCount];
	sonicTextureIDs = new int*[sonicMeshCount];

	for (int s = 0; s < sonicMeshCount; s++) {
		sonicVertexCounts[s] = shapes[s].mesh.num_face_vertices.size() * 3;
		sonicVertices[s] = new Vertex[sonicVertexCounts[s]];

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

				sonicVertices[s][index_offset + v] = {{vx, vy, vz}, {tx, ty}, {nx, ny, nz}};
			}
			index_offset += fv;
		}

		sonicTextureIDs[s] = new int[shapes[s].mesh.material_ids.size()];
		for(size_t f = 0; f < shapes[s].mesh.material_ids.size(); f++) {sonicTextureIDs[s][f] = shapes[s].mesh.material_ids[f];}
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

	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile0.c_str(), mtlDir.c_str(), true); sonicVertices0 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices0);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile1.c_str(), mtlDir.c_str(), true); sonicVertices1 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices1);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile2.c_str(), mtlDir.c_str(), true); sonicVertices2 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices2);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile3.c_str(), mtlDir.c_str(), true); sonicVertices3 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices3);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile4.c_str(), mtlDir.c_str(), true); sonicVertices4 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices4);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile5.c_str(), mtlDir.c_str(), true); sonicVertices5 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices5);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile6.c_str(), mtlDir.c_str(), true); sonicVertices6 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices6);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile7.c_str(), mtlDir.c_str(), true); sonicVertices7 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices7);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile8.c_str(), mtlDir.c_str(), true); sonicVertices8 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices8);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile9.c_str(), mtlDir.c_str(), true); sonicVertices9 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices9);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile10.c_str(), mtlDir.c_str(), true); sonicVertices10 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices10);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile11.c_str(), mtlDir.c_str(), true); sonicVertices11 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices11);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile12.c_str(), mtlDir.c_str(), true); sonicVertices12 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices12);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile13.c_str(), mtlDir.c_str(), true); sonicVertices13 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices13);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile14.c_str(), mtlDir.c_str(), true); sonicVertices14 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices14);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile15.c_str(), mtlDir.c_str(), true); sonicVertices15 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices15);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile16.c_str(), mtlDir.c_str(), true); sonicVertices16 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices16);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile17.c_str(), mtlDir.c_str(), true); sonicVertices17 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices17);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile18.c_str(), mtlDir.c_str(), true); sonicVertices18 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices18);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile19.c_str(), mtlDir.c_str(), true); sonicVertices19 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices19);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile20.c_str(), mtlDir.c_str(), true); sonicVertices20 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices20);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile21.c_str(), mtlDir.c_str(), true); sonicVertices21 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices21);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile22.c_str(), mtlDir.c_str(), true); sonicVertices22 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices22);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile23.c_str(), mtlDir.c_str(), true); sonicVertices23 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices23);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile24.c_str(), mtlDir.c_str(), true); sonicVertices24 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices24);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile25.c_str(), mtlDir.c_str(), true); sonicVertices25 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices25);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile26.c_str(), mtlDir.c_str(), true); sonicVertices26 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices26);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile27.c_str(), mtlDir.c_str(), true); sonicVertices27 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices27);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile28.c_str(), mtlDir.c_str(), true); sonicVertices28 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices28);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile29.c_str(), mtlDir.c_str(), true); sonicVertices29 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices29);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile30.c_str(), mtlDir.c_str(), true); sonicVertices30 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices30);
	tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile31.c_str(), mtlDir.c_str(), true); sonicVertices31 = new Vertex*[sonicMeshCount]; loadVerts(sonicVertices31);
}

void sceneRender(void) {
	C3D_Mtx modelView;
	Mtx_Identity(&modelView);
	Mtx_Translate(&modelView, 0.0, 0.0, -5.0 + 0.5*sinf(angleX), true);
	Mtx_RotateX(&modelView, angleX, true);
	Mtx_RotateY(&modelView, angleY, true);

    updateUniforms(&modelView);

	switch(frame) {
		case(0): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices0[s], sonicVertexCounts[s]);} break;
		case(1): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices1[s], sonicVertexCounts[s]);} break;
		case(2): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices2[s], sonicVertexCounts[s]);} break;
		case(3): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices3[s], sonicVertexCounts[s]);} break;
		case(4): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices4[s], sonicVertexCounts[s]);} break;
		case(5): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices5[s], sonicVertexCounts[s]);} break;
		case(6): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices6[s], sonicVertexCounts[s]);} break;
		case(7): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices7[s], sonicVertexCounts[s]);} break;
		case(8): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices8[s], sonicVertexCounts[s]);} break;
		case(9): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices9[s], sonicVertexCounts[s]);} break;
		case(10): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices10[s], sonicVertexCounts[s]);} break;
		case(11): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices11[s], sonicVertexCounts[s]);} break;
		case(12): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices12[s], sonicVertexCounts[s]);} break;
		case(13): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices13[s], sonicVertexCounts[s]);} break;
		case(14): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices14[s], sonicVertexCounts[s]);} break;
		case(15): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices15[s], sonicVertexCounts[s]);} break;
		case(16): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices16[s], sonicVertexCounts[s]);} break;
		case(17): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices17[s], sonicVertexCounts[s]);} break;
		case(18): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices18[s], sonicVertexCounts[s]);} break;
		case(19): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices19[s], sonicVertexCounts[s]);} break;
		case(20): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices20[s], sonicVertexCounts[s]);} break;
		case(21): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices21[s], sonicVertexCounts[s]);} break;
		case(22): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices22[s], sonicVertexCounts[s]);} break;
		case(23): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices23[s], sonicVertexCounts[s]);} break;
		case(24): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices24[s], sonicVertexCounts[s]);} break;
		case(25): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices25[s], sonicVertexCounts[s]);} break;
		case(26): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices26[s], sonicVertexCounts[s]);} break;
		case(27): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices27[s], sonicVertexCounts[s]);} break;
		case(28): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices28[s], sonicVertexCounts[s]);} break;
		case(29): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices29[s], sonicVertexCounts[s]);} break;
		case(30): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices30[s], sonicVertexCounts[s]);} break;
		case(31): for(int s = 0; s < sonicMeshCount; s++) {sonicMeshes[s].loadVertices(sonicVertices31[s], sonicVertexCounts[s]);} break;
	}

	frame++;
	if(frame == 32) {frame = 0;}

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
