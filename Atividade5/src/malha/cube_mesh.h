#ifndef CUBE_MESH_H
#define CUBE_MESH_H
#include "list_mesh.h"
#include "../material/structs.h"
#include "../vectors/vec3.h"

ListMesh MakeCubeMesh(const Cubo& C) {
    ListMesh M;
    double a = C.aresta;
    double hx = a * 0.5;
    double hz = a * 0.5;

    // As arestas da base são paralelas aos eixos x e z do sistema de coordenadas
    // para determinar as posições de x e z com base no centro usamos a metade da aresta a e variamos o seu valor para deslocar
    // aos cantos da base. A coordenada Y é fixa
    const vec3 center = C.centerCubo;

    // Vértices da base (paralelos a X e Z)
    int v0 = M.addVertex({center.x() - hx, center.y(), center.z() - hz});
    int v1 = M.addVertex({center.x() + hx, center.y(), center.z() - hz});
    int v2 = M.addVertex({center.x() + hx, center.y(), center.z() + hz});
    int v3 = M.addVertex({center.x() - hx, center.y(), center.z() + hz});

    // Vértice do topo 
    int v4 = M.addVertex({center.x() - hx, center.y() + a, center.z() - hz});
    int v5 = M.addVertex({center.x() + hx, center.y() + a, center.z() - hz});
    int v6 = M.addVertex({center.x() + hx, center.y() + a, center.z() + hz});
    int v7 = M.addVertex({center.x() - hx, center.y() + a, center.z() + hz});

    // face da base
    M.addFace({v0, v1, v2});
    M.addFace({v0, v2, v3});

    // face do topo
    M.addFace({v4, v5, v6});
    M.addFace({v4, v6, v7});

    // face frontal
    M.addFace({v0, v5, v1});
    M.addFace({v0, v4, v5});

    // face traseira
    M.addFace({v3, v2, v6});
    M.addFace({v3, v6, v7});

    // face esquerda
    M.addFace({v0, v4, v7});
    M.addFace({v0, v7, v3});

    // face direita
    M.addFace({v1, v5, v6});
    M.addFace({v1, v6, v2});

    return M;
}
#endif