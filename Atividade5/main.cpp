#include <iostream>
#include "src/ray/ray.h"
#include "src/vectors/vec3.h"
#include "src/colors/color.h"
#include "src/objects/hittable.h"
#include "src/objects/hittable_lists.h"
#include "src/material/material.h"
#include "src/objects/sphere.h"
#include "src/objects/plane.h"
#include "src/objects/cone.h"
#include "src/objects/cilindro.h"
#include "src/malha/cube_mesh.h"
#include "src/texture/texture.h"

// reflete v em torno de n
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v,n) * n;
}

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;

    double tmin = 0.001;
    if (!world.hit(r, tmin, std::numeric_limits<double>::infinity(), rec))
        return color(0, 0, 0);

    point3 light_pos(-100, 140, -20);

    // intensidade luz ambiente
    color I_A(0.3, 0.3, 0.3);
    // intensidade da fonte pontual
    color I_F(0.7, 0.7, 0.7);

    color base_color;
    if(rec.mat->tex)
        base_color = rec.mat->tex->value(rec.u, rec.v, rec.p);
    else
        base_color = rec.mat->K_dif;

    // componente ambiente
    color ambient_color = I_A * base_color;
    
    // verificação de sombra
    vec3 l = unit_vector(light_pos - rec.p);
    double light_distance = (light_pos - rec.p).length();

    point3 shadow_origin = rec.p + rec.normal * tmin;
    ray shadow_ray(shadow_origin, l);
    hit_record shadow_rec;

    if (world.hit(shadow_ray, tmin, light_distance, shadow_rec))
        return ambient_color;

    vec3 n = rec.normal;
    vec3 v = unit_vector(-r.direction());
    vec3 rfl = reflect(-l, n);

    double diff = std::max(0.0, dot(l, n));
    double spec = pow(std::max(0.0, dot(v, rfl)), rec.mat->shininess);

    color I_d = I_F * base_color * diff;
    color I_e = I_F * rec.mat->K_esp * spec;

    return ambient_color + I_d + I_e;
}


int main() {
    // -------- Requisitos da tarefa --------
    double wJanela = 60.0;           // largura da janela (cm)
    double hJanela = 60.0;           // altura da janela (cm)
    double dJanela = 30.0;           // distância da janela do olho(cm)
    int nCol = 500;                 // número de colunas
    int nLin = 500;                 // número de linhas
    double Dx = wJanela / nCol;     // Tamanho dos pixels da janela. DX e DY
    double Dy = hJanela / nLin;
    point3 E(0,0,0);                // Olho do pintor     

    hittable_list world;

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
    double R_esfera = 5;
    point3 C_esfera = point3(0, 95, -200);
    world.add(
        make_shared<sphere>(C_esfera, R_esfera, material_esfera)
    );

    auto material_plano1 = std::make_shared<material>(
        color(0.686, 0.933, 0.933), color(0.686, 0.933, 0.933), color(0.686, 0.933, 0.933), 10
    );


    auto tex_chao =
    std::make_shared<image_texture>("madeira.jpg");
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

    auto material_cube = std::make_shared<material>(
        color(1.0, 0.078, 0.576), color(1.0, 0.078, 0.576), color(1.0, 0.078, 0.576), 1.0 
    );

    // renderiza o cubo
    world.add(make_shared<cube_mesh>(40,point3(0, -150, -165),material_cube));


    auto material_cone = std::make_shared<material>(
        color(0.0, 1.0, 0.498), color(0.0, 1.0, 0.498), color(0.0, 1.0, 0.498), 1
    );
    world.add(make_shared<cone>(point3(0, -60, -200), vec3(0, 1, 0), 150, 90, true, material_cone));

    auto material_cilindro = std::make_shared<material>(
        color(0.824, 0.706, 0.549), color(0.824, 0.706, 0.549), color(0.824, 0.706, 0.549), 1
    );
    world.add(make_shared<cilindro>(point3(0, -150, -200), vec3(0, 1, 0), 90, 5, false, false, material_cilindro));

    // Planos do cenário
    world.add(make_shared<plane>(point3(0.0, -150, 0.0), vec3(0.0, 1.0, 0.0), material_chao));
    world.add(make_shared<plane>(point3(200, -150, 0.0), vec3(-1.0, 0.0, 0.0), material_plano1));
    world.add(make_shared<plane>(point3(200, -150, -400), vec3(0.0, 0.0, 1.0), material_plano1));
    world.add(make_shared<plane>(point3(-200, -150, 0.0), vec3(1.0, 0.0, 0.0), material_plano1));
    world.add(make_shared<plane>(point3(0.0, 150, 0.0), vec3(0.0, -1.0, 0.0), material_teto));

    vec3 dr = vec3(-1.0/sqrt(3.0), 1.0/sqrt(3.0), -1.0/sqrt(3.0));

    // PPM header
    std::cout << "P3\n" << nCol << " " << nLin << "\n255\n";

    // Loop linhas e colunas
    for (int l = 0; l < nLin; l++) {
        for (int c = 0; c < nCol; c++) {
            // Coordenadas do centro da célula
            double x = -wJanela/2.0 + Dx/2.0 + c*Dx;
            double y =  hJanela/2.0 - Dy/2.0 - l*Dy;
            double z = -dJanela;

            point3 pixelPos(x,y,z);
            vec3 dir = unit_vector(pixelPos - E); // direção do raio
            ray r(E, dir);

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
}