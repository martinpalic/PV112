#include "application.hpp"

#include "utilities.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Reserved for future lesson
  glCreateVertexArrays(1, &vao);

  // Task 2.1: Compile vertex & fragment Shader
  // Help: - use the helper function load_file from utilities to load the code
  //         Example: const std::string vertex_shader_string = load_file("main.vert");
  //                  const char* vertex_shader_source = vertex_shader_string.data();
  //       - create a function so that you don't need to copy paste the code for both
  //         vertex and the fragment
  const char *vertexSource = NULL;
  const char *fragmentSource = NULL;
  GLuint vertexShader =
      createShaderFromFile("shaders/main.vert", GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = 
      createShaderFromFile("shaders/main.frag", GL_FRAGMENT_SHADER, fragmentSource);

  // Task 2.2: Link the shaders to create a program
  //           Delete shaders after linking
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

/*
returns compiled shader pointer of shaderType type with source held under shader
*/
GLuint Application::createShaderFromFile(std::string relativeAddress, GLenum shaderType, const char *shaderSource) {
  GLuint shader = glCreateShader(shaderType);
  std::string shaderString = load_file(relativeAddress);
  shaderSource = shaderString.data();
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  return shader;
}

Application::~Application() {
  glDeleteVertexArrays(1, &vao);
  // Task 2.2 Delete the program
  glDeleteProgram(program);
}

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Task 2.3: Use the compiled program for drawing

  glViewport(0, 0, width/2, height);
  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  
  glViewport(width / 2, 0, width/2, height);
  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}