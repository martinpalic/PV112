#include "WallsObject.hpp"

WallsObject::WallsObject() {
  program = create_program("shaders/draw_object_textured.vert", "shaders/textured_walls.frag");
  bottom_texture = load_texture_2d("models/skybox/bottom.jpg");
  front_texture = load_texture_2d("models/skybox/front.jpg");
  back_texture = load_texture_2d("models/skybox/back.jpg");
  top_texture = load_texture_2d("models/skybox/top.jpg");
  left_texture = load_texture_2d("models/skybox/left.jpg");
  right_texture = load_texture_2d("models/skybox/right.jpg");
  terrase_texture = load_texture_2d("models/skybox/wood.jpg");

  light.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  light.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  light_ubos.push_back(light);
  light_ubos.push_back({
    glm::vec4(0.0f, 3.0f, -2.0f, 0.0f),
    glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
  });

  // terrase
  object_ubos.push_back({
      glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(3, 0.2, 3)), glm::vec3(0.0, -0.8, 0.0)), // model matrix
      glm::vec4(0.0f),                                                                                            // ambient color
      glm::vec4(1.0f),                                                                                            // diffuse color
      glm::vec3(0.0f),                                                                                            // specular color
      1.0                                                                                                         // shininess
  });

  // bottom
  object_ubos.push_back({
      glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(face_size, 0.1, face_size)), glm::vec3(0.0,-1,0.0)), // model matrix
      glm::vec4(0.0f),                                                    // ambient color
      glm::vec4(1.0f),                                                    // diffuse color
      glm::vec3(0.0f),                                                    // specular color
      1.0                                                                 // shininess
  });
  // front
  object_ubos.push_back({
      glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)),
                                glm::vec3(0.0, -face_size/2, -face_size)),
                 glm::vec3(face_size, face_size, 0.1)), // model matrix
      glm::vec4(0.0f),                                   // ambient color
      glm::vec4(1.0f),                                   // diffuse color
      glm::vec3(0.0f),                                   // specular color
      1.0                                                // shininess
  });
  // top
  object_ubos.push_back({
      glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, face_size*1.5 , 0.0)), glm::vec3(face_size, 0.1, face_size)),
                  glm::radians(-180.0f), glm::vec3(0.0, 1.0, 0.0)), // model matrix
      glm::vec4(0.0f),                                            // ambient color
      glm::vec4(1.0f),                                            // diffuse color
      glm::vec3(0.0f),                                            // specular color
      1.0                                                         // shininess
  });
  // back
  object_ubos.push_back({
      glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)),
                                glm::vec3(0.0, -face_size/2, face_size)),
                 glm::vec3(face_size, face_size, 0.1)), // model matrix
      glm::vec4(0.0f),                                   // ambient color
      glm::vec4(1.0f),                                   // diffuse color
      glm::vec3(0.0f),                                   // specular color
      1.0                                                // shininess
  });
  // left
  object_ubos.push_back({
      glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)),
                                glm::vec3(-face_size, -face_size/2, 0.0)),
                 glm::vec3(0.1, face_size, face_size)), // model matrix
      glm::vec4(0.0f),                                   // ambient color
      glm::vec4(1.0f),                                   // diffuse color
      glm::vec3(0.0f),                                   // specular color
      1.0                                                // shininess
  });
  // right
  object_ubos.push_back({
      glm::scale(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0)),
                                glm::vec3(face_size, -face_size/2, 0.0)),
                 glm::vec3(0.1, face_size, face_size)), // model matrix
      glm::vec4(0.0f),                                   // ambient color
      glm::vec4(1.0f),                                   // diffuse color
      glm::vec3(0.0f),                                   // specular color
      1.0                                                // shininess
  });
}

WallsObject::~WallsObject() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &light_ubos_buffer);
  glDeleteBuffers(1, &object_ubos_buffer);
}

void WallsObject::draw(CameraUBO camera, int fog) {

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera, GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera);

  glCreateBuffers(1, &light_ubos_buffer);
  glNamedBufferStorage(light_ubos_buffer, sizeof(LightUBO), &light_ubos.at(0), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(0), GL_DYNAMIC_STORAGE_BIT);

  glUseProgram(this->program);

  glProgramUniform1i(this->program, 4, fog);

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(0), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, terrase_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  terasse.draw();

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(1), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, bottom_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  bottom.draw();

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(2), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, front_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  front.draw();

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(3), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, top_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  top.draw();

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(4), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, back_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  back.draw();

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(5), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, left_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  left.draw();

  glCreateBuffers(1, &object_ubos_buffer);
  glNamedBufferStorage(object_ubos_buffer, sizeof(ObjectUBO), &object_ubos.at(6), GL_DYNAMIC_STORAGE_BIT);
  glBindTextureUnit(0, right_texture);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, object_ubos_buffer);
  right.draw();

  glEnable(GL_DEPTH_TEST);
}
