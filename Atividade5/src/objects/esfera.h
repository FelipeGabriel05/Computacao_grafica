#ifndef ESFERA_H
#define ESFERA_H
#include "../material/structs.h"
#include "../ray/ray.h"

double hit_sphere(const Esfera& esf,const ray& r) {
    vec3 w = r.origin() - esf.center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(w, r.direction());
    auto c = dot(w, w) - esf.radius*esf.radius;
    auto discriminante = b*b - 4*a*c;
    if (discriminante < 0) return -1.0;
    return (-b - std::sqrt(discriminante)) / (2.0*a);
}

#endif