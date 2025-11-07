#include <iostream>
#include <cmath>

// ----------------------
// Classe de vetores
// ----------------------
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
};

using point3 = vec3;
using color = vec3;

// Produto escalar
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0]*v.e[0] + u.e[1]*v.e[1] + u.e[2]*v.e[2];
}

// Normalização
inline vec3 unit_vector(const vec3& v) {
    double len = std::sqrt(dot(v,v));
    return v * (1.0/len);
}

// ----------------------
// Classe Ray
// ----------------------
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

// ----------------------
// Esfera
// ----------------------
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant >= 0);
}

// ----------------------
// Função cor
// ----------------------
color ray_color(const ray& r, const point3& esfCenter, double rEsfera,
                const color& esfColor, const color& bgColor) {
    if (hit_sphere(esfCenter, rEsfera, r)) {
        return esfColor; // bateu na esfera
    }
    return bgColor; // fundo cinza
}

// ----------------------
// Main
// ----------------------
int main() {
    // -------- Requisitos da tarefa --------
    double wJanela = 2.0;     // largura da janela (m)
    double hJanela = 2.0;     // altura da janela (m)
    double dJanela = 1.0;     // distância da janela do olho
    int nCol = 400;           // número de colunas
    int nLin = 400;           // número de linhas

    double rEsfera = 0.5;     // raio da esfera
    point3 esfCenter(0,0,-(dJanela+1.0)); // centro da esfera atrás da janela

    color esfColor(1.0, 0.0, 0.0);                  // vermelho
    color bgColor(100.0/255.0, 100.0/255.0, 100.0/255.0); // cinza

    // Tamanho dos pixels da janela
    double Dx = wJanela / nCol;
    double Dy = hJanela / nLin;

    // Olho do pintor
    point3 E(0,0,0);

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
            vec3 dir = pixelPos - E; // direção do raio
            ray r(E, dir);

            // Determinar a cor
            color pixel_color = ray_color(r, esfCenter, rEsfera, esfColor, bgColor);

            // Converter para [0,255]
            int ir = int(255.999 * pixel_color.x());
            int ig = int(255.999 * pixel_color.y());
            int ib = int(255.999 * pixel_color.z());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
