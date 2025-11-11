#ifndef PLANO_CENARIO_H
#define PLANO_CENARIO_H
#include "structs.h"

struct Plano_cenario {
    Plano chao;
    Plano paredeLdireito;
    Plano paredeFrontal;
    Plano paredeLesquerda;
    Plano teto;
};

struct Hit_plano {
    double t;
    const Plano* plano;
};

#endif