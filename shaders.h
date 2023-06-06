#pragma once
#include <GL/glew.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include "globals.h"
#include <opencv2\opencv.hpp> // OpenCV

void make_shader(std::string vertex_shader, std::string fragment_shader, GLuint* shader);
void make_shaders();
std::string textFileRead(const std::string fn);
std::string getProgramInfoLog(const GLuint obj);
void logErrorShader(const GLuint obj);
void logErrorProgram(const GLuint obj);