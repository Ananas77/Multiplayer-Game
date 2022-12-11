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

struct Light // do NOT use this as a lightsource, use StdLight instead
{
    glm::vec3 color;
    float ambient;
    float diffuse;
    float specular;
};

struct StdLight : Light
{
    glm::vec3 position;
};

struct DirLight : Light
{
    glm::vec3 direction;
};

struct PointLight : Light
{
    glm::vec3 position;

    float constant = 1.0f;
    float linear;
    float quadratic;
};

struct Spotlight : PointLight
{
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
};

#endif