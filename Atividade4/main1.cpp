// salvar como cena_corrigida.cpp
#include <iostream>
#include <cmath>
#include <algorithm>

class vec3 {
public:
    double e[3];
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    vec3 operator+(const vec3& v) const { return vec3(e[0]+v.e[0], e[1]+v.e[1], e[2]+v.e[2]); }
    vec3 operator-(const vec3& v) const { return vec3(e[0]-v.e[0], e[1]-v.e[1], e[2]-v.e[2]); }
    vec3 operator*(double t) const { return vec3(e[0]*t, e[1]*t, e[2]*t); }
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
};

using point3 = vec3;
using color  = vec3;

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

inline vec3 unit_vector(const vec3& v) {
    double len = std::sqrt(dot(v,v));
    if (len == 0.0) return vec3(0,0,0);
    return v * (1.0/len);
}

inline vec3 operator*(double t, const vec3& v) { return v * t; }
inline vec3 operator*(const vec3& u, const vec3& v) { return vec3(u.e[0]*v.e[0], u.e[1]*v.e[1], u.e[2]*v.e[2]); }
inline double length(const vec3& v) { return std::sqrt(dot(v,v)); }

struct Luz {
    vec3 P_F;
    color I_F;
    color I_A;
};

struct Material {
    color Kamb;
    color Kdif;
    color Kesp;
    int m;
};

struct Esfera {
    point3 center;
    double radius;
    Material mat_esfera;
};

struct Plano {
    point3 point;
    vec3 normal;
    Material mat_plano;
};

struct Cilindro {
    point3 centerB;   // centro da base (segundo sua especificação)
    double radius;
    double height;    // distância da base até a top (na direção direcao)
    vec3 direcao;     // eixo (unit)
    Material mat_cilindro;
};

struct Cone {
    point3 centerB;   // centro da base (segundo sua especificação)
    double radius;    // raio da base
    double height;    // altura (distância base -> vértice, sentido direcao)
    vec3 direcao;     // eixo que aponta da base para o vértice (unit)
    Material mat_cone;
};

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}
    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
private:
    point3 orig;
    vec3 dir;
};

// sphere hit (unchanged)
double hit_sphere(const Esfera& esf,const ray& r) {
    vec3 oc = r.origin() - esf.center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - esf.radius*esf.radius;
    auto disc = b*b - 4*a*c;
    if (disc < 0) return -1.0;
    return (-b - std::sqrt(disc)) / (2.0*a);
}

// cylinder hit (keeps your algebra; ensure closest_t updated by caller)
double hit_cylinder(const Cilindro& ci, const ray& r) {
    vec3 CO = r.origin() - ci.centerB;
    vec3 v = CO - dot(CO, ci.direcao) * ci.direcao; // componente perpendicular à direção do eixo
    vec3 w = r.direction() - dot(r.direction(), ci.direcao) * ci.direcao;
    double a = dot(w,w);
    double b = 2.0 * dot(v, w);
    double c = dot(v,v) - ci.radius*ci.radius;
    double disc = b*b - 4*a*c;
    if (disc < 0.0) return -1.0;
    double t1 = (-b - std::sqrt(disc)) / (2.0*a);
    double t2 = (-b + std::sqrt(disc)) / (2.0*a);
    double t_hit = -1.0;
    if (t1 > 0 && t2 > 0) t_hit = std::min(t1,t2);
    else if (t1 > 0) t_hit = t1;
    else if (t2 > 0) t_hit = t2;
    else return -1.0;
    point3 P = r.origin() + t_hit * r.direction();
    double h = dot(P - ci.centerB, ci.direcao); // projeção sobre o eixo (0 .. height)
    if (h < 0.0 || h > ci.height) return -1.0;
    return t_hit;
}

// cone hit — coeficientes ajustados (b com fator 2, discriminant com 4*a*c)
double hit_Cone(const Cone& co, const ray& r) {
    // base center = co.centerB, direcao aponta da base para o vértice
    // vértice = base + height * direcao
    point3 vertice = co.centerB + co.height * co.direcao;
    vec3 CO = vertice - r.origin();
    double cos2 = (co.height*co.height) / (co.height*co.height + co.radius*co.radius); // cos^2(theta)
    double w = dot(r.direction(), co.direcao);
    double m = dot(CO, co.direcao);
    double a = w*w - dot(r.direction(), r.direction()) * cos2;
    double b = 2.0 * (dot(CO, r.direction()) * cos2 - m * w);
    double c = m*m - dot(CO, CO) * cos2;
    double disc = b*b - 4.0*a*c;
    if (disc < 0.0) return -1.0;
    const double EPS = 1e-12;
    if (std::fabs(a) < EPS) return -1.0;
    double t1 = (-b - std::sqrt(disc)) / (2.0*a);
    double t2 = (-b + std::sqrt(disc)) / (2.0*a);
    double t_hit = -1.0;
    if (t1 > 0 && t2 > 0) t_hit = std::min(t1,t2);
    else if (t1 > 0) t_hit = t1;
    else if (t2 > 0) t_hit = t2;
    else return -1.0;
    point3 P = r.origin() + t_hit * r.direction();
    double h = dot(vertice - P, co.direcao); // distância do ponto à vértice ao longo do eixo
    if (h < 0.0 || h > co.height) return -1.0;
    return t_hit;
}

