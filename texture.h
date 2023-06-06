#pragma once
#include "texture.h"
#include <GL/glew.h>
#include <SOIL2.h>
#include <iostream>
#include <string>
void bind_texture(GLuint shader, std::string path, GLuint unit, GLenum tex);
GLuint generate_texture(std::string path, GLuint unit, GLenum tex);
GLuint textureInit(const char* cesta, const bool mirror, const bool transparent);