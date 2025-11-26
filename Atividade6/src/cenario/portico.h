#ifndef PORTICO_H
#define PORTICO_H
#include "../objects/hittable_lists.h"
#include "structs_cena.h"
#include "../malha/box_mesh.h"
 
class portico : public hittable_list {
    public:
        portico (portico_data& p) {
            add(make_shared<box_mesh>(
                p.centroBase_colunaE,
                p.largura_colunaE,
                p.altura_colunaE,
                p.profundiade_colunaE,
                p.material_colunaE
            ));

            add(make_shared<box_mesh>(
                p.centroBase_colunaD,
                p.largura_colunaD,
                p.altura_colunaD,
                p.profundiade_colunaD,
                p.material_colunaD
            ));           
            
            add(make_shared<box_mesh>(
                p.centroBase_vigaE,
                p.largura_vigaE,
                p.altura_vigaE,
                p.profundiade_vigaE,
                p.material_vigaE
            ));   

            add(make_shared<box_mesh>(
                p.centroBase_vigaD,
                p.largura_vigaD,
                p.altura_vigaD,
                p.profundiade_vigaD,
                p.material_vigaD
            ));  
        }
};

#endif