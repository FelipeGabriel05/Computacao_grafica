#ifndef COLOR_H
#define COLOR_H
#include "../vectors/vec3.h"
#include "../material/structs.h"

using color = vec3;
color calculo_cor(const point3 Pi, const vec3 n, 
                    const vec3 v, const Material& mat, const Luz& luz) {
    
    vec3 l = unit_vector(luz.P_F - Pi);                 // Vetor da superfície até a luz
    vec3 r = unit_vector(2 * dot(l, n) * n - l);

    // Reflexão difusa 
    double ln = std::max(0.0, dot(l, n));
    color I_d = luz.I_F * mat.Kdif * ln;

    // Reflexão especular: (I_F @ K) * (v . r)^m
    double vr = std::max(0.0, dot(v, r));
    color I_e = luz.I_F * mat.Kesp * pow(vr, mat.m);

    color I_amb = mat.Kamb * luz.I_A; 
 
    // Soma total da iluminação 
    color I = I_amb + I_d + I_e;
    return color(std::min(I.x(),1.0), std::min(I.y(),1.0), std::min(I.z(),1.0));
}

#endif