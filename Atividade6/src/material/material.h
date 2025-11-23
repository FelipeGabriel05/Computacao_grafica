#ifndef MATERIAL_H
#define MATERIAL_H
#include "../colors/color.h"
#include "../texture/texture.h"

class material {
    public:
        std::shared_ptr<texture> tex;
        color K_amb;
        color K_dif;
        color K_esp;
        int shininess;

    material(const color& ka, const color& kdif, const color& kesp, int m, std::shared_ptr<texture> t = nullptr)
        : K_amb(ka), K_dif(kdif), K_esp(kesp), shininess(m), tex(t) {}

    color albedo(double u, double v, const point3& p) const {
        if (tex)
            return tex->value(u, v, p);
        return K_dif;
    }
};

#endif