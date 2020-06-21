#include "TreatObject.hpp"

TreatObject::TreatObject() {
  this->program = create_program("shaders/draw_object.vert", "shaders/draw_object.frag");
  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  treat.model_matrix = glm::mat4(1.0f);
  treat.ambient_color = glm::vec4(0.5f);
  treat.diffuse_color = glm::vec4(0.5f, 0.35f, 0.05f, 1.0f);
  treat.specular_color = glm::vec3(0.2f);
  treat.shininess = 1.0;

  light_ubos.push_back(light);
}

TreatObject::~TreatObject() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void TreatObject::draw(CameraUBO camera, int fog, glm::vec3 position) {

  treat.model_matrix = glm::mat4(1.0f);
  treat.model_matrix = glm::translate(treat.model_matrix, position);
  treat.model_matrix = glm::scale(treat.model_matrix, glm::vec3(0.1, 0.1, 0.1));
  object_ubos.clear();
  object_ubos.push_back(treat);

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, light_ubos.size() * sizeof(LightUBO), light_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &treat , GL_DYNAMIC_STORAGE_BIT);

  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);

  glUseProgram(program);
  glProgramUniform1i(this->program, 4, fog);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, light_ubos_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);

  treat_mesh.draw();
}
