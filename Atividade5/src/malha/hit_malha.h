#ifndef HIT_MALHA
#define HIT_MALHA
#include "list_mesh.h"
#include "../ray/ray.h"
#include "../objects/hit_triangulo.h"

double hit_malha(const ray& r, const ListMesh& L, int& face_hit) {
    double t_min = 1e30;
    face_hit = -1;

    for(int i = 0; i < L.faces().size(); i++) {
        double t = hit_triangulo(r, L, i);
        if(t > 0 && t < t_min) {
            t_min = t;
            face_hit = i;
        }
    }

    if(face_hit == -1) {
        return -1.0;
    }
    return t_min;
}
#endif