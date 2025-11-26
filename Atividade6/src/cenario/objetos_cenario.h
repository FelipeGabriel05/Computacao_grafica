#ifndef OBJETOS_CENARIO_H
#define OBJECTOS_CENARIO_H
#include "structs_cena.h"
#include "materiais_cenario.h"

tree_data criar_arvore() {
    tree_data arvore;
    // esfera da árvore de natal
    arvore.centro_esfera = point3(0, 144, -200);
    arvore.raio_esfera = 4.5;
    arvore.material_esfera = material_esfera;
    // cone da árvore de natal
    arvore.centroBase_cone = point3(0, -6, -200);
    arvore.dir_cone = vec3(0, 1, 0);
    arvore.altura_cone = 150;
    arvore.raio_cone = 60;
    arvore.tem_base_cone = true;
    arvore.material_cone = material_cone;

    // tronco
    arvore.centroBase_tronco = point3(0, -46, -200);
    arvore.dir_tronco = vec3(0, 1, 0);
    arvore.altura_tronco = 40;
    arvore.raio_tronco = 6;
    arvore.fundo_tronco = false;
    arvore.tampa_tronco = false;
    arvore.material_tronco = material_cilindro;

    // suporte
    arvore.centroBase_suporte = point3(0, -55, -200);
    arvore.dir_suporte = vec3(0, 1, 0);
    arvore.altura_suporte = 9;
    arvore.raio_suporte = 30;
    arvore.fundo_suporte = true;
    arvore.tampa_suporte = true;
    arvore.material_suporte = material_cilindro;
    return arvore;
}

table_data criar_mesa() {
    table_data mesa;
    mesa.centroBase_tampo = point3(20, -55, -200);
    mesa.largura_tampo = 250;
    mesa.altura_tampo = 5;
    mesa.profundiade_tampo = 150;
    mesa.material_tampo  = material_tampo;

    mesa.centroBase_suporte1 = point3(-102.5, -150, -200);
    mesa.largura_suporte1 = 5;
    mesa.altura_suporte1 = 95;
    mesa.profundidade_suporte1 = 150;
    mesa.material_suporte1 = material_suporte;

    mesa.centroBase_suporte2 = point3(142.5, -150, -200);
    mesa.largura_suporte2 = 5;
    mesa.altura_suporte2 = 95;
    mesa.profundidade_suporte2 = 150;
    mesa.material_suporte2 = material_suporte;
    return mesa;
}

#endif