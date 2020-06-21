#pragma once

#include <glm/glm.hpp>

struct SpotLightUBO {
    glm::vec4 position;
    glm::vec4 ambient_color;
    glm::vec4 diffuse_color;
    glm::vec4 specular_color;
    glm::vec4 direction;
    float cos_cutoff;
    int exponent;
};