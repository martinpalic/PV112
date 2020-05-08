#pragma once

#include <glm/glm.hpp>

struct ObjectUBO {
  glm::mat4 model_matrix;
  glm::vec4 ambient_color;
  glm::vec4 diffuse_color;
  glm::vec3 specular_color;
  float shininess;
};