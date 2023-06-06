#pragma once

#include <GL/glew.h> // OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW toolkit
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <opencv2\opencv.hpp>
#include "vertex.h"
#include "texture.h"

struct vertex_p {
    glm::vec3 position;
};


class mesh {
public:
    std::vector<vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO;
    GLenum primitive;
    GLuint shader_id;
    GLuint texture_id;
    glm::mat4 model_matrix;
    glm::vec3 ambient_material, diffuse_material, specular_material;
    float specular_shinines;

    mesh() = default;
    mesh(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint& VAO, GLuint primitive);
    mesh(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint primitive = GL_TRIANGLES);

    void add_texture_id(const char* path, bool transp);
    void draw(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P);
    void draw_with_material(const glm::mat4& V, const glm::mat4& P, const glm::vec3& light_position);
    void add_material(glm::vec3 ambient_material, glm::vec3 diffuse_material, glm::vec3 specular_material, float specular_shinines);
};


