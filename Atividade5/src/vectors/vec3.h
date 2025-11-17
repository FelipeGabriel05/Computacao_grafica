#ifndef VEC3_H
#define VEC3_H
#include <cmath>

class vec3 {
public:
    double e[3];

    // Construtor. Inicializa o array na construção
    vec3() : e{0,0,0} {}

    // Construtor personalizado
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // sobrecarga 
    vec3 operator+(const vec3& v) const { return vec3(e[0]+v.e[0], e[1]+v.e[1], e[2]+v.e[2]); }
    vec3 operator-(const vec3& v) const { return vec3(e[0]-v.e[0], e[1]-v.e[1], e[2]-v.e[2]); }
    vec3 operator*(double t) const { return vec3(e[0]*t, e[1]*t, e[2]*t); }
    vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
};

// Clareza semântica 
using point3 = vec3;

// Produto escalar
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(
        u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]
    );
}

// Normalização
inline vec3 unit_vector(const vec3& v) {
    double len = std::sqrt(dot(v,v));
    return v * (1.0/len);
}

// Multiplicação compenente a compenente
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// (double * vec3)
inline vec3 operator*(double t, const vec3& v) {
    return v * t;
}

// Comprimento do vetor
inline double length(const vec3& v) {
    return std::sqrt(dot(v, v));
}

#endif