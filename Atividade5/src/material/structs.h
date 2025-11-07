#ifndef STRUCTS_H
#define STRUCTS_H
#include "../vectors/vec3.h"

using color = vec3;

struct Luz {
    vec3 P_F;
    color I_F;
    color I_A;
};

struct Material {
    color Kamb;
    color Kdif;
    color Kesp;
    double m;
};

struct Esfera {
    point3 center;
    double radius;
    Material mat_esfera;
};

struct Plano {
    point3 point;           //ponto p_pi
    vec3 normal;            //n_bar
    Material mat_plano;
};

struct Cone {
    point3 centerB;
    double radius;
    double height;
    vec3 direcao;
    Material mat_cone;
};

struct Cilindro {
    point3 centerB;
    double radius;
    double height;
    vec3 direcao;
    Material mat_cilindro;
};

#endif