#include "application.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  // Task 1.1: Set the viewport

  // Task 1.2: Set the clear color
}

Application::~Application() {}

void Application::render() {
  // Task 1.2: Clear the window
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  // Task 1.1: Change the viewport to new dimensions
  //           Save new dimensions in the class
}

void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}