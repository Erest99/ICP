#pragma once
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include "collider.h"
#include <vector>
#include <map>
#include "mesh.h"

struct s_globals {
    GLFWwindow* window;
    int width;
    int height;
    int window_xpos;
    int window_ypos;
    int window_width;
    int window_height;
    double mouse_xpos;
    double mouse_ypos;
    double app_start_time;
    bool fullscreen;
    double last_update;
    glm::vec4 color;
    std::map<std::string, GLuint> shader;
    std::vector<Collider> colliders;
    std::map<std::string, mesh> mesh;

};