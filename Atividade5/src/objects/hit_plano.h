#ifndef HIT_PLANO_H
#define HIT_PLANO_H
#include "plano.h"
#include "../ray/ray.h"
#include "../material/plano_cenario.h"

Hit_plano hit_plano(double closest_t, const Plano_cenario& planos, const ray& r) {
    double t_ch = Cplano(planos.chao, r);
    double t_fr = Cplano(planos.paredeFrontal, r);
    double t_Pld = Cplano(planos.paredeLdireito, r);
    double t_Ple = Cplano(planos.paredeLesquerda, r);
    double t_teto = Cplano(planos.teto, r);
    Hit_plano hit = {closest_t, nullptr};

    if(t_ch > 0 && t_ch < hit.t) {
        hit.t = t_ch;
        hit.plano = &planos.chao;
    }

    if(t_fr > 0 && t_fr < hit.t) {
        hit.t = t_fr;
        hit.plano = &planos.paredeFrontal;
    }

    if(t_Pld > 0 && t_Pld < hit.t) {
        hit.t = t_Pld;
        hit.plano = &planos.paredeLdireito;
    }

    if(t_Ple > 0 && t_Ple < hit.t) {
        hit.t = t_Ple;
        hit.plano = &planos.paredeLesquerda;
    }

    if(t_teto > 0 && t_teto < hit.t) {
        hit.t = t_teto;
        hit.plano = &planos.teto;
    }

    return hit;
}

#endif