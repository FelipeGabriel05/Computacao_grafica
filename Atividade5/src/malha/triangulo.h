#ifndef TRIANGULO_H
#define TRIANGULO_H
#include "../objects/hittable.h"
#include "list_mesh.h"
#include <cmath>
#include <memory>
#include <vector>


class triangulo : public hittable {
    public:
        triangulo (
            int i0,
            int i1,
            int i2,
            std::shared_ptr<ListMesh> meshPtr,
            std::shared_ptr<material> m
        ) : i0(i0), i1(i1), i2(i2), mesh(meshPtr), mat(m) {}

        bool hit(const ray& r, double ray_min, double ray_max, hit_record& rec) const override {
            auto& V = mesh->vertices();

            const point3& P1 = V[i0].pos();
            const point3& P2 = V[i1].pos();
            const point3& P3 = V[i2].pos();

            vec3 r1 = P2 - P1;
            vec3 r2 = P3 - P1;
            vec3 normal = unit_vector(cross(r1, r2));

            double denom = dot(r.direction(), normal);
            if(fabs(denom) < 1e-8) {
                return false;
            }

            double t = dot(P1 - r.origin(), normal) / denom;
            if(t < ray_min || t > ray_max) {
                return false;
            }

            point3 PI = r.at(t);

            // coordenadas barycentricas
            vec3 s1 = P1 - PI;
            vec3 s2 = P2 - PI;
            vec3 s3 = P3 - PI;

            double area2 = dot(cross(r1, r2), normal);

            double C1 = dot(cross(s3, s1), normal) / area2;
            if(C1 < 0) return false;

            double C2 = dot(cross(s1, s2), normal) / area2;
            if(C2 < 0) return false;

            double C3 = 1 - C1 - C2;
            if(C3 < 0) return false;

            rec.t = t;
            rec.p = PI;
            rec.mat = mat;

            if (dot(r.direction(), normal) > 0)
                rec.normal = -normal;
            else
                rec.normal = normal;
            
            return true;
        }

        private:
        int i0;
        int i1;
        int i2;
        std::shared_ptr<ListMesh> mesh;
        std::shared_ptr<material> mat;
    };

#endif