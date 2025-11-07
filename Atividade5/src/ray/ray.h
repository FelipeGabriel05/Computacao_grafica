#ifndef RAY_H
#define RAY_H
#include "../vectors/vec3.h"


// P(t) = origem + t.direção
class ray { 
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

private:
    point3 orig;
    vec3 dir;
};

#endif