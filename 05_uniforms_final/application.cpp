#include "application.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  main_program = create_program("shaders/main.vert", "shaders/main.frag");

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_MULTISAMPLE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

Application::~Application() { glDeleteProgram(main_program); }

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera
  glm::vec3 eye_position = glm::vec3(1.5f, 2.0f, -8.0f);
  glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
  glm::mat4 view_matrix = glm::lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 model_matrix = glm::mat4(1.0f);

  glProgramUniformMatrix4fv(main_program, 0, 1, false, glm::value_ptr(projection_matrix));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  glProgramUniformMatrix4fv(main_program, 2, 1, false, glm::value_ptr(view_matrix));
  glProgramUniform3f(main_program, 3, eye_position.x, eye_position.y, eye_position.z);

  // Light
  glProgramUniform4f(main_program, 4, 1.0f, 1.0f, 0.0f, 0);

  glProgramUniform3f(main_program, 5, 0.2f, 0.2f, 0.2f);
  glProgramUniform3f(main_program, 6, 1.0f, 1.0f, 1.0f);
  glProgramUniform3f(main_program, 7, 0.0f, 0.0f, 0.0f);

  // Material
  glProgramUniform3f(main_program, 8, 0.2f, 0.2f, 0.2f);
  glProgramUniform3f(main_program, 9, 0.5f, 0.5f, 0.5f);
  glProgramUniform3f(main_program, 10, 0.0f, 0.0f, 0.0f);
  glProgramUniform1f(main_program, 11, 4.0f);

  glUseProgram(main_program);

  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  cube.draw();

  model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  glProgramUniform3f(main_program, 9, 0.0f, 0.0f, 0.8f);
  sphere.draw();

  model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.0f, 0.0f));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  glProgramUniform3f(main_program, 9, 0.8f, 0.0f, 0.0f);
  teapot.draw();
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) {}
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) {}
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch (key) {
    case GLFW_KEY_F:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case GLFW_KEY_L:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case GLFW_KEY_Q:
      light = light == 1.0f ? 0.0f : 1.0f;
      break;
    }
  }
}