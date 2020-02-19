#include "application.hpp"
#include <stdlib.h>

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  // Task 1.1: Set the viewport
  glViewport(0, 0, initial_width, initial_height);

  // Task 1.2: Set the clear color
  glClearColor(255.0f / 255.0f, 105.0f / 255.0f, 180 / 255.0f, 1.0f);
}

Application::~Application() {}

void Application::render() {
  // Task 1.2: Clear the window
  double seconds = glfwGetTime();
  if ((int)seconds % 1 < 1) {
    glClearColor((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
  }
  glClear(GL_COLOR_BUFFER_BIT);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  // Task 1.1: Change the viewport to new dimensions
  //           Save new dimensions in the class
  glViewport(0, 0, width, height);
}

void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}