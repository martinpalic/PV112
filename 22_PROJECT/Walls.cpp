#include "Walls.hpp"

Walls::Walls(CameraUBO camera) {
  program = create_program("shaders/main_textured.vert", "shaders/main_textured.frag");
  texture = load_texture_2d("models/skybox/bottom.jpg");
  this->camera = camera;
  bottom = Mesh::cube();

  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  bottom_ubo.model_matrix = glm::mat4(1.0f);
  bottom_ubo.ambient_color = glm::vec4(0.5f);
  bottom_ubo.diffuse_color = glm::vec4(1.0f);
  bottom_ubo.specular_color = glm::vec3(0.2f);
  bottom_ubo.shininess = 1.0;

  light_ubos.push_back(light);
  object_ubos.push_back(bottom_ubo);

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &this->camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, object_ubos.size() * sizeof(ObjectUBO), object_ubos.data(),
                       GL_DYNAMIC_STORAGE_BIT);
}

Walls::~Walls() {}

void Walls::draw() {
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(this->program);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  glBindTextureUnit(0, texture);

  bottom.draw();
}
