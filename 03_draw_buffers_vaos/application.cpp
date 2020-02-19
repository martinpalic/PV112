#include <iostream>

#include "application.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glCreateVertexArrays(1, &vao_empty);

  // Task 3.3: Create Vertex Buffer Objects (VBO)
  //           Copy vertex data found in data.hpp to VBOs
  glCreateBuffers(1, &diamond_positions_vbo);
  glNamedBufferStorage(diamond_positions_vbo, 36 * sizeof(float), &diamond_positions, NULL);
  // Task 3.4-3.6: Create Vertex Array Objects (VAO) fetching data from VBOs
}

Application::~Application() {
  glDeleteProgram(program);
}

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Task 3.1: Draw a Square
  // Task 3.2: Draw a Square using GL_TRIANGLE_STRIP
  glUseProgram(program);
  glBindVertexArray(vao_empty);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Task 3.4-3.6: Draw all objects
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}