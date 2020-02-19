#include <iostream>

#include "application.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  // Configure fixed function pipeline
  glViewport(0, 0, width, height);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // --------------------------------------------------------------------------
  // Create Framebuffer
  // --------------------------------------------------------------------------
  glCreateFramebuffers(1, &framebuffer);

  // Initialize color output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_color);
  glTextureStorage2D(framebuffer_color, 1, GL_RGBA32F, width, height);

  // Initialize depth output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_depth);
  glTextureStorage2D(framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);

  // Set output 0 to GL_COLOR_ATTACHMENT0
  const GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
  glNamedFramebufferDrawBuffers(framebuffer, 1, draw_buffers);

  // Associate color and depth `attachments` with color and depth `textures`
  glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, framebuffer_color, 0);
  glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, framebuffer_depth, 0);
}

Application::~Application() {
  glDeleteTextures(1, &wood_texture);
  glDeleteTextures(1, &rock_texture);

  glDeleteProgram(main_program);
  glDeleteProgram(postprocess_program);

  glDeleteFramebuffers(1, &framebuffer);
  glDeleteTextures(1, &framebuffer_color);
  glDeleteTextures(1, &framebuffer_depth);
}

void Application::render() {
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

  // Default Material
  glProgramUniform3f(main_program, 8, 0.2f, 0.2f, 0.2f);
  glProgramUniform3f(main_program, 9, 0.5f, 0.5f, 0.5f);
  glProgramUniform3f(main_program, 10, 0.0f, 0.0f, 0.0f);
  glProgramUniform1f(main_program, 11, 4.0f);

  // --------------------------------------------------------------------------
  // Draw scene into the custom framebuffer
  // --------------------------------------------------------------------------

  // Bind the Framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  
  // Clear attachments
  glClearNamedFramebufferfv(framebuffer, GL_COLOR, 0, clear_color);
  glClearNamedFramebufferfv(framebuffer, GL_DEPTH, 0, clear_depth);

  // Configure fixed function pipeline
  glEnable(GL_DEPTH_TEST);

  // Use the `main` program
  glUseProgram(main_program);

  // Perform standard draw commands
  glBindTextureUnit(0, wood_texture);
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
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

  // --------------------------------------------------------------------------
  // Post-process
  // --------------------------------------------------------------------------

  // Bind back the default framebuffer (0)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Clear color buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Disabl depth test - we do not need it
  glDisable(GL_DEPTH_TEST);

  // Use post-process program
  glUseProgram(postprocess_program);

  // Bind the output from previous program as input texture to the post-process program
  glBindTextureUnit(0, framebuffer_color);

  // Draw the full-screen triangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glDeleteTextures(1, &framebuffer_color);
  glDeleteTextures(1, &framebuffer_depth);

  // Re-Initialize color output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_color);
  glTextureStorage2D(framebuffer_color, 1, GL_RGBA32F, width, height);

  // Re-Initialize depth output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer_depth);
  glTextureStorage2D(framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);

  // Re-Associate color and depth `attachments` with color and depth `textures`
  glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, framebuffer_color, 0);
  glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, framebuffer_depth, 0);
}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}