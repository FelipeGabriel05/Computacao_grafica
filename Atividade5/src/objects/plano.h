#ifndef PLANO_H
#define PLANO_H
#include "../ray/ray.h"
#include "../material/structs.h"

double Cplano(const Plano pl, const ray& r) {
    vec3 w = r.origin() - pl.point;
    double s = -(dot(w, pl.normal));
    double t_i = s/(dot(r.direction(), pl.normal));
    if(t_i > 0.0 ) return t_i;                          // interseção válida 
    return -1.0;                                        // sem interserção visível 
}

#endif