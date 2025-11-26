#ifndef MESA_H
#define MESA_H
#include "../objects/hittable_lists.h"
#include "../malha/box_mesh.h"
#include "structs_cena.h"

class mesa : public hittable_list {
    public: 
        mesa (table_data& t) {
            add(make_shared<box_mesh> (
                t.centroBase_tampo,
                t.largura_tampo,
                t.altura_tampo,
                t.profundiade_tampo,
                t.material_tampo 
            ));

            add(make_shared<box_mesh> (
                t.centroBase_suporte1,
                t.largura_suporte1,
                t.altura_suporte1,
                t.profundidade_suporte1,
                t.material_suporte1
            ));

            add(make_shared<box_mesh> (
                t.centroBase_suporte2,
                t.largura_suporte2,
                t.altura_suporte2,
                t.profundidade_suporte2,
                t.material_suporte2
            ));
        }
};
#endif