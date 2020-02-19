#include "application.hpp"
#include <cstdlib>

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  // Task 1.1: Set the viewport
  glViewport(0, 0, width, height);

  // Task 1.2: Set the clear color
  glClearColor(0.5, 0.5, 0.5, 1);
}

Application::~Application() {}

void Application::render() {
  // Task 1.2: Clear the window
  glClear(GL_COLOR_BUFFER_BIT);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  // Task 1.1: Change the viewport to new dimensions
  //           Save new dimensions in the class
  this->height = height;
  this->width = width;
  glViewport(0, 0, width, height);
}

void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    double r = (double)rand() / (RAND_MAX);
    double g = (double)rand() / (RAND_MAX);
    double b = (double)rand() / (RAND_MAX);
    glClearColor(r, g, b, 1);
  }
}