#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../structs/CameraUBO.hpp"
#include "../../structs/LightUBO.hpp"
#include "../../structs/ObjectUBO.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "mesh.hpp"

class LampObject {
public:
  LampObject();
  ~LampObject();
  void draw(CameraUBO camera, int fog);

private:
  std::vector<std::unique_ptr<Mesh>> meshes = Mesh::from_file("models/lamp/Lamp.obj");
  ObjectUBO lamp;

  std::vector<ObjectUBO> object_ubos;
  GLuint object_ubos_buffer;

  GLuint camera_buffer;

  LightUBO light;
  std::vector<LightUBO> light_ubos;
  GLuint light_ubos_buffer;

  GLuint program;
  GLuint texture = load_texture_2d("models/lamp/Lamp.jpg");
};