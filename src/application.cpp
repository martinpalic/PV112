#include <iostream>
#include <random>

#include "application.hpp"
#include "../models/Walls.hpp"
#include "../models/table/Table.hpp"

float random() { return rand() / (RAND_MAX + 1.); }
float random_neg() { return (rand() / (RAND_MAX + 1.) * 2.0) - 1.0; }

void Application::drawTable() {
  this->program = create_program("shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag");
  this->table_mesh_vec = Mesh::from_file("models/table/table.obj");

  texture = load_texture_2d("models/table/wood.jpg");

  main_light_ubo.position = glm::vec4(0.0f, 3.0f, 2.0f, 0.0f);
  main_light_ubo.ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
  main_light_ubo.diffuse_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  main_light_ubo.specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

  table_ubo.model_matrix = glm::mat4(1.0f);
  table_ubo.ambient_color = glm::vec4(0.5f);
  table_ubo.diffuse_color = glm::vec4(1.0f);
  table_ubo.specular_color = glm::vec3(0.2f);
  table_ubo.shininess = 1.0;

  main_lights.clear();
  table_objects.clear();
  main_lights.push_back(main_light_ubo);
  table_objects.push_back(table_ubo);

  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &main_lights_buffer);
  glNamedBufferStorage(main_lights_buffer, main_lights.size() * sizeof(LightUBO), main_lights.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &table_objects_buffer);
  glNamedBufferStorage(table_objects_buffer, table_objects.size() * sizeof(ObjectUBO), table_objects.data(), GL_DYNAMIC_STORAGE_BIT);

  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

  // Clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, this->width, this->height);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(program);

  glBindTextureUnit(0, texture);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, main_lights_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, table_objects_buffer);

  for (size_t i = 0; i < table_mesh_vec.size(); i++) {

    Mesh *table_mesh = table_mesh_vec.at(i).get();
    table_mesh->draw();
  }
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &main_lights_buffer);
  glDeleteBuffers(1, &table_objects_buffer);
}

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;
}

Application::~Application() {
  glDeleteProgram(program);
  glDeleteBuffers(1, &camera_buffer);
  glDeleteBuffers(1, &main_lights_buffer);
  glDeleteBuffers(1, &table_objects_buffer);
}

CameraUBO createFromCamera(Camera camera, float width, float height) {
  CameraUBO camera_ubo;
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  return camera_ubo;
}

void Application::render() {

  camera_ubo = createFromCamera(this->camera, this->width, this->height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);

  //this->drawTable();
  Walls woool = Walls(camera_ubo);
  Table taable = Table(camera_ubo);
  taable.draw();
  woool.draw();

}

void Application::on_resize(GLFWwindow *window, int width, int height) {}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}