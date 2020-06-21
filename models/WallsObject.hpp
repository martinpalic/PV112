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

class WallsObject {
public:
  WallsObject();
  ~WallsObject();
  void draw(CameraUBO camera, int fog);

private:
  float face_size = 20.0f;

  Mesh bottom = Mesh::cube();
  Mesh front = Mesh::cube();
  Mesh top = Mesh::cube();
  Mesh left = Mesh::cube();
  Mesh right = Mesh::cube();
  Mesh back = Mesh::cube();
  Mesh terasse = Mesh::cube();
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
  GLuint top_texture;
  GLuint left_texture;
  GLuint right_texture;
  GLuint back_texture;
  GLuint terrase_texture;
};