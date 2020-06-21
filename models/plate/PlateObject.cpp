#include "PlateObject.hpp"

PlateObject::PlateObject() {
  this->program = create_program("shaders/draw_object.vert", "shaders/draw_object.frag");
  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  plate.model_matrix = glm::mat4(1.0f);
  plate.model_matrix = glm::translate(plate.model_matrix, glm::vec3(0.0, 1.44, 0.0));
  plate.model_matrix = glm::rotate(plate.model_matrix, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
  plate.model_matrix = glm::scale(plate.model_matrix, glm::vec3(0.08f, 0.08f, 0.08f));

  plate.ambient_color = glm::vec4(0.5f);
  plate.diffuse_color = glm::vec4(1.0f);
  plate.specular_color = glm::vec3(0.2f);
  plate.shininess = 1.0;

  light_ubos.push_back(light);
  object_ubos.push_back(plate);

  object_ubos.push_back(
      {glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.65, 1.44, 0.65)), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)),
                  glm::vec3(0.04f, 0.04f, 0.04f)),
       glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});

  object_ubos.push_back(
      {glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.65, 1.44, 0.65)), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)),
                  glm::vec3(0.04f, 0.04f, 0.04f)),
       glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});

  object_ubos.push_back(
      {glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.65, 1.44, -0.65)), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0)),
                  glm::vec3(0.04f, 0.04f, 0.04f)),
       glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});

  object_ubos.push_back({glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-0.65, 1.44, -0.65)), glm::radians(-90.0f),
                                                glm::vec3(1.0, 0.0, 0.0)),
                  glm::vec3(0.04f, 0.04f, 0.04f)),
       glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});
}

PlateObject::~PlateObject() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void PlateObject::draw(CameraUBO camera, int fog) {

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(0), GL_DYNAMIC_STORAGE_BIT);

  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);

  glUseProgram(program);
  glProgramUniform1i(this->program, 4, fog);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);

  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh *plate_mesh = meshes.at(i).get();
    plate_mesh->draw();
  }

    glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(1), GL_DYNAMIC_STORAGE_BIT);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh *cup_mesh = meshes.at(i).get();
    cup_mesh->draw();
  }

    glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(2), GL_DYNAMIC_STORAGE_BIT);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh *cup_mesh = meshes.at(i).get();
    cup_mesh->draw();
  }

    glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(3), GL_DYNAMIC_STORAGE_BIT);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh *cup_mesh = meshes.at(i).get();
    cup_mesh->draw();
  }

    glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(4), GL_DYNAMIC_STORAGE_BIT);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  for (size_t i = 0; i < meshes.size(); i++) {
    Mesh *cup_mesh = meshes.at(i).get();
    cup_mesh->draw();
  }
}
