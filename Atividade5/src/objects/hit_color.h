#ifndef HIT_COLOR_H
#define HIT_COLOR_H
#include "../material/structs.h"
#include "../material/plano_cenario.h"
#include "../ray/ray.h"
#include "../colors/color.h"
#include "hit_plano.h"
#include "cilindro.h"
#include "cone.h"
#include "plano.h"
#include "esfera.h"
#include "../malha/list_mesh.h"
#include "../malha/cube_mesh.h"
#include "../malha/hit_malha.h"
#include "hit_triangulo.h"

bool shadowRay(const point3 Pi, const Luz& luz, 
               const Esfera& esf, const Cilindro& ci, const Cone& co){
    vec3 l = unit_vector(luz.P_F - Pi);
    double dist_luz = length(luz.P_F - Pi);
    ray shadow_ray(Pi + 0.01 * l, l);
    

    double t_s = hit_sphere(esf, shadow_ray);
    if (t_s > 1e-4 && t_s < dist_luz - 1e-4)
        return true;

    double t_ci = hit_cylinder(ci, shadow_ray);
    if (t_ci > 1e-4 && t_ci < dist_luz - 1e-4)
        return true;

    double t_co = hit_Cone(co, shadow_ray, true);
    if (t_co > 1e-4 && t_co < dist_luz - 1e-4)
        return true;

    return false;
}

color ray_color(const ray& r, const Esfera& esf, const Cilindro& ci, const Cone& co, const Cubo& cube, const ListMesh& L,
                const Plano_cenario& planos, const Luz& luz, const point3& E 
    ){
    // retorna o escalar
    double t_max = 1e30;
    double closest_t = t_max;
    color I;

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

    // recebe um dos 5 planos e já retorna o menor deles
    Hit_plano t_planos = hit_plano(closest_t, planos, r);
    if(t_planos.t > 0 && t_planos.t != closest_t) {
        closest_t = t_planos.t;
        point3 PI = r.origin() + t_planos.t*r.direction();
        vec3 v = unit_vector(E - PI);
        const Plano* plano_escolhido = t_planos.plano; 
        if(shadowRay(PI, luz, esf, ci, co)) {
            I = plano_escolhido->mat_plano.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, plano_escolhido->normal, v, plano_escolhido->mat_plano, luz);
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

    double t_co = hit_Cone(co, r, true);
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

    int id_face;
    double t_cube = hit_malha(r, L, id_face);
    if(t_cube > 0 && t_cube < closest_t) {
        closest_t = t_cube;
        point3 PI = r.origin() + t_cube * r.direction();
        const LstFace& f = L.faces()[id_face];
        int i0 = f.verts()[0];
        int i1 = f.verts()[1];
        int i2 = f.verts()[2];

        vec3 P1 = L.vertices()[i0].pos();
        vec3 P2 = L.vertices()[i1].pos();
        vec3 P3 = L.vertices()[i2].pos();

        vec3 normal = unit_vector(cross(P2 - P1, P3 - P1));
        if (dot(normal, r.direction()) > 0) {
            normal = -normal;
        }
        
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, luz, esf, ci, co)) {
            I = cube.mat_cubo.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, normal, v, cube.mat_cubo, luz);
        }
    }
    
    return I;
}

#endif