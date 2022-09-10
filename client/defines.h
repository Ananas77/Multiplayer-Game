#ifndef DEFINES_H
#define DEFINES_H

#include "lib/glm/glm.hpp"

struct Material
{
    float ambient;
    float diffuse;
    unsigned int shininess;

    unsigned int texture;
    unsigned int specular;
};

#endif