#ifndef CILINDRO_H
#define CILINDRO_H

#include "hittable.h"
#include "../material/material.h"
#include "../vectors/vec3.h"
#include <memory>
#include <cmath>

class cilindro : public hittable {
    public:
        cilindro(
            const point3& centroBase,
            const vec3& dir,
            double h,
            double raio,
            bool fundo,
            bool tampa,
            std::shared_ptr<material> m
        ) : centroBase(centroBase),
            h(h),
            raio(raio),
            fundo(fundo),
            tampa(tampa),
            u(unit_vector(dir)), 
            centroTopo(centroBase + u*h),
            m(m) {}

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& hr) const override {
            bool hit_anything = false;
            double closest_t = ray_tmax;
            hit_record temp_rec;

            if (fundo && teste_fundo(r, ray_tmin, closest_t, temp_rec)) {
                hit_anything = true;
                closest_t = temp_rec.t;
                hr = temp_rec;
            }

            if (tampa && teste_tampa(r, ray_tmin, closest_t, temp_rec)) {
                hit_anything = true;
                closest_t = temp_rec.t;
                hr = temp_rec;
            }

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
        bool fundo;
        bool tampa;
        vec3 u;
        point3 centroTopo;
        std::shared_ptr<material> m;

        // teste de interseção do corpo
        bool teste_corpo(const ray& r, double ray_tmin, double closest_t, hit_record& temp_rec) const {
            // equação de interseção do cilindro:
            // (w(w)t² + 2(vw)t + (v*v-R²=0)
            // w = d − (d ∙ u)u
            // u é o vetor unitário que define a direção e o sentido do eixo do cilindro;
            // v = (P0 − B) − ((P0 − B) ∙ u)u
            // v é apenas um vetor muito conveniente para simplificar expressões
            vec3 d = r.direction();
            point3 P = r.origin();
            vec3 deltaP = P - centroBase;
            vec3 w = d - dot(d, u)*u;
            vec3 v = deltaP - dot(deltaP, u)*u;

            double a = dot(w,w);
            double b = 2*dot(v,w);
            double c = dot(v,v) - raio*raio;

            auto delta = b*b-4*a*c;
            if (delta < 0) return false;
            if (fabs(a) < 1e-12) return false;

            // teste de interseção (corpo)
            // bhaskara
            auto sqrtd = std::sqrt(delta);
            
            double raizes[] = {
                (-b - sqrtd)/(2*a),
                (-b + sqrtd)/(2*a)
            };

            bool hit = false;
            hit_record b_rec;
            for (double tx : raizes) {
                if (tx <= ray_tmin || tx >= closest_t) continue;
            
                point3 p = r.at(tx);
                double altura = dot(p - centroBase, u);
                if (altura < 0 || altura > h) continue;

                closest_t = tx;
                hit = true;
                b_rec.t = tx;
                b_rec.p = p;
                // cálculo da normal
                b_rec.normal = unit_vector((p - centroBase) - dot(p - centroBase, u) * u);
                b_rec.mat = m;
            }

            if (hit) {
                temp_rec = b_rec;
            }

            return hit;
        }

        // teste de interseção do fundo
        bool teste_fundo(const ray& r, double ray_tmin, double closest_t, hit_record& temp_rec) const {
            vec3 normal = -u;
            vec3 d = r.direction();
            // t = (P - P0)*n/(d*n)
            double denominador = dot(d, normal);
            if (fabs(denominador) < 1e-12) return false;

            point3 p0 = r.origin();
            double t = dot(centroBase - p0, normal)/denominador;

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

        // teste de interseção da tampa
        bool teste_tampa(const ray& r, double ray_tmin, double closest_t, hit_record& temp_rec) const {
            vec3 normal = u;
            vec3 d = r.direction();
            // t = (P - P0)*n/(d*n)
            double denominador = dot(d, normal);
            if (fabs(denominador) < 1e-12) return false;

            point3 p0 = r.origin();
            double t = dot(centroTopo - p0, normal)/denominador;

            if (t <= ray_tmin || t >= closest_t) return false;
            point3 p = r.at(t);

            double dist_squared = (p - centroTopo).length_squared();
            if (dist_squared > raio*raio) return false;

            temp_rec.t = t;
            temp_rec.p = p;
            temp_rec.normal = normal;
            temp_rec.mat = m;

            return true;
        }
};

#endif