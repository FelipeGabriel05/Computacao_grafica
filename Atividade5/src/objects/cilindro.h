#ifndef CILINDRO_H
#define CILINDRO_H
#include "../material/structs.h"
#include "../ray/ray.h"

double hit_cylinder(const Cilindro&  ci, const ray& r) {
    vec3 v = (r.origin() -  ci.centerB) - dot((r.origin() -  ci.centerB), ci.direcao)*ci.direcao;
    vec3 w = r.direction() - dot(r.direction(), ci.direcao) *  ci.direcao;
    auto a = dot(w, w);
    auto b = 2.0 * dot(v, w);
    auto c = dot(v, v) - ci.radius*ci.radius;
    auto discriminante = b*b - 4 * a * c;
    double t_hit = -1.0;
    if(discriminante < 0) {
        return -1.0; 
    } else {
        double t1 = (-b - std::sqrt(discriminante)) / (2.0*a);
        double t2 = (-b + std::sqrt(discriminante)) / (2.0*a);
        if (t1 > 0 && t2 > 0)
            t_hit = std::min(t1, t2);     // os dois válidos → pega o mais próximo
        else if (t1 > 0)
            t_hit = t1;
        else if (t2 > 0)
            t_hit = t2;  
    }
    // Verifica se o ponto está dentro da altura do cilindro
    point3 P = r.origin() + t_hit * r.direction();
    double h = dot(P - ci.centerB, ci.direcao); // projeção sobre o eixo

    if (h < 0 || h > ci.height) return -1.0; // fora dos limites

    return t_hit;
}


#endif