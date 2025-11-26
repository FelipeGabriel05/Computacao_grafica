#ifndef MATERIAIS_CENARIO_H
#define MATERIAIS_CENARIO_H
#include "../material/material.h"

auto material_esfera = std::make_shared<material>(
    // coeficiente ambiente
    color(0.854, 0.647, 0.125),
    // coeficiente difuso
    color(0.854, 0.647, 0.125),
    // coeficiente especular
    color(0.854, 0.647, 0.125),
    // expoente especular
    50
);

auto material_plano1 = std::make_shared<material>(
    color(0.686, 0.933, 0.933), color(0.686, 0.933, 0.933), color(0.686, 0.933, 0.933), 10
);

auto tex_chao = std::make_shared<image_texture>("madeira.jpg");
// material com textura
auto material_chao = std::make_shared<material>(
    color(.1,.1,.1),
    color(1,1,1),
    color(0,0,0),
    0,
    tex_chao
);

auto material_teto = std::make_shared<material>(
    color(0.933, 0.933, 0.933), color(0.933, 0.933, 0.933), color(0.933, 0.933, 0.933), 1.0  
);

auto material_suporte = std::make_shared<material>(
    color(0.0, 0.0, 1.0), color(0.0, 0.0, 1.0), color(0.0, 0.0, 1.0), 1.0 
);

auto material_tampo = std::make_shared<material>(
    color(1.0, 0.27, 0.0), color(1.0, 0.27, 0.0), color(1.0, 0.27, 0.0), 1.0
);

auto material_cone = std::make_shared<material>(
    color(0.0, 1.0, 0.498), color(0.0, 1.0, 0.498), color(0.0, 1.0, 0.498), 1
);

auto material_cilindro = std::make_shared<material>(
    color(0.824, 0.706, 0.549), color(0.824, 0.706, 0.549), color(0.824, 0.706, 0.549), 1
);

#endif