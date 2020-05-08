#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "program.hpp"
#include "texture.hpp"
#include "mesh.hpp"

#include "../../structs/CameraUBO.hpp"
#include "../../structs/ObjectUBO.hpp"
#include "../../structs/LightUBO.hpp"

class Table {
public:
  Table(CameraUBO camera);
  ~Table();
  void draw();

private:
  std::vector<std::unique_ptr<Mesh>> meshes = Mesh::from_file("models/table/table.obj");
  ObjectUBO table;

  std::vector<ObjectUBO> object_ubos;
  GLuint object_ubos_buffer;

  CameraUBO camera;
  GLuint camera_buffer;

  LightUBO light;
  std::vector<LightUBO> light_ubos;
  GLuint light_ubos_buffer;

  GLuint program;
  GLuint texture = load_texture_2d("models/table/table.jpg");
};