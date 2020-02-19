#include "application.hpp"

#include "utilities.hpp"


Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  glViewport(0, 0, width/2, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Reserved for future lesson
  glCreateVertexArrays(1, &vao);

  const std::string vertex_shader_source = load_file("shaders/main.vert");
  const std::string fragment_shader_source = load_file("shaders/main.frag");

  const char *vertex_shader_source_ptr = vertex_shader_source.data();
  const char *fragment_shader_source_ptr = fragment_shader_source.data();

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(vs, 1, &vertex_shader_source_ptr, nullptr);
  glShaderSource(fs, 1, &fragment_shader_source_ptr, nullptr);

  glCompileShader(vs);
  glCompileShader(fs);

  // Task 2.1: Compile vertex & fragment Shader
  // Help: - use the helper function load_file from utilities to load the code
  //         Example: const std::string vertex_shader_source = load_file("main.vert");
  //       - create a function so that you don't need to copy paste the code for both
  //         vertex and the fragment

  // Task 2.2: Link the shaders to create a program
  //           Delete shaders after linking

  this->program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);

  glLinkProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);



}

Application::~Application() {
  glDeleteVertexArrays(1, &vao);
  // Task 2.2 Delete the program
  glDeleteProgram(program);
}

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(vao);

  glViewport(0, 0, width / 2, height);
  // Task 2.3: Use the compiled program for drawing
  glUseProgram(program);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glViewport(width / 2, 0, width/2, height);
  glUseProgram(program);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glViewport(0, 0, width/2, height);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}