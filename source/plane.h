#ifndef PLANE_H
#define PLANE_H

#include "mesh.h"
#include "graphics.h"

class Plane {
public:
    Mesh mesh;
    C3D_Tex* texture;
    
    Mat4 transform;

    void init(Mat4 transform) {
        this->transform = transform;
        
        Vec3 vps[6];
        vps[0].x =  1; vps[0].y = 0; vps[0].z = -1; // tr
        vps[1].x = -1; vps[1].y = 0; vps[1].z = -1; // tl
        vps[2].x = -1; vps[2].y = 0; vps[2].z =  1; // bl

        vps[3].x =  1; vps[3].y = 0; vps[3].z = -1; // tr
        vps[4].x = -1; vps[4].y = 0; vps[4].z =  1; // bl
        vps[5].x =  1; vps[5].y = 0; vps[5].z =  1; // br

        Vec2 vuvs[6];
        vuvs[0].x = 1; vuvs[0].y = 0;
        vuvs[1].x = 0; vuvs[1].y = 0;
        vuvs[2].x = 0; vuvs[2].y = 1;

        vuvs[3].x = 1; vuvs[3].y = 0;
        vuvs[4].x = 0; vuvs[4].y = 1;
        vuvs[5].x = 1; vuvs[5].y = 1;

        Vec3 vms[6];

        mesh.loadMesh(
            6,
            vps,
            vuvs,
            vms,
            0
        );
        mesh.initVBO();
    }

    void setTexture(C3D_Tex* texture) {this->texture = texture;}

    void draw() {
        mesh.updateVertData(transform);
        mesh.draw(texture);
    }
};

#endif