#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

GLuint load_texture_2d(const std::string& filename);

GLuint load_texture_skybox(std::vector<std::string> filenames);