#ifndef ARVORE_NATAL_H
#define ARVORE_NATAL_H
#include "../objects/hittable_lists.h"
#include "../objects/cilindro.h"
#include "../objects/cone.h"
#include "../objects/sphere.h"
#include "structs_cena.h"

class tree : public hittable_list {
    public:
        tree(tree_data& t) {
            add(make_shared<sphere>(
                t.centro_esfera, 
                t.raio_esfera, 
                t.material_esfera
            ));

            add(make_shared<cone>(
                t.centroBase_cone,
                t.dir_cone,
                t.altura_cone,
                t.raio_cone,
                t.tem_base_cone,
                t.material_cone
            ));

            add(make_shared<cilindro>(
                t.centroBase_tronco,
                t.dir_tronco,
                t.altura_tronco,
                t.raio_tronco,
                t.fundo_tronco,
                t.tampa_tronco,
                t.material_tronco
            ));

            add(make_shared<cilindro>(
                t.centroBase_suporte,
                t.dir_suporte,
                t.altura_suporte,
                t.raio_suporte,
                t.fundo_suporte,
                t.tampa_suporte,
                t.material_suporte
            ));
        }
};
#endif