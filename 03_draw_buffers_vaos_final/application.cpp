#include <iostream>

#include "application.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glCreateVertexArrays(1, &vao_empty);

  // Diamond
  glCreateBuffers(1, &diamond_positions_vbo);
  glNamedBufferStorage(diamond_positions_vbo, 36 * sizeof(float), diamond_positions, NULL);
  glCreateBuffers(1, &diamond_colors_vbo);
  glNamedBufferStorage(diamond_colors_vbo, 36 * sizeof(float), diamond_colors, NULL);

  glCreateVertexArrays(1, &diamond_vao);
  glVertexArrayVertexBuffer(diamond_vao, 0, diamond_positions_vbo, 0, 3 * sizeof(float));
  glVertexArrayVertexBuffer(diamond_vao, 1, diamond_colors_vbo, 0, 3 * sizeof(float));

  glEnableVertexArrayAttrib(diamond_vao, 0);
  glVertexArrayAttribFormat(diamond_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(diamond_vao, 0, 0);

  glEnableVertexArrayAttrib(diamond_vao, 1);
  glVertexArrayAttribFormat(diamond_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(diamond_vao, 1, 1);

  // Square
  glCreateBuffers(1, &square_vbo);
  glNamedBufferStorage(square_vbo, 36 * sizeof(float), square_data, NULL);

  glCreateVertexArrays(1, &square_vao);
  glVertexArrayVertexBuffer(square_vao, 0, square_vbo, 0, 3 * sizeof(float));
  glVertexArrayVertexBuffer(square_vao, 1, square_vbo, 6 * 3 * sizeof(float), 3 * sizeof(float));

  glEnableVertexArrayAttrib(square_vao, 0);
  glVertexArrayAttribFormat(square_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(square_vao, 0, 0);

  glEnableVertexArrayAttrib(square_vao, 1);
  glVertexArrayAttribFormat(square_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(square_vao, 1, 1);

  // Triangle
  glCreateBuffers(1, &triangle_vbo);
  glNamedBufferStorage(triangle_vbo, 3 * sizeof(Vertex), triangle_vertices, NULL);

  glCreateVertexArrays(1, &triangle_vao);
  glVertexArrayVertexBuffer(triangle_vao, 0, triangle_vbo, 0, sizeof(Vertex));

  glEnableVertexArrayAttrib(triangle_vao, 0);
  glVertexArrayAttribFormat(triangle_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(triangle_vao, 0, 0);

  glEnableVertexArrayAttrib(triangle_vao, 1);
  glVertexArrayAttribFormat(triangle_vao, 1, 3, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));
  glVertexArrayAttribBinding(triangle_vao, 1, 0);

  // Diamond
  glCreateBuffers(1, &diamond_indices_vbo);
  glCreateBuffers(1, &diamond_positions_indexed_vbo);
  glCreateBuffers(1, &diamond_colors_indexed_vbo);

  glNamedBufferStorage(diamond_indices_vbo, 12 * sizeof(uint32_t), diamond_indices, NULL);
  glNamedBufferStorage(diamond_positions_indexed_vbo, 15 * sizeof(float), diamond_positions_indexed, NULL);
  glNamedBufferStorage(diamond_colors_indexed_vbo, 15 * sizeof(float), diamond_colors_indexed, NULL);

  glCreateVertexArrays(1, &diamond_indexed_vao);
  glVertexArrayVertexBuffer(diamond_indexed_vao, 0, diamond_positions_indexed_vbo, 0, 3 * sizeof(float));
  glVertexArrayVertexBuffer(diamond_indexed_vao, 1, diamond_colors_indexed_vbo, 0, 3 * sizeof(float));

  glVertexArrayElementBuffer(diamond_indexed_vao, diamond_indices_vbo);

  glEnableVertexArrayAttrib(diamond_indexed_vao, 0);
  glVertexArrayAttribFormat(diamond_indexed_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(diamond_indexed_vao, 0, 0);

  glEnableVertexArrayAttrib(diamond_indexed_vao, 1);
  glVertexArrayAttribFormat(diamond_indexed_vao, 1, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(diamond_indexed_vao, 1, 1);
}

Application::~Application() { 
	glDeleteVertexArrays(1, &vao_empty);

	glDeleteProgram(program); 
	glDeleteProgram(program_vao);

	glDeleteVertexArrays(1, &diamond_vao);
    glDeleteBuffers(1, &diamond_positions_vbo);
    glDeleteBuffers(1, &diamond_colors_vbo);

	glDeleteVertexArrays(1, &square_vao);
    glDeleteBuffers(1, &square_vbo);

	glDeleteVertexArrays(1, &triangle_vao);
    glDeleteBuffers(1, &triangle_vbo);

	glDeleteVertexArrays(1, &diamond_indexed_vao);
    glDeleteBuffers(1, &diamond_indices_vbo);
    glDeleteBuffers(1, &diamond_positions_indexed_vbo);
    glDeleteBuffers(1, &diamond_colors_indexed_vbo);
}

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Task 3.2: Draw a Square using GL_TRIANGLE_STRIP
  // glUseProgram(program);
  // glBindVertexArray(vao_empty);
  // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glUseProgram(program_vao);

  glBindVertexArray(diamond_vao);
  glDrawArrays(GL_TRIANGLES, 0, 12);

  glBindVertexArray(square_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(triangle_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray(diamond_indexed_vao);
  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
}