#include "src/vectors/vec3.h"
#include "src/material/structs.h"
#include "src/objects/hit_color.h"
#include "src/ray/ray.h"
#include "src/material/plano_cenario.h"
#include <iostream>
#include <cmath>

int main() {
    // -------- Requisitos da tarefa --------
    double wJanela = 0.6;           // largura da janela (m)
    double hJanela = 0.6;           // altura da janela (m)
    double dJanela = 0.3;           // distância da janela do olho(m)
    int nCol = 500;                 // número de colunas
    int nLin = 500;                 // número de linhas
    double Dx = wJanela / nCol;     // Tamanho dos pixels da janela. DX e DY
    double Dy = hJanela / nLin;
    point3 E(0,0,0);                // Olho do pintor     
    color bgColor(100.0/255.0, 100.0/255.0, 100.0/255.0);   // cinza    


    // -------- Plano do chão --------
    Plano chao;
    chao.point = point3(0, -1.5, 0);             // ponto conhecido do plano do chão
    chao.normal = vec3(0.0, 1.0, 0.0);
    chao.mat_plano = {
        .Kamb = color(0.2, 0.7, 0.2),
        .Kdif = color(0.2, 0.7, 0.2),            // provisório aqui deve ser o cálculo da textura
        .Kesp = color(0.0, 0.0, 0.0),
        .m = 1.0
    };

    // -------- Plano da parede lateral direita --------
    Plano paredeLdireita;
    paredeLdireita.point = point3(2.0, -1.5, 0.0),
    paredeLdireita.normal = vec3(-1.0, 0.0, 0.0),
    paredeLdireita.mat_plano = {
        .Kamb = color(0.686, 0.933, 0.933),
        .Kdif = color(0.686, 0.933, 0.933),
        .Kesp = color(0.686, 0.933, 0.933),
        .m = 1.0
    };

    // -------- Plano da parede frontal --------
    Plano paredeFrontal;
    paredeFrontal.point = point3(2.0, -1.5, -4.0),
    paredeFrontal.normal = vec3(0.0, 0.0, 1.0),
    paredeFrontal.mat_plano = {
        .Kamb = color(0.686, 0.933, 0.933),
        .Kdif = color(0.686, 0.933, 0.933),
        .Kesp = color(0.686, 0.933, 0.933),
        .m = 1.0
    };

    // -------- Plano da parede lateral esquerda --------
    Plano paredeLesquerda;
    paredeLesquerda.point = point3(-2.0, -1.5, 0.0),
    paredeLesquerda.normal = vec3(1.0, 0.0, 0.0),
    paredeLesquerda.mat_plano = {
        .Kamb = color(0.686, 0.933, 0.933),
        .Kdif = color(0.686, 0.933, 0.933),
        .Kesp = color(0.686, 0.933, 0.933),
        .m = 1.0
    };

    // -------- Plano do teto --------
    Plano teto;
    teto.point = point3(0.0, 1.5, 0.0),
    teto.normal = vec3(0.0, -1.0, 0.0),
    teto.mat_plano = {
        .Kamb = color(0.933, 0.933, 0.933),
        .Kdif = color(0.933, 0.933, 0.933),
        .Kesp = color(0.933, 0.933, 0.933),
        .m = 1.0
    };

    // -------- Planos do cenário --------
    Plano_cenario planos;
    planos.chao = chao;
    planos.paredeLdireito = paredeLdireita;
    planos.paredeLesquerda = paredeLesquerda;
    planos.paredeFrontal = paredeFrontal;
    planos.teto = teto;

    Cilindro ci;
    ci.centerB = point3(0.0, -1.5, -2.0);
    ci.radius = 0.05;
    ci.height = 0.90;
    ci.direcao = unit_vector(vec3(0.0, 1.0, 0.0));
    ci.mat_cilindro = {
        .Kamb = color(0.824, 0.706, 0.549),
        .Kdif = color(0.824, 0.706, 0.549),
        .Kesp = color(0.824, 0.706, 0.549),
        .m = 10.0
    }; 

    Cone cone;
    cone.centerB = point3(0.0, -0.6, -2.0);
    cone.radius = 0.9;
    cone.height = 1.5;
    cone.direcao = unit_vector(vec3(0.0, 1.0, 0.0));
    cone.mat_cone = {
        .Kamb = color(0.0, 1.0, 0.498),
        .Kdif = color(0.0, 1.0, 0.498),
        .Kesp = color(0.0, 1.0, 0.498),
        .m = 10.0
    };

    Esfera esf;
    esf.center = point3(0.0, 0.95, -2.0);
    esf.radius = 0.05;
    esf.mat_esfera = {
        .Kamb = color(0.854, 0.647, 0.125),
        .Kdif = color(0.854, 0.647, 0.125),
        .Kesp = color(0.854, 0.647, 0.125),
        .m = 10.0
    };

    Cubo cube;
    cube.aresta = 0.4;
    cube.centerCubo = point3(0.0, -1.5, -1.65);
    cube.mat_cubo = {
        .Kamb = color(1.0, 0.078, 0.576),
        .Kdif = color(1.0, 0.078, 0.576),
        .Kesp = color(1.0, 0.078, 0.576),
        .m = 10.0
    };
    ListMesh L = MakeCubeMesh(cube);                // malha do cubo

    // -------- Luz --------
    Luz luz;
    luz.P_F = vec3(-1.0, 1.4, -0.2);                       // Posição da fonte pontual situada a 60 cm acima do olho do observador
    luz.I_F = color(0.7, 0.7, 0.7);                        // Intensidade da luz pontual
    luz.I_A = color(0.3, 0.3, 0.3);                        // Intensidade da luz ambiente

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

            // Determinar a cor da esfera
            color pixel_color = ray_color(r, esf,  ci, cone, cube, L, planos, luz, E);

            // Converter para [0,255]
            int ir = int(255.999 * pixel_color.x());
            int ig = int(255.999 * pixel_color.y());
            int ib = int(255.999 * pixel_color.z());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}