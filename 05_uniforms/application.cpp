#include "application.hpp"

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
}

Application::~Application() {
  // glDeleteTextures(1, &wood_texture);
  // glDeleteTextures(1, &rock_texture);
  glDeleteTextures(1, &cat_texture);
  glDeleteProgram(main_program);
}

void Application::render() {

  const float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  glm::vec3 camera_position = glm::vec3(1.0f, 3.0f, -8.0f);

  glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  glm::mat4 view_matrix = glm::lookAt(camera_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDepthMask(GL_FALSE);
  glUseProgram(skybox_program);

  GLuint skyboxVAO;
  GLuint diamond_positions_vbo;

  glCreateBuffers(1, &diamond_positions_vbo);
  glNamedBufferStorage(diamond_positions_vbo, 108 * sizeof(float), skyboxVertices, NULL);

  glCreateVertexArrays(1, &skyboxVAO);
  glVertexArrayVertexBuffer(skyboxVAO, 0, diamond_positions_vbo, 0, 3 * sizeof(float));

  glEnableVertexArrayAttrib(skyboxVAO, 0);
  glVertexArrayAttribFormat(skyboxVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(skyboxVAO, 0, 0);

  glBindVertexArray(skyboxVAO);
  glProgramUniformMatrix4fv(main_program, 0, 1, false, glm::value_ptr(projection_matrix));
  glm::mat4 view = glm::mat4(glm::mat3(view_matrix));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(view));
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // TIPS: Always use f after floating numbers, e.g 1.0f. This helps in avoiding glm template errors
  // Tak 5.1: Setup camera
  //           1. Choose camera position (for example 1.5f, 2.0f, -8.0f is good to see all our objects) as glm::vec3
  //           2. Create perspective matrix using glm::perspective(float field_of_view, float aspect_ratio, float near, float far);
  //              - glm works in radians by default, you can convert degrees to radians using glm::radians(degrees_
  //              - aspect_ratio is width of the window divided by height of the window
  //              - near and far planes should be: positive, near < far, near should be much smaller than 1.0f, e.g 0.01f
  //           3. Create view matrix using glm::lookAt(glm::vec3 eye_position, glm::vec3 look_at_position, glm::vec3 look_from_position);
  //           4. Leave model matrix as identity for now
  camera_position = glm::vec3(1.0f, 3.0f, -8.0f);

  projection_matrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  view_matrix = glm::lookAt(camera_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));
  model_matrix = glm::rotate(model_matrix, glm::radians(155.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  // Tak 5.1: Upload MVP matrices + camera(eye) position to `program`

  glProgramUniformMatrix4fv(main_program, 0, 1, false, glm::value_ptr(projection_matrix));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  glProgramUniformMatrix4fv(main_program, 2, 1, false, glm::value_ptr(view_matrix));

  // Tak 5.2: Set uniform data for light
  glm::vec3 object_color = glm::vec3(152.0f / 255.0f, 80.0f / 255.0f, 60.0f / 255.0f);
  glProgramUniform3fv(main_program, 4, 1, glm::value_ptr(object_color));
  glm::vec3 material_ambient_color = glm::vec3(0.3, 0.3, 0.3);
  glProgramUniform3fv(main_program, 5, 1, glm::value_ptr(material_ambient_color));
  glm::vec3 material_diffuse_color = glm::vec3(1, 1, 1);
  glProgramUniform3fv(main_program, 6, 1, glm::value_ptr(material_diffuse_color));
  glProgramUniform4fv(main_program, 7, 1, glm::value_ptr(light_position));
  glProgramUniform3fv(main_program, 8, 1, glm::value_ptr(camera_position));
  GLfloat shininess = 0.08;
  glProgramUniform1f(main_program, 9, shininess);
  glm::vec3 material_specular_color = glm::vec3(0.1, 0.1, 0.1);
  glProgramUniform3fv(main_program, 10, 1, glm::value_ptr(material_specular_color));

  glm::vec3 light_ambient_color = glm::vec3(0.2f, 0.2f, 0.2f);
  glProgramUniform3fv(main_program, 11, 1, glm::value_ptr(light_ambient_color));
  glm::vec3 light_diffuse_color = glm::vec3(1.0f, 1.0f, 1.0f);
  glProgramUniform3fv(main_program, 12, 1, glm::value_ptr(light_diffuse_color));
  glm::vec3 light_specular_color = glm::vec3(1.0f, 1.0f, 1.0f);
  glProgramUniform3fv(main_program, 13, 1, glm::value_ptr(light_diffuse_color));

  // Tak 5.2: Set uniform data for material
  // (Remember: it's ambient, diffuse, specular and additionally shininess for material)

  // Tak 5.1: Separate each object by moving them to different locations using
  //          glm::translate(glm::mat4 matrix_to_translate, glm::vec3)
  //          Play with other glm functions of the same form:
  //            - rotate (mat4x4 matrix_to_rotate, float angle, glm::vec3 axis_to_rotate_around)
  //            - translate (mat4x4 matrix_to_translate, glm::vec3 move_by_vector)
  // cube.draw();

  // SKYBOX

  glDepthMask(GL_TRUE);
  glUseProgram(main_program);

  glBindTextureUnit(0, cat_texture);
  glDepthMask(GL_TRUE);
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  for (size_t i = 0; i < mine.size(); i++) {
    mine.at(i).get()->draw();
  }

  model_matrix = glm::translate(glm::mat4(), glm::vec3(3.0f, 0.0f, 0.0f));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
  sphere.draw();

  model_matrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f, 0.0f));
  glProgramUniformMatrix4fv(main_program, 1, 1, false, glm::value_ptr(model_matrix));
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
    case GLFW_KEY_UP:
      light_position.y += 0.5f;
      break;
    case GLFW_KEY_DOWN:
      light_position.y -= 0.5f;
      break;
    case GLFW_KEY_LEFT:
      light_position.x -= 0.5f;
      break;
    case GLFW_KEY_RIGHT:
      light_position.x += 0.5f;
      break;
    case GLFW_KEY_PAGE_DOWN:
      light_position.z -= 0.5f;
      break;
    case GLFW_KEY_PAGE_UP:
      light_position.z += 0.5f;
      break;
    case GLFW_KEY_0:
      light_position.w == 1 ? light_position.w = 0 : light_position.w = 1;
      break;
    }
  }
}