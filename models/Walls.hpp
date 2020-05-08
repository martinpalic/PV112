#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../structs/CameraUBO.hpp"
#include "../structs/LightUBO.hpp"
#include "../structs/ObjectUBO.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "mesh.hpp"

class Walls {
public:
  Walls(CameraUBO camera);
  ~Walls();
  void draw();

private:
  Mesh bottom = Mesh::cube();
  Mesh front = Mesh::cube();
  ObjectUBO bottom_ubo;

  std::vector<ObjectUBO> object_ubos;
  GLuint object_ubos_buffer;

  CameraUBO camera;
  GLuint camera_buffer;

  LightUBO light;
  std::vector<LightUBO> light_ubos;
  GLuint light_ubos_buffer;

  GLuint program;
  GLuint bottom_texture;
  GLuint front_texture;
};