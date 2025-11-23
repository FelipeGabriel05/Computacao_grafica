#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H
#include "../objects/hittable.h"
#include "list_mesh.h"
#include "triangulo.h"

class mesh_object : public hittable {
public:
    mesh_object(std::shared_ptr<ListMesh> mesh, std::shared_ptr<material> m) : mesh(mesh), mat(m) {
        for(const auto& f : mesh->faces()) {
            int a = f.verts()[0];
            int b = f.verts()[1];
            int c = f.verts()[2];

            triangles.push_back(
                std::make_shared<triangulo>(a,b,c, mesh, m)
            );
        }
    }

    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
        bool hit_anything = false;
        double closest = t_max;
        
        for(const auto& tri : triangles) {
            if(tri->hit(r,t_min,closest,rec)) {
                hit_anything = true;
                closest = rec.t;
            }
        }

        return hit_anything;
    }

private:
    std::shared_ptr<ListMesh> mesh;
    std::shared_ptr<material> mat;
    std::vector<std::shared_ptr<triangulo>> triangles;
};

#endif