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
    vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
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

// Comprimento do vetor
inline double length(const vec3& v) {
    return std::sqrt(dot(v, v));
}

// Estructs
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
    point3 point; //ponto p_pi
    vec3 normal;   //n_bar
    Material mat_plano;
};

struct Cone {
    point3 centerB;
    double radius;
    double height;
    vec3 direcao;
    Material mat_cone;
};

struct Cilindro {
    point3 centerB;
    double radius;
    double height;
    vec3 direcao;
    Material mat_cilindro;
};

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
double hit_sphere(const Esfera& esf,const ray& r) {
    vec3 w = r.origin() - esf.center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(w, r.direction());
    auto c = dot(w, w) - esf.radius*esf.radius;
    auto discriminante = b*b - 4*a*c;
    if (discriminante < 0) return -1.0;
    return (-b - std::sqrt(discriminante)) / (2.0*a);
}

// ----------------------
// Cilindro
// ----------------------
double hit_cylinder(const Cilindro&  ci, const ray& r) {
    vec3 v = (r.origin() -  ci.centerB) - dot((r.origin() -  ci.centerB), ci.direcao)*ci.direcao;
    vec3 w = r.direction() - dot(r.direction(), ci.direcao) *  ci.direcao;
    auto a = dot(w, w);
    auto b = 2.0 * dot(v, w);
    auto c = dot(v, v) - ci.radius*ci.radius;
    auto discriminante = b*b - 4 * a * c;
    double t_hit = -1.0;
    if(discriminante < 0) {
        return -1.0; 
    } else {
        double t1 = (-b - std::sqrt(discriminante)) / (2.0*a);
        double t2 = (-b + std::sqrt(discriminante)) / (2.0*a);
        if (t1 > 0 && t2 > 0)
            t_hit = std::min(t1, t2);     // os dois válidos → pega o mais próximo
        else if (t1 > 0)
            t_hit = t1;
        else if (t2 > 0)
            t_hit = t2;  
    }
    // Verifica se o ponto está dentro da altura do cilindro
    point3 P = r.origin() + t_hit * r.direction();
    double h = dot(P - ci.centerB, ci.direcao); // projeção sobre o eixo

    if (h < 0 || h > ci.height) return -1.0; // fora dos limites

    return t_hit;
}

// ----------------------
// Cone
// ----------------------
double hit_Cone(const Cone& co, const ray& r) {
    point3 vertice = co.centerB + co.height * co.direcao;
    vec3 v = vertice - r.origin();
    double cos_theta2 = (co.height*co.height) / (co.height*co.height + co.radius*co.radius);

    auto w = dot(r.direction(), co.direcao);
    auto m = dot(v, co.direcao);

    auto a = w*w - dot(r.direction(), r.direction()) * cos_theta2;
    auto b = dot(v, r.direction()) * cos_theta2 - m * w;    
    auto c = m * m - dot(v, v) * cos_theta2;
    auto discriminante = b*b -  a * c;
    double t_hit = -1.0;
    if(discriminante < 0){
        return t_hit;
    } else {
        const double EPS = 1e-12;
        if (std::fabs(a) < EPS) return -1.0;

        double t1 = (-b - std::sqrt(discriminante)) / a;
        double t2 = (-b + std::sqrt(discriminante)) / a;

        if (t1 > 0 && t2 > 0)
            t_hit = std::min(t1, t2);     // os dois válidos → pega o mais próximo
        else if (t1 > 0)
            t_hit = t1;
        else if (t2 > 0)
            t_hit = t2;  
    }

    point3 P = r.origin() + t_hit * r.direction();
    double h = dot(vertice - P, co.direcao); // projeção sobre o eixo
    if (h < 0.0 || h > co.height) return -1.0;
    return t_hit;
}

// ----------------------
// Plano
// ----------------------
double Cplano(const Plano pl, const ray& r) {
    vec3 w = r.origin() - pl.point;
    double s = -(dot(w, pl.normal));
    double t_i = s/(dot(r.direction(), pl.normal));
    if(t_i > 0.0 ) return t_i;                          // interseção válida 
    return -1.0;                                        // sem interserção visível 
}

