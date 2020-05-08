#include "Walls.hpp"

Walls::Walls(CameraUBO camera) {
  program = create_program("shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag");
  bottom_texture = load_texture_2d("models/skybox/bottom.jpg");
  front_texture = load_texture_2d("models/skybox/front.jpg");
  this->camera = camera;

  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  light_ubos.push_back(light);
  // bottom
  object_ubos.push_back({
      glm::scale(glm::mat4(1.0f), glm::vec3(8.0, 0.01, 8.0)), // model matrix
      glm::vec4(0.5f),                                        // ambient color
      glm::vec4(1.0f),                                        // diffuse color
      glm::vec3(0.2f),                                        // specular color
      1.0                                                     // shininess
  });

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &this->camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, object_ubos.size() * sizeof(ObjectUBO), object_ubos.data(), GL_DYNAMIC_STORAGE_BIT);
}

Walls::~Walls() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void Walls::draw() {
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);

  glUseProgram(this->program);

  glBindTextureUnit(0, bottom_texture);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  bottom.draw();
  // front
  object_ubos.clear();
  object_ubos.push_back({
      glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0,0.0,1.0)), glm::vec3(0.0, -8.0, -8.0)), glm::vec3(8.0, 8.0, 0.01)), // model matrix
      glm::vec4(0.5f),                                        // ambient color
      glm::vec4(1.0f),                                        // diffuse color
      glm::vec3(0.2f),                                        // specular color
      1.0                                                     // shininess
  });
  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &this->camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, object_ubos.size() * sizeof(ObjectUBO), object_ubos.data(), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, front_texture);
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  front.draw();

  // instanced
  // glBindVertexArray(bottom.get_vao());
  // glDrawElementsInstanced(bottom.get_mode(), bottom.get_indices_count(), GL_UNSIGNED_INT, nullptr, object_ubos.size());
}
