#include "ChairObject.hpp"

ChairObject::ChairObject() {
  this->program = create_program("shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag");
  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  chair.model_matrix = glm::mat4(1.0f);
  /*  chair.model_matrix = glm::scale(chair.model_matrix, glm::vec3(0.02, 0.02, 0.02));*/
  chair.model_matrix = glm::translate(chair.model_matrix, glm::vec3(-1.2, 0.0, 1.2));
  chair.model_matrix = glm::rotate(chair.model_matrix, glm::radians(-45.0f), glm::vec3(0.0, 1.0, 0.0));
  chair.model_matrix = glm::translate(chair.model_matrix, glm::vec3(-2.75, 0.0, 0.0));
  chair.ambient_color = glm::vec4(0.5f);
  chair.diffuse_color = glm::vec4(1.0f);
  chair.specular_color = glm::vec3(0.2f);
  chair.shininess = 1.0;

  light_ubos.push_back(light);
  object_ubos.push_back(chair);

  object_ubos.push_back({glm::translate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(1.2, 0.0, 1.2)), glm::radians(45.0f),
                                                    glm::vec3(0.0, 1.0, 0.0)),
                                        glm::vec3(-2.75, 0.0, 0.0)),
                         glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});

  object_ubos.push_back({glm::translate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-1.2, 0.0, -1.2)), glm::radians(-135.0f),
                                                    glm::vec3(0.0, 1.0, 0.0)),
                                        glm::vec3(-2.75, 0.0, 0.0)),
                         glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});

  object_ubos.push_back({glm::translate(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(1.2, 0.0, -1.2)), glm::radians(135.0f),
                                                    glm::vec3(0.0, 1.0, 0.0)),
                                        glm::vec3(-2.75, 0.0, 0.0)),
                         glm::vec4(0.5f), glm::vec4(1.0f), glm::vec3(0.2f), 1.0});
}

ChairObject::~ChairObject() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void ChairObject::draw(CameraUBO camera, int fog) {

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(0), GL_DYNAMIC_STORAGE_BIT);

  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);

  glUseProgram(program);
  glProgramUniform1i(this->program, 4, fog);

  glBindTextureUnit(0, texture);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);

  for (size_t i = 0; i < meshes.size(); i++) {

    Mesh *chair_mesh = meshes.at(i).get();
    chair_mesh->draw();
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
}