// ----------------------
// Função cor
// ----------------------
color calculo_cor(const point3 Pi, const vec3 n, 
                    const vec3 v, const Material& mat, const Luz& luz) {
    
    vec3 l = unit_vector(luz.P_F - Pi);                 // Vetor da superfície até a luz
    vec3 r = unit_vector(2 * dot(l, n) * n - l);

    // Reflexão difusa 
    double ln = std::max(0.0, dot(l, n));
    color I_d = luz.I_F * mat.Kdif * ln;

    // Reflexão especular: (I_F @ K) * (v . r)^m
    double vr = std::max(0.0, dot(v, r));
    color I_e = luz.I_F * mat.Kesp * pow(vr, mat.m);

    color I_amb = mat.Kamb * luz.I_A; 
 
    // Soma total da iluminação 
    color I = I_amb + I_d + I_e;
    return color(std::min(I.x(),1.0), std::min(I.y(),1.0), std::min(I.z(),1.0));
}

bool shadowRay(const point3 Pi, const Esfera& esf,const Luz& luz) {
    vec3 l = unit_vector(luz.P_F - Pi);
    ray shadow_ray(Pi + 0.001 * l, l);
    // verifica se há algo entre P e a luz
    bool em_sombra = false;
    double t_sombra = hit_sphere(esf, shadow_ray);
    if (t_sombra > 0 && t_sombra < length(luz.P_F - Pi))
        em_sombra = true;
    return em_sombra;
}

color ray_color(const ray& r, const Esfera& esf, const Plano& plano_chao, const Plano& plano_fundo,
                const Luz& luz,const point3& E, color bgColor, const Cilindro& ci, const Cone& co
            ){
    // retorna o escalar
    double t_max = 1e30;
    double closest_t = t_max;
    color I = bgColor;

    double t_s = hit_sphere(esf, r);
    if(t_s > 0 && t_s < closest_t) {
        closest_t = t_s;
        point3 PI = r.origin() + t_s*r.direction();
        vec3 n = unit_vector(PI - esf.center);
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, esf, luz)) {
            I = esf.mat_esfera.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, n, v, esf.mat_esfera, luz);
        }
    }

    double t_ch = Cplano(plano_chao, r);
    if(t_ch > 0 && t_ch < closest_t) {
        closest_t = t_ch;
        point3 PI = r.origin() + t_ch*r.direction();
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, esf, luz)) {
            I = plano_chao.mat_plano.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, plano_chao.normal, v, plano_chao.mat_plano, luz);
        }
    }

    double t_f = Cplano(plano_fundo, r);
    if(t_f > 0 && t_f < closest_t) {
        closest_t = t_f;
        point3 PI = r.origin() + t_f*r.direction();
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, esf, luz)) {
            I = plano_fundo.mat_plano.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, plano_fundo.normal, v, plano_fundo.mat_plano, luz);
        }
    }

    double t_ci = hit_cylinder(ci, r);
    if(t_ci > 0 && t_ci < closest_t) {
        point3 PI = r.origin() + t_ci*r.direction();
        vec3 proj = dot(PI - ci.centerB, ci.direcao) * ci.direcao;
        vec3 n = unit_vector((PI - ci.centerB) - proj);
        vec3 v = unit_vector(E - PI);
        if(shadowRay(PI, esf, luz)) {
            I = ci.mat_cilindro.Kamb * luz.I_A;
        } else {
            I =  calculo_cor(PI, n, v, ci.mat_cilindro, luz);
        }
    }

    double t_co = hit_Cone(co, r);
    if(t_co > 0 && t_co < closest_t) {
        point3 PI = r.origin() + t_co *r.direction();
        vec3 v = unit_vector(E - PI);
        vec3 n = co.direcao;
        if (dot(r.direction(), n) > 0) {
            n = -n;
        }
        if(shadowRay(PI, esf, luz)) {
            I = co.mat_cone.Kamb * luz.I_A;
        }else {
            I = calculo_cor(PI, n, v, co.mat_cone, luz);
        }
        // std::cerr << "t_co = " << t_co
        //       << " | I = (" << I.x() << ", " << I.y() << ", " << I.z() << ")\n";
    }
    return I;
}

