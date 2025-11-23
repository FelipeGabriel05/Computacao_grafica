#ifndef PLANE_H
#define PLANE_H
#include "../material/material.h"
#include "hittable.h"
#include "../vectors/vec3.h"
#include <memory>

class plane : public hittable {
    public:
        plane(  
            const point3& ponto,
            const vec3& normal,
            std::shared_ptr<material> m
        ) : p(ponto), n(unit_vector(normal)), mat(m) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override  {
            double denominador = dot(r.direction(), n);
            if (std::abs(denominador) < 1e-8) {
                return false;
            }

            double t = dot(p  - r.origin(), n) / denominador;

            if(t <= ray_tmin || t > ray_tmax) {
                return false;
            }

            rec.t = t;
            rec.p = r.at(t);
            rec.mat = mat;

            if (dot(r.direction(), n) > 0.0) {
                rec.normal = -n;
            } else {
                rec.normal = n;
            }

            // Ponto de colisão
            auto hit_p = rec.p;

            // mapeamento usando XZ (ideal para chão)
            double u = hit_p.x() * 0.005;
            double v = hit_p.z() * 0.10;

            // fmod para manter no intervalo [0, 1)
            u = u - std::floor(u);
            v = v - std::floor(v);

            rec.u = u;
            rec.v = v;

            return true;
        }

    private:
        point3 p;
        vec3 n;
        std::shared_ptr<material> mat;
};

#endif