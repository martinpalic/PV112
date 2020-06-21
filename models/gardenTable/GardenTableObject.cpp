#include "GardenTableObject.hpp"

GardenTableObject::GardenTableObject() {
  this->program = create_program("shaders/spot_light_textured.vert", "shaders/spot_light_textured.frag");
  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
  light.specular_color = glm::vec4(0.0f);

  gardenTable.model_matrix = glm::mat4(1.0f);
  gardenTable.model_matrix = glm::translate(gardenTable.model_matrix, glm::vec3(-4.0, 0, 2.0));
  gardenTable.model_matrix = glm::scale(gardenTable.model_matrix, glm::vec3(0.2, 0.2, 0.2));
  gardenTable.model_matrix = glm::rotate(gardenTable.model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  gardenTable.model_matrix = glm::translate(gardenTable.model_matrix, glm::vec3(0.0, -6.0, 0.0));
  gardenTable.ambient_color = glm::vec4(0.5f);
  gardenTable.diffuse_color = glm::vec4(1.0f);
  gardenTable.specular_color = glm::vec3(0.2f);
  gardenTable.shininess = 1.0;

  light_ubos.push_back(light);
  object_ubos.push_back(gardenTable);

  
  spot_light.position = glm::vec4(-5.5, 1.8, 0.6, 0.0f);
  spot_light.ambient_color = glm::vec4(0.0f);
  spot_light.diffuse_color = glm::vec4(1.0);
  spot_light.specular_color = glm::vec4(0.0f);
  spot_light.direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
  spot_light.cos_cutoff = 0.8f;
  spot_light.exponent = 1;
}

GardenTableObject::~GardenTableObject() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void GardenTableObject::draw(CameraUBO camera, int fog) {


  glCreateBuffers(1, &spot_light_buffer);
  glNamedBufferStorage(spot_light_buffer, sizeof(CameraUBO), &spot_light, GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferSubData(spot_light_buffer, 0, sizeof(SpotLightUBO), &spot_light);

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
  glBindBufferBase(GL_UNIFORM_BUFFER, 3, spot_light_buffer);

  for (size_t i = 0; i < meshes.size(); i++) {

    Mesh *gardenTable_mesh = meshes.at(i).get();
    gardenTable_mesh->draw();
  }
}
