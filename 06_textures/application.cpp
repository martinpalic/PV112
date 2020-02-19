#include "application.hpp"
#include <stb_image.h>

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_MULTISAMPLE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // Task 6.1: Initialize textures: "images/wood.jpg" and "images/rocks.jpg"
}

Application::~Application() {

  glDeleteProgram(main_program);
}

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera
  glm::vec3 eye_position = camera.get_eye_position();
  glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
  glm::mat4 view_matrix = glm::lookAt(eye_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 model_matrix = glm::mat4(1.0f);

  glProgramUniformMatrix4fv(main_program, 0, 1, false, glm::value_ptr(projection_matrix));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  glProgramUniformMatrix4fv(main_program, 2, 1, false, glm::value_ptr(view_matrix));
  glProgramUniform3f(main_program, 3, eye_position.x, eye_position.y, eye_position.z);

  // Light
  glProgramUniform4f(main_program, 4, 0.0f, 3.0f, 2.0f, light);

  glProgramUniform3f(main_program, 5, 0.2f, 0.2f, 0.2f);
  glProgramUniform3f(main_program, 6, 1.0f, 1.0f, 1.0f);
  glProgramUniform3f(main_program, 7, 0.0f, 0.0f, 0.0f);

//Task 6.1
  int width1, height1, channels1;
  unsigned char *data = stbi_load("images/wood.jpg", &width1, &height1, &channels1, 4);
  GLuint wood_texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &wood_texture);
  glTextureStorage2D(wood_texture, std::log2(width1), GL_RGBA8, width1, height1);
  glTextureSubImage2D(wood_texture, 0, 0,0,width1,height1, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(wood_texture);
  glTextureParameteri(wood_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(wood_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  
  int width2, height2, channels2;
  unsigned char *data2 = stbi_load("images/rock.jpg", &width2, &height2, &channels2, 4);
  GLuint rock_texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &rock_texture);
  glTextureStorage2D(rock_texture, std::log2(width2), GL_RGBA8, width2, height2);
  glTextureSubImage2D(rock_texture, 0, 0, 0, width2, height2, GL_RGBA, GL_UNSIGNED_BYTE, data2);
  glGenerateTextureMipmap(rock_texture);
  glTextureParameteri(rock_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(rock_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Material
  glProgramUniform3f(main_program, 8, 0.2f, 0.2f, 0.2f);
  glProgramUniform3f(main_program, 9, 0.5f, 0.5f, 0.5f);
  glProgramUniform3f(main_program, 10, 0.0f, 0.0f, 0.0f);
  glProgramUniform1f(main_program, 11, 4.0f);

  glUseProgram(main_program);

  

  // Task 6.2: Bind a texture using glBindTextureUnit for each object 
  glBindTextureUnit(0, wood_texture);
  //           (you can choose either wood or rocks texure)
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  glProgramUniform3f(main_program, 9, 1.0f, 1.0f, 1.0f);
  cube.draw();

  model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 0.0f, 0.0f));
  glBindTextureUnit(0, rock_texture);
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
void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
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