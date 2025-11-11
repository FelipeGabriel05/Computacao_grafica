#ifndef CONE_H
#define CONE_H
#include "../ray/ray.h"
#include "../material/structs.h"

double hit_Cone(const Cone& co, const ray& r, bool tampa = false) {
    point3 vertice = co.centerB + co.height * co.direcao;
    vec3 v = r.origin() - vertice;
    double cos_theta2 = (co.height*co.height) / (co.height*co.height + co.radius*co.radius);

    auto w = dot(r.direction(), co.direcao);
    auto m = dot(v, co.direcao);

    auto a = w*w - dot(r.direction(), r.direction()) * cos_theta2;
    auto b = 2.0f * (m * w - dot(v, r.direction()) * cos_theta2);    
    auto c = m * m - dot(v, v) * cos_theta2;
    auto discriminante = b*b -  4 * a * c;
    double t_hit = -1.0;
    if(discriminante < 0){
        return t_hit;
    } else {
        const double EPS = 1e-12;
        if (std::fabs(a) < EPS) return -1.0;

        double t1 = (-b - std::sqrt(discriminante)) / (2*a);
        double t2 = (-b + std::sqrt(discriminante)) / (2*a);

        if (t1 > 0 && t2 > 0)
            t_hit = std::min(t1, t2);     // os dois válidos → pega o mais próximo
        else if (t1 > 0)
            t_hit = t1;
        else if (t2 > 0)
            t_hit = t2;  
    }

    point3 P = r.origin() + t_hit * r.direction();
    double h = dot(P - co.centerB, co.direcao); // projeção sobre o eixo
    if (h < 0.0 || h > co.height) t_hit = -1.0;

    if (tampa) {
        // Interseção com a base do cone
        double denom = dot(co.direcao, r.direction());
        if (fabs(denom) > 1e-6) {
            double t_base = dot(co.centerB - r.origin(), co.direcao) / denom;
            if (t_base > 0) {
                point3 P_base = r.origin() + t_base * r.direction();
                if (length(P_base - co.centerB) <= co.radius) {
                    if (t_hit < 0 || t_base < t_hit)
                        t_hit = t_base;
                }
            }
        }
    }
    return t_hit;
}

#endif