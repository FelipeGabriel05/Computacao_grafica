#ifndef CONE_H
#define CONE_H

#include "hittable.h"
#include "../material/material.h"
#include "../vectors/vec3.h"
#include <memory>
#include <cmath>

class cone : public hittable {
    public:
        cone(
            const point3& centroBase,
            const vec3& dir, // vetor que aponta da base para o vértice
            double h,
            double raio,
            bool tem_base,
            std::shared_ptr<material> m
        ) : centroBase(centroBase),
            h(h),
            raio(raio),
            tem_base(tem_base),
            u(unit_vector(dir)),
            vertice(centroBase + u*h),
            cos2_theta( (h*h) / (h*h + raio*raio) ), // pré-calcula cos^2(theta)
            m(m) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& hr) const override {
            bool hit_anything = false;
            double closest_t = ray_tmax;
            hit_record temp_rec;

            // Testa a base primeiro, se ela existir
            if (tem_base && teste_base(r, ray_tmin, closest_t, temp_rec)) {
                hit_anything = true;
                closest_t = temp_rec.t;
                hr = temp_rec;
            }

            // Testa o corpo do cone
            if (teste_corpo(r, ray_tmin, closest_t, temp_rec)) {
                hit_anything = true;
                closest_t = temp_rec.t;
                hr = temp_rec;
            }

            return hit_anything;
        }

    private:
        point3 centroBase;
        double h;
        double raio;
        bool tem_base;
        vec3 u;
        point3 vertice;
        double cos2_theta;
        std::shared_ptr<material> m;

        bool teste_corpo(const ray& r, double ray_tmin, double closest_t, hit_record& temp_rec) const {
            vec3 d = r.direction();
            point3 P0 = r.origin();
            vec3 n = u;
            point3 V = vertice;
            double c2 = cos2_theta;

            vec3 v = V - P0;

            double dn = dot(d, n);
            double dd = dot(d, d);
            double vn = dot(v, n);
            double vd = dot(v, d);
            double vv = dot(v, v);

            //((d.n)²-(d.d)cos²teta)t²+2((v.d)cos²tetaw - (v.n)(d.n))t + ((...))
            double a = dn*dn - dd*c2;
            double b = vd*c2 - vn*dn;
            double c = vn*vn - vv*c2;

            if (fabs(a) < 1e-12) return false;

            double delta = b*b - a*c;
            if (delta < 0.0) return false;
            double sqrtd = std::sqrt(delta);

            double raizes[] = { (-b - sqrtd) / a, (-b + sqrtd) / a };

            bool hit = false;
            hit_record rec;

            for (double t : raizes) {
                if (t <= ray_tmin || t >= closest_t) continue;

                point3 P = r.at(t);
                double proj = dot(V - P, n);
                if (proj < 0.0 || proj > h) continue;

                vec3 w = P - V;
                vec3 grad = ((dot(w, n)) * n) - c2 * w;

                double len = grad.length();
                if (len < 1e-12) continue;

                vec3 normal = grad / len;
                if (dot(r.direction(), normal) > 0)
                    normal = -normal;

                closest_t = t;
                hit = true;

                rec.t = t;
                rec.p = P;
                rec.normal = normal;
                rec.mat = m;
            }

            if (hit) temp_rec = rec;
            return hit;
        }

        // a interseção com a base do cone é idêntica à interseção com o fundo do cilindro
        bool teste_base(const ray& r, double ray_tmin, double closest_t, hit_record& temp_rec) const {
            vec3 normal = -u; // Normal aponta para fora da base
            double denominador = dot(r.direction(), normal);

            if (fabs(denominador) < 1e-12) return false;

            double t = dot(centroBase - r.origin(), normal) / denominador;

            if (t <= ray_tmin || t >= closest_t) return false;
            
            point3 p = r.at(t);
            double dist_squared = (p - centroBase).length_squared();
            
            if (dist_squared > raio*raio) return false;

            temp_rec.t = t;
            temp_rec.p = p;
            temp_rec.normal = normal;
            temp_rec.mat = m;
            return true;
        }
};

#endif