#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <camera.hpp>
#include "../structs/CameraUBO.hpp"

#include "../models/table/TableObject.hpp"
#include "../models/WallsObject.hpp"
#include "../models/snake/SnakeObject.hpp"
#include "../models/plate/PlateObject.hpp"
#include "../models/cup/CupObject.hpp"
#include "../models/chair/ChairObject.hpp"
#include "../models/cat/CatObject.hpp"
#include "../models/gardenTable/GardenTableObject.hpp"
#include "../models/lamp/LampObject.hpp"
#include "../models/treat/TreatObject.hpp"

// ----------------------------------------------------------------------------
// APPLICATION
// ----------------------------------------------------------------------------
class Application {
public:
  Application(size_t initial_width, size_t initial_height);
  ~Application();

  void render();

  void on_resize(GLFWwindow *window, int width, int height);
  void on_mouse_move(GLFWwindow *window, double x, double y);
  void on_mouse_pressed(GLFWwindow *window, int button, int action, int mods);
  void on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods);

private:

  void processTimedEvents();
  bool droppingTreat = false;
  bool catRunning = false;
  bool catReturning = false;
  bool catDead = false;
  glm::vec3 snakePossition = glm::vec3(3.0, 0.0, 2.0);
  glm::vec3 treatPossition = glm::vec3(0, -2, 0);
  glm::vec3 catPossition = glm::vec3(0, 0, 0);
  glm::vec3 returnVector = glm::vec3(0.0);
  float catRotation = 0.0f;

  size_t width;
  size_t height;
  Camera camera;  
  int fog = -1;
  CameraUBO camera_ubo;

  DWORD previousFrameTime;

  WallsObject woool = WallsObject();
  TableObject taable = TableObject();
  SnakeObject snaake = SnakeObject();
  PlateObject plaate = PlateObject();
  CupObject cuup = CupObject();
  ChairObject chaar = ChairObject();
  CatObject caat = CatObject();
  GardenTableObject gateee = GardenTableObject();
  LampObject laaaamp = LampObject();
  TreatObject treaeat = TreatObject();
};