// normal do cone em P (usa vértice = base + height*direcao)
vec3 normal_cone(const Cone& co, const point3& P) {
    point3 vertice = co.centerB + co.height * co.direcao;
    vec3 v = P - vertice;
    vec3 v_par = dot(v, co.direcao) * co.direcao;
    vec3 v_perp = v - v_par;
    double cos_theta = co.height / std::sqrt(co.height*co.height + co.radius*co.radius);
    double sin_theta = co.radius / std::sqrt(co.height*co.height + co.radius*co.radius);
    // formula: n ~ v_perp * cosθ - v_par * sinθ  (direção correta para superfície)
    return unit_vector(v_perp * cos_theta - v_par * sin_theta);
}

// plano (unchanged)
double Cplano(const Plano pl, const ray& r) {
    vec3 w = r.origin() - pl.point;
    double denom = dot(r.direction(), pl.normal);
    if (std::fabs(denom) < 1e-12) return -1.0;
    double s = -(dot(w, pl.normal));
    double t_i = s / denom;
    if (t_i > 0.0) return t_i;
    return -1.0;
}

// shading
color calculo_cor(const point3 Pi, const vec3 n, const vec3 v, const Material& mat, const Luz& luz) {
    vec3 l = unit_vector(luz.P_F - Pi);
    vec3 r = unit_vector(2.0 * dot(l,n) * n - l);
    double ln = std::max(0.0, dot(l,n));
    color I_d = luz.I_F * mat.Kdif * ln;
    double vr = std::max(0.0, dot(v,r));
    color I_e = luz.I_F * mat.Kesp * pow(vr, mat.m);
    color I_amb = mat.Kamb * luz.I_A;
    color I = I_amb + I_d + I_e;
    return color(std::min(I.x(),1.0), std::min(I.y(),1.0), std::min(I.z(),1.0));
}

// shadow test against sphere, cylinder and cone
bool shadow_any(const point3& Pi, const Esfera& esf, const Cilindro& ci, const Cone& co, const Luz& luz) {
    vec3 L = unit_vector(luz.P_F - Pi);
    ray shadow(Pi + 1e-4 * L, L);
    double maxd = length(luz.P_F - Pi);

    double t_s = hit_sphere(esf, shadow);
    if (t_s > 0 && t_s < maxd) return true;

    double t_cyl = hit_cylinder(ci, shadow);
    if (t_cyl > 0 && t_cyl < maxd) return true;

    double t_cone = hit_Cone(co, shadow);
    if (t_cone > 0 && t_cone < maxd) return true;

    return false;
}

