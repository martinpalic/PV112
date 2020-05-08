#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utilities.hpp"
#include "program.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "../structs/LightUBO.hpp"
#include "../structs/CameraUBO.hpp"
#include "../structs/ObjectUBO.hpp"

// ----------------------------------------------------------------------------
// APPLICATION
// ----------------------------------------------------------------------------
class Application {
public:
  Application(size_t initial_width, size_t initial_height);
  ~Application();

  void render();
  void drawTable();

  void on_resize(GLFWwindow *window, int width, int height);
  void on_mouse_move(GLFWwindow *window, double x, double y);
  void on_mouse_pressed(GLFWwindow *window, int button, int action, int mods);
  void on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
  size_t width;
  size_t height;
  Camera camera;

  GLuint program;
  GLuint texture;

  GLuint camera_buffer;
  CameraUBO camera_ubo;
  
  LightUBO main_light_ubo;
  std::vector<LightUBO> main_lights;
  GLuint main_lights_buffer;

  
  ObjectUBO table_ubo;
  std::vector<ObjectUBO> table_objects;
  GLuint table_objects_buffer;

  std::vector<std::unique_ptr<Mesh>> table_mesh_vec;
};