#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cmath>
#include <iostream>

GLuint load_texture_2d(const std::string &filename) {
  int width, height, channels;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 4);

  GLuint texture;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);

  glTextureStorage2D(texture, std::log2(width), GL_RGBA8, width, height);

  glTextureSubImage2D(texture,
                      0,                         //
                      0, 0,                      //
                      width, height,             //
                      GL_RGBA, GL_UNSIGNED_BYTE, //
                      data);

  stbi_image_free(data);

  glGenerateTextureMipmap(texture);

  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}

GLuint load_texture_skybox(std::vector<std::string> filenames) {

  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < filenames.size(); i++) {
    unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << filenames[i] << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}