#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "program.hpp"

class Application {
public:
  Application(size_t initial_width, size_t initial_height);
  ~Application();

  void render();

  void reload_shaders();

  /// Input callbacks
  /// https://www.glfw.org/docs/latest/input_guide.html
  ///
  void on_resize(GLFWwindow *window, int width, int height);
  void on_mouse_move(GLFWwindow *window, double x, double y);
  void on_mouse_pressed(GLFWwindow *window, int button, int action, int mods);
  void on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
  size_t width;
  size_t height;

  GLuint vao;

  GLuint program1 = create_program("shaders/main.vert", "shaders/main.frag");
  GLuint program2 = create_program("shaders/main2.vert", "shaders/main2.frag");
};