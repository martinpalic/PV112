#include <iostream>

#include "application.hpp"

float random() { return rand() / (RAND_MAX + 1.); }
float random_neg() { return (rand() / (RAND_MAX + 1.) * 2.0) - 1.0; }

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  // --------------------------------------------------------------------------
  // Initialize UBO Data
  // --------------------------------------------------------------------------
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  light_ubo.position = glm::vec4(0.0f, 3.0f, 2.0f, 1.0f);
  light_ubo.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  light_ubo.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  light_ubo.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  object_ubo.model_matrix = glm::mat4(1.0f);
  object_ubo.ambient_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  object_ubo.diffuse_color = glm::vec4(0.0f, 0.0f, 0.8f, 1.0f);
  object_ubo.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  // Scatter lights
  for (size_t i = 0; i < 128; i++) {
    lights.push_back({
        glm::vec4(random_neg() * 100.0, random_neg() * 100.0, random_neg() * 100.0, 1.0), // position
        glm::vec4(0.0),                                                                   // ambient
        glm::vec4(random() * 100.0, random() * 100.0, random() * 100.0, 1.0),             // diffuse
        glm::vec4(0.0, 0.0, 0.0, 1.0)                                                     // specular
    });
  }

  // Scatter instanced objects
  const glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(10.0));
  for (size_t i = 0; i < 256; i++) {
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(random_neg() * 100.0, random_neg() * 100.0, random_neg() * 100.0));
    instanced_objects.push_back({
        glm::scale(translate, glm::vec3(2.5)),        // position
        glm::vec4(0.0),                               // ambient
        glm::vec4(random(), random(), random(), 1.0), // diffuse
        glm::vec4(0.0, 0.0, 0.0, 1.0)                 // specular
    });
  }

  // --------------------------------------------------------------------------
  // CREATE BUFFERS
  // --------------------------------------------------------------------------

  // TODO:
  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &light_buffer);
  glNamedBufferStorage(light_buffer, sizeof(LightUBO), &light_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &object_buffer);
  glNamedBufferStorage(object_buffer, sizeof(ObjectUBO), &object_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &lights_buffer);
  glNamedBufferStorage(lights_buffer, lights.size() * sizeof(LightUBO), lights.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &instanced_objects_buffer);
  glNamedBufferStorage(instanced_objects_buffer, instanced_objects.size() * sizeof(ObjectUBO), instanced_objects.data(),
                       GL_DYNAMIC_STORAGE_BIT);
}

Application::~Application() {
  glDeleteProgram(main_program);

  glDeleteBuffers(1, &camera_buffer);
}

void Application::render() {
  // --------------------------------------------------------------------------
  // UPDATE UBOS
  // --------------------------------------------------------------------------

  // Camera
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

  // --------------------------------------------------------------------------
  // DRAW THE SCENE
  // --------------------------------------------------------------------------
  // Clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Configure fixed function pipeline
  glViewport(0, 0, this->width, this->height);
  glEnable(GL_DEPTH_TEST);

  // Use the `main` program
  glUseProgram(main_program);

  // Bind camera and light buffers
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_UNIFORM_BUFFER, 1, lights_buffer);

  glBindVertexArray(sphere.get_vao());
  glDrawElementsInstanced(sphere.get_mode(), sphere.get_indices_count(), GL_UNSIGNED_INT, nullptr, lights.size());

  // Draw objects
  glUseProgram(main_program);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, instanced_objects_buffer);

  glBindVertexArray(teapot.get_vao());
  glDrawElementsInstanced(teapot.get_mode(), teapot.get_indices_count(), GL_UNSIGNED_INT, nullptr, instanced_objects.size());


  // TODO Task 8.2: Rewrite the main shader to use all of the lights

  // TODO:
  // - Task 8.3 bind lightS buffer
  // - Task 8.3 use draw_lights_program
  // - Task 8.3 call glDrawElementsInstanced to draw all the lights

  // TODO Task 8.4: Draw instanced objects
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}