// ----------------------
// Main
// ----------------------
int main() {
    // -------- Requisitos da tarefa --------
    double wJanela = 0.6;           // largura da janela (m)
    double hJanela = 0.6;           // altura da janela (m)
    double dJanela = 0.3;           // distância da janela do olho(m)
    int nCol = 500;                 // número de colunas
    int nLin = 500;                 // número de linhas
    double Dx = wJanela / nCol;     // Tamanho dos pixels da janela. DX e DY
    double Dy = hJanela / nLin;
    point3 E(0,0,0);                // Olho do pintor     
    color bgColor(100.0/255.0, 100.0/255.0, 100.0/255.0);   // cinza    


    // ----- Material da esfera -----
    Material mat;
    mat.Kamb = color(0.7, 0.2, 0.2),
    mat.Kdif = color(0.7, 0.2, 0.2),
    mat.Kesp = color(0.7, 0.2, 0.2),
    mat.m = 10.0;

    // ----- Material do cilindro -----
    Material matCi;
    matCi.Kamb = color(0.2, 0.3, 0.8);
    matCi.Kdif = color(0.2, 0.3, 0.8);
    matCi.Kesp = color(0.2, 0.3, 0.8);
    matCi.m = 10.0;

    // ----- Material do cone -----    
    Material matCone;
    matCone.Kamb = color(0.8, 0.3, 0.2);
    matCone.Kdif = color(0.8, 0.3, 0.2);
    matCone.Kesp = color(0.8, 0.3, 0.2);
    matCone.m = 10.0;
    
    // ----- Material do plano do chão -----
    Material matC;
    matC.Kamb = color(0.2, 0.7, 0.2),
    matC.Kdif = color(0.2, 0.7, 0.2),
    matC.Kesp = color(0.0, 0.0, 0.0),
    matC.m = 1.0;  

    // ----- Material do plano de fundo  -----
    Material matF;
    matF.Kamb = color(0.3, 0.3, 0.7),
    matF.Kdif = color(0.3, 0.3, 0.7),
    matF.Kesp = color(0.0, 0.0, 0.0),
    matF.m = 1.0;  

    // -------- Luz --------
    Luz luz;
    luz.P_F = vec3(0, 0.6, -0.3);                          // Posição da fonte pontual situada a 60 cm acima do olho do observador
    luz.I_F = color(0.7, 0.7, 0.7);                        // Intensidade da luz pontual
    luz.I_A = color(0.3, 0.3, 0.3);                        // Intensidade da luz ambiente

    // -------- Esfera --------
    Esfera esf;
    esf.center = point3(0,0,-1.00);
    esf.radius = 0.4;
    esf.mat_esfera = mat;

    // -------- Plano do chão --------
    Plano chao;
    chao.point = point3(0, -esf.radius, 0);             // ponto conhecido do plano do chão
    chao.normal = vec3(0, 1, 0);
    chao.mat_plano = matC;

    // -------- Plano do fundo --------
    Plano fundo;
    fundo.point = point3(0, 0, -2);                     // ponto conhecido do plano do fundo 
    fundo.normal = vec3(0, 0, 1);
    fundo.mat_plano = matF;

    Cilindro ci;
    ci.centerB = esf.center;
    ci.radius = esf.radius / 3;
    ci.direcao = unit_vector(vec3(-1.0/std::sqrt(3.0), 1/std::sqrt(3.0), -1/std::sqrt(3.0)));
    ci.height = 3 * esf.radius;
    ci.mat_cilindro = matCi;    

    Cone cone;
    cone.direcao = ci.direcao;
    cone.centerB = ci.centerB + ci.height *ci.direcao;
    cone.radius = 0.5 * esf.radius;
    cone.height = cone.radius/3;
    cone.mat_cone = matCone;

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

            // Determinar a cor da esfera
            color pixel_color = ray_color(r, esf, chao, fundo, luz, E, bgColor, ci, cone);

            // Converter para [0,255]
            int ir = int(255.999 * pixel_color.x());
            int ig = int(255.999 * pixel_color.y());
            int ib = int(255.999 * pixel_color.z());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
