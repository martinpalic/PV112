#include <iostream>
#include <random>
#include <windows.h>

#include "application.hpp"

#pragma comment(lib, "winmm.lib")

float random() { return rand() / (RAND_MAX + 1.); }
float random_neg() { return (rand() / (RAND_MAX + 1.) * 2.0) - 1.0; }

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;
  PlaySound(TEXT("src/audio.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

Application::~Application() {}

CameraUBO createFromCamera(Camera camera, float width, float height) {
  CameraUBO camera_ubo;
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  return camera_ubo;
}

void Application::processTimedEvents() {
  DWORD elapsedMillis = GetTickCount() - previousFrameTime;

  if (this->catRunning) {
    // run cat to treat
    catPossition.x += treatPossition.x * (elapsedMillis / 1000.0f) * 2;
    catPossition.z += treatPossition.z * (elapsedMillis / 1000.0f) * 2;

    if (glm::abs(catPossition.x - treatPossition.x) < 0.2 && glm::abs(catPossition.z - treatPossition.z) < 0.2) {
      catPossition.x = treatPossition.x;
      catPossition.z = treatPossition.z;
      this->returnVector = glm::vec3(treatPossition.x, treatPossition.y, treatPossition.z);
      this->treatPossition = glm::vec3(3.0, 4.0, 3.0);
      this->catRunning = false;
      this->catReturning = true;
      this->catRotation = this->catRotation + 180;
    }
  }

  if (this->catReturning) {
    catPossition.x += -1 * returnVector.x * (elapsedMillis / 1000.0f) * 2;
    catPossition.z += -1 * returnVector.z * (elapsedMillis / 1000.0f) * 2;
    if (catPossition.x < 0.4 && catPossition.z < 0.4) {
      catPossition.x = 0;
      catPossition.z = 0;
      this->catReturning = false;
    }
  }

  // dropping candy if on
  if (this->droppingTreat) {
    this->treatPossition.y = treatPossition.y - (elapsedMillis / 1000.0f) * 4; // drop * x point per second
    if (!(treatPossition.y > 0)) {
      // candy dropped
      treatPossition.y = 0;
      droppingTreat = false;
      this->catRunning = true;
      this->catRotation = glm::sign(treatPossition.x) * 90 - 
          glm::sign(treatPossition.x) * glm::sign(treatPossition.z) * glm::degrees(glm::atan(glm::abs(treatPossition.z / treatPossition.x)));
      
    }
  }

  if (glm::abs(catPossition.x - snakePossition.x) < 0.3 && glm::abs(catPossition.z - snakePossition.z) < 0.3) {
    this->droppingTreat = false;
    this->catReturning = false;
    this->catRunning = false;
    this->catDead = true;
  }

  if (catDead) {
    catPossition.y += (elapsedMillis / 1000.0f) * 3;
    if (catPossition.y > 10) {
      std::cout << "You killed the cat, shame!";
      exit(0);
    }
  }
}

void Application::render() {

  processTimedEvents();

  camera_ubo = createFromCamera(this->camera, this->width, this->height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);

  woool.draw(camera_ubo, this->fog);
  snaake.draw(camera_ubo, this->fog);
  taable.draw(camera_ubo, this->fog);
  cuup.draw(camera_ubo, this->fog);
  plaate.draw(camera_ubo, this->fog);
  chaar.draw(camera_ubo, this->fog);
  caat.draw(camera_ubo, this->fog, catPossition, catRotation);
  gateee.draw(camera_ubo, this->fog);
  laaaamp.draw(camera_ubo, this->fog);
  treaeat.draw(camera_ubo, this->fog, treatPossition);

  previousFrameTime = GetTickCount();
}

void Application::on_resize(GLFWwindow *window, int width, int height) {}
void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (action != GLFW_RELEASE) {
    return;
  }
  if (key == 'F' || key == 'f') {
    this->fog = fog * -1;
  }
  if (key == 'R' || key == 'r') {
    this->treatPossition = glm::vec3(0, -2, 0);
  }
  if ((key == 'T' || key == 't') && !this->catRunning) {
    this->treatPossition = glm::vec3(3.0, 4.0, 3.0);
    this->droppingTreat = false;
  }
  if (key == GLFW_KEY_UP) {
    this->treatPossition.z -= 0.2f;
  }
  if (key == GLFW_KEY_DOWN) {
    this->treatPossition.z += 0.2f;
  }
  if (key == GLFW_KEY_RIGHT) {
    this->treatPossition.x += 0.2f;
  }
  if (key == GLFW_KEY_LEFT) {
    this->treatPossition.x -= 0.2f;
  }
  if (key == GLFW_KEY_SPACE && !(this->catReturning || this->catRunning)) {
    if (!this->droppingTreat && !this->catRunning) {
      this->droppingTreat = true;
    }
  }
}