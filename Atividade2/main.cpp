#include <iostream>
#include <cmath>

// ----------------------
// Classe de vetores
// ----------------------
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
};

// Clareza semântica 
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

// Multiplicação compenente a compenente
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// (double * vec3)
inline vec3 operator*(double t, const vec3& v) {
    return v * t;
}

// ----------------------
// Classe Ray
// ----------------------

// P(t) = origem + t.direção
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
double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 w = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(w, r.direction());
    auto c = dot(w, w) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) return -1.0;
    return (-b - std::sqrt(discriminant)) / (2.0*a);
}

// ----------------------
// Função cor
// ----------------------
color ray_color(const ray& r, const point3& esfCenter, double rEsfera,
                const color& esfColor, const color& bgColor,
                const color& I_F, const point3& P_F, const point3& E
            ) {

    // retorna o escalar
    double t = hit_sphere(esfCenter, rEsfera, r);

    // definição da luz ambiente
    color I_amb_global(0.1, 0.1, 0.1);
    color K_amb(0.2, 0.0, 0.0);

    // produto componente a componente
    color I_amb = I_amb_global * K_amb;

    if(t > 0.0) {
        point3 PI = r.origin() + t * r.direction();     // Ponto de interseção
        vec3 n = unit_vector(PI - esfCenter);           // Vetor normal do ponto da esfera 
        vec3 l = unit_vector(P_F - PI);                 // Vetor da superfície até a luz
        vec3 v = unit_vector(E - PI);                   // Vetor da superfície até o observador

        // Vetor da reflexão especular
        vec3 r_reflect = unit_vector(2 * dot(l, n) * n - l);

        // Reflexão difusa 
        double ln = std::max(0.0, dot(l, n));
        color I_d = I_F * esfColor * ln;

        // Reflexão especular: (I_F @ K) * (v . r)^m
        double vr = std::max(0.0, dot(v, r_reflect));
        int m = 100; // expoente especular
        color I_e = I_F * esfColor * pow(vr, m);
 
        // Soma total da iluminação 
        color I = I_amb + I_d + I_e;

        return color (
            std::min(I.x(), 1.0),
            std::min(I.y(), 1.0),
            std::min(I.z(), 1.0)
        );
    }
    return bgColor;
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

    color I_F(0.7, 0.7, 0.7);                               // Intensidade da luz pontual
    color esfColor(1.0, 0.0, 0.0);                          // vermelho
    color bgColor(100.0/255.0, 100.0/255.0, 100.0/255.0);   // cinza

    // Tamanho dos pixels da janela
    double Dx = wJanela / nCol;
    double Dy = hJanela / nLin;

    // Olho do pintor
    point3 E(0,0,0);

    // Posição da fonte pontual situada a 5 metros acima do olho do observador
    vec3 P_F(0, 5, 0); 

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
            color pixel_color = ray_color(r, esfCenter, rEsfera, esfColor, bgColor, I_F, P_F, E);

            // Converter para [0,255]
            int ir = int(255.999 * pixel_color.x());
            int ig = int(255.999 * pixel_color.y());
            int ib = int(255.999 * pixel_color.z());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