color ray_color(const ray& r, const Esfera& esf, const Plano& plano_chao, const Plano& plano_fundo,
                const Luz& luz,const point3& E, color bgColor, const Cilindro& ci, const Cone& co) {
    double closest_t = 1e30;
    color I = bgColor;

    double t_s = hit_sphere(esf, r);
    if (t_s > 0 && t_s < closest_t) {
        closest_t = t_s;
        point3 PI = r.origin() + t_s * r.direction();
        vec3 n = unit_vector(PI - esf.center);
        vec3 v = unit_vector(E - PI);
        if (shadow_any(PI, esf, ci, co, luz)) I = esf.mat_esfera.Kamb * luz.I_A;
        else I = calculo_cor(PI, n, v, esf.mat_esfera, luz);
    }

    double t_ch = Cplano(plano_chao, r);
    if (t_ch > 0 && t_ch < closest_t) {
        closest_t = t_ch;
        point3 PI = r.origin() + t_ch * r.direction();
        vec3 v = unit_vector(E - PI);
        if (shadow_any(PI, esf, ci, co, luz)) I = plano_chao.mat_plano.Kamb * luz.I_A;
        else I = calculo_cor(PI, plano_chao.normal, v, plano_chao.mat_plano, luz);
    }

    double t_f = Cplano(plano_fundo, r);
    if (t_f > 0 && t_f < closest_t) {
        closest_t = t_f;
        point3 PI = r.origin() + t_f * r.direction();
        vec3 v = unit_vector(E - PI);
        if (shadow_any(PI, esf, ci, co, luz)) I = plano_fundo.mat_plano.Kamb * luz.I_A;
        else I = calculo_cor(PI, plano_fundo.normal, v, plano_fundo.mat_plano, luz);
    }

    double t_ci = hit_cylinder(ci, r);
    if (t_ci > 0 && t_ci < closest_t) {
        closest_t = t_ci;
        point3 PI = r.origin() + t_ci * r.direction();
        vec3 proj = dot(PI - ci.centerB, ci.direcao) * ci.direcao;
        vec3 n = unit_vector((PI - ci.centerB) - proj);
        vec3 v = unit_vector(E - PI);
        if (shadow_any(PI, esf, ci, co, luz)) I = ci.mat_cilindro.Kamb * luz.I_A;
        else I = calculo_cor(PI, n, v, ci.mat_cilindro, luz);
    }

    double t_co = hit_Cone(co, r);
    if (t_co > 0 && t_co < closest_t) {
        closest_t = t_co;
        point3 PI = r.origin() + t_co * r.direction();
        vec3 v = unit_vector(E - PI);
        vec3 n = normal_cone(co, PI);
        if (dot(r.direction(), n) > 0) n = -n;
        if (shadow_any(PI, esf, ci, co, luz)) I = co.mat_cone.Kamb * luz.I_A;
        else I = calculo_cor(PI, n, v, co.mat_cone, luz);
    }

    return I;
}

int main() {
    // janela
    double wJanela = 0.6, hJanela = 0.6, dJanela = 0.3;
    int nCol = 500, nLin = 500;
    double Dx = wJanela / nCol, Dy = hJanela / nLin;
    point3 E(0,0,0);
    color bgColor(100.0/255.0, 100.0/255.0, 100.0/255.0);

    // materiais
    Material m_sph{ color(0.7,0.2,0.2), color(0.7,0.2,0.2), color(0.7,0.2,0.2), 10 };
    Material m_ci{ color(0.2,0.3,0.8), color(0.2,0.3,0.8), color(0.2,0.3,0.8), 10 };
    Material m_co{ color(0.8,0.3,0.2), color(0.8,0.3,0.2), color(0.8,0.3,0.2), 10 };
    Material m_pl{ color(0.2,0.7,0.2), color(0.2,0.7,0.2), color(0,0,0), 1 };
    Material m_f{ color(0.3,0.3,0.7), color(0.3,0.3,0.7), color(0,0,0), 1 };

    Luz luz;
    luz.P_F = vec3(0, 0.6, -0.3);
    luz.I_F = color(0.7,0.7,0.7);
    luz.I_A = color(0.3,0.3,0.3);

    Esfera esf;
    esf.center = point3(0,0,-1.0);
    esf.radius = 0.4;
    esf.mat_esfera = m_sph;

    Plano chao; chao.point = point3(0, -esf.radius, 0); chao.normal = vec3(0,1,0); chao.mat_plano = m_pl;
    Plano fundo; fundo.point = point3(0,0,-2); fundo.normal = vec3(0,0,1); fundo.mat_plano = m_f;

    Cilindro ci;
    ci.centerB = esf.center; // base center at sphere center
    ci.radius = esf.radius / 3.0;
    ci.direcao = unit_vector(vec3(-1.0/std::sqrt(3.0), 1.0/std::sqrt(3.0), -1.0/std::sqrt(3.0)));
    ci.height = 3.0 * esf.radius; // three times sphere radius
    ci.mat_cilindro = m_ci;

    Cone cone;
    cone.direcao = ci.direcao; // same axis
    cone.centerB = ci.centerB + ci.height * ci.direcao; // center of base at top of cylinder
    cone.radius = 1.5 * esf.radius;  // per spec
    cone.height = (1.0/3.0) * cone.radius; // per spec
    cone.mat_cone = m_co;

    // PPM
    std::cout << "P3\n" << nCol << " " << nLin << "\n255\n";
    for (int l = 0; l < nLin; ++l) {
        for (int c = 0; c < nCol; ++c) {
            double x = -wJanela/2.0 + Dx/2.0 + c*Dx;
            double y =  hJanela/2.0 - Dy/2.0 - l*Dy;
            double z = -dJanela;
            point3 pixelPos(x,y,z);
            vec3 dir = pixelPos - E;
            ray r(E, dir);
            color pix = ray_color(r, esf, chao, fundo, luz, E, bgColor, ci, cone);
            int ir = int(255.999 * pix.x());
            int ig = int(255.999 * pix.y());
            int ib = int(255.999 * pix.z());
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}
