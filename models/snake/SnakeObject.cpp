#include "SnakeObject.hpp"

SnakeObject::SnakeObject() {
  this->program = create_program("shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag");
  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  snake.model_matrix = glm::mat4(1.0f);
  snake.model_matrix = glm::translate(snake.model_matrix, glm::vec3(3.0, 0.0, 2.0));
  snake.model_matrix = glm::scale(snake.model_matrix, glm::vec3(0.02, 0.02, 0.02));
  snake.model_matrix = glm::rotate(snake.model_matrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  snake.ambient_color = glm::vec4(0.5f);
  snake.diffuse_color = glm::vec4(1.0f);
  snake.specular_color = glm::vec3(0.2f);
  snake.shininess = 1.0;

  light_ubos.push_back(light);
  object_ubos.push_back(snake);
}

SnakeObject::~SnakeObject() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void SnakeObject::draw(CameraUBO camera, int fog) {

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, object_ubos.size() * sizeof(ObjectUBO), object_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);

  glUseProgram(program);

  glProgramUniform1i(this->program, 4, fog);
  glBindTextureUnit(0, texture);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);

  for (size_t i = 0; i < meshes.size(); i++) {

    Mesh *snake_mesh = meshes.at(i).get();
    snake_mesh->draw();
  }
}
