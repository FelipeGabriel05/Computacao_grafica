#ifndef HIT_TRIANGULO
#define HIT_TRIANGULO
#include "../vectors/vec3.h"
#include "../material/structs.h"
#include "../malha/cube_mesh.h"
#include "../ray/ray.h"
#include <vector>

double hit_triangulo(const ray& r, const ListMesh& L, int faceIndex) {
    const LstFace& f = L.faces()[faceIndex];
    int i0 = f.verts()[0];
    int i1 = f.verts()[1];
    int i2 = f.verts()[2];

    vec3 P1 = L.vertices()[i0].pos();
    vec3 P2 = L.vertices()[i1].pos();
    vec3 P3 = L.vertices()[i2].pos();
    
    vec3 r1 = P2 - P1;
    vec3 r2 = P3 - P1;
    vec3 normal = unit_vector(cross(r1, r2));

    double denom = dot(r.direction(), normal);  
    if (fabs(denom) < 1e-8) return -1.0;

    vec3 w = r.origin() - P1;
    double t = -(dot(w, normal)/denom);
    if(t < 0) {
        return -1.0;
    }

    vec3 PI = r.origin() + t * r.direction();
    vec3 s1 = P1 - PI;
    vec3 s2 = P2 - PI;
    vec3 s3 = P3 - PI;  
    
    double area = dot(cross(r1, r2), normal);
    if(fabs(area) < 1e-10) {
        return -1.0;
    }
    double C1 = dot(cross(s3, s1), normal) / area;
    if(C1 < 0) {
        return -1.0;
    }
    double C2 = dot(cross(s1, s2), normal) / area;
    if(C2 < 0) {
        return -1.0;
    }   
    double C3 = 1 - C1 - C2;
    if(C3 < 0) {
        return -1.0;
    }
    return t;
}

#endif