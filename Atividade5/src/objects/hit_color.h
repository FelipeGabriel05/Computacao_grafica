#ifndef HIT_COLOR_H
#define HIT_COLOR_H
#include "../material/structs.h"
#include "../ray/ray.h"
#include "../colors/color.h"
#include "cilindro.h"
#include "cone.h"
#include "plano.h"
#include "esfera.h"

bool shadowRay(const point3 Pi, const Luz& luz, 
               const Esfera& esf, const Cilindro& ci, const Cone& co){
    vec3 l = unit_vector(luz.P_F - Pi);
    ray shadow_ray(Pi + 0.001 * l, l);
    
    double dist_luz = length(luz.P_F - Pi);

    double t_s = hit_sphere(esf, shadow_ray);
    if (t_s > 0 && t_s < dist_luz)
        return true;

    double t_ci = hit_cylinder(ci, shadow_ray);
    if (t_ci > 0 && t_ci < dist_luz)
        return true;

    double t_co = hit_Cone(co, shadow_ray);
    if (t_co > 0 && t_co < dist_luz)
        return true;

    return false;
}

color ray_color(const ray& r, const Esfera& esf, const Plano& plano_chao, const Plano& plano_fundo,
                const Luz& luz,const point3& E, color bgColor, const Cilindro& ci, const Cone& co
            ){
    // retorna o escalar
    double t_max = 1e30;
    double closest_t = t_max;
    color I = bgColor;

    double t_s = hit_sphere(esf, r);
    if(t_s > 0 && t_s < closest_t) {
        closest_t = t_s;
        point3 PI = r.origin() + t_s*r.direction();
        vec3 n = unit_vector(PI - esf.center);
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, luz, esf, ci, co)) {
            I = esf.mat_esfera.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, n, v, esf.mat_esfera, luz);
        }
    }

    double t_ch = Cplano(plano_chao, r);
    if(t_ch > 0 && t_ch < closest_t) {
        closest_t = t_ch;
        point3 PI = r.origin() + t_ch*r.direction();
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, luz, esf, ci, co)) {
            I = plano_chao.mat_plano.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, plano_chao.normal, v, plano_chao.mat_plano, luz);
        }
    }

    double t_f = Cplano(plano_fundo, r);
    if(t_f > 0 && t_f < closest_t) {
        closest_t = t_f;
        point3 PI = r.origin() + t_f*r.direction();
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, luz, esf, ci, co)) {
            I = plano_fundo.mat_plano.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, plano_fundo.normal, v, plano_fundo.mat_plano, luz);
        }
    }

    double t_ci = hit_cylinder(ci, r);
    if(t_ci > 0 && t_ci < closest_t) {
        closest_t = t_ci;
        point3 PI = r.origin() + t_ci*r.direction();
        vec3 proj = dot(PI - ci.centerB, ci.direcao) * ci.direcao;
        vec3 n = unit_vector((PI - ci.centerB) - proj);
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, luz, esf, ci, co)) {
            I = ci.mat_cilindro.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, n, v, ci.mat_cilindro, luz);
        }
    }

    double t_co = hit_Cone(co, r);
    if(t_co > 0 && t_co < closest_t) {
        closest_t = t_co;
        point3 PI = r.origin() + t_co *r.direction();
        vec3 v = unit_vector(E - PI);

        point3 vertice = co.centerB + co.height * co.direcao;
        double cos_theta2 = (co.height * co.height) / (co.height * co.height + co.radius * co.radius);
        vec3 v_p = PI - vertice;

        double m = dot(v_p, co.direcao);
        vec3 n = unit_vector(m * co.direcao - v_p * cos_theta2);
    
        if (dot(r.direction(), n) > 0) {
            n = -n;
        }

        if(shadowRay(PI, luz, esf, ci, co)) {
            I = co.mat_cone.Kamb * luz.I_A;
        }else {
            I = calculo_cor(PI, n, v, co.mat_cone, luz);
        }
    }
    return I;
}

#endif