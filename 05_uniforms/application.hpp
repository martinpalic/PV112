#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"
#include "utilities.hpp"
#include "program.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include <cstdint>




class Application {
public:
  Application(size_t initial_width, size_t initial_height);
  ~Application();

  void render();

  /// Input callbacks
  /// https://www.glfw.org/docs/latest/input_guide.html
  ///
  void on_resize(GLFWwindow *window, int width, int height);
  void on_mouse_move(GLFWwindow *window, double x, double y);
  void on_mouse_pressed(GLFWwindow *window, int button, int action, int mods);
  void on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
  size_t width;
  size_t height;

  GLfloat light_type;

  GLuint main_program = create_program("shaders/main.vert", "shaders/main.frag");
  GLuint skybox_program = create_program("shaders/skybox.vert", "shaders/skybox.frag");

  glm::vec4 light_position = glm::vec4(0.0, 0.0, 0.0, 0);

  Mesh cube = Mesh::cube();
  Mesh sphere = Mesh::sphere();
  Mesh teapot = Mesh::teapot();
  std::vector<std::unique_ptr<Mesh>> mine = Mesh::from_file("cat/12221_Cat_v1_l3.obj");

  // GLuint wood_texture = load_texture_2d("images/wood.jpg");
  // GLuint rock_texture = load_texture_2d("images/rocks.jpg");
  GLuint cat_texture = load_texture_2d("cat/Cat_diffuse.jpg");

  std::vector<std::string> faces{"skybox/right.jpg",  "skybox/left.jpg",  "skybox/top.jpg",
                                 "skybox/bottom.jpg", "skybox/front.jpg", "skybox/back.jpg"};
  GLuint cubemapTexture = load_texture_skybox(faces);

};