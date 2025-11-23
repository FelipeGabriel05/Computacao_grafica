#ifndef LIST_MESH_H
#define LIST_MESH_H
#include <vector>
#include "../vectors/vec3.h"

class LstVertex {
    vec3 pos_;
public:
    explicit LstVertex(const vec3& p = {}) : pos_(p) {}
    const vec3& pos() const { return pos_;}
};

class LstFace {
    std::vector<int> verts_;    // CCW vertex loop
public:
    explicit LstFace(std::vector<int> vs = {}) : verts_(std::move(vs)) {}
    const std::vector<int>& verts() const { return verts_;}
};

class ListMesh {
    std::vector<LstVertex> vertices_;
    std::vector<LstFace> faces_;
public:
    int addVertex(const vec3& p) { 
        vertices_.emplace_back(p); 
        return (int)vertices_.size()-1;
    }
    int addFace(const std::vector<int>& vs) {
        faces_.emplace_back(vs);
        return (int)faces_.size()-1;
    }

    const std::vector<LstFace>& faces() const { return faces_; }
    const std::vector<LstVertex>& vertices() const { return vertices_; }
};

// vertices_.emplace_back(p); — insere um novo LstVertex construído com p no final do vetor. emplace_back constrói no lugar (mais eficiente que push_back com cópia).
// return (int)vertices_.size()-1; — retorna o índice do vértice recém-adicionado (útil para usar nas faces).
#endif