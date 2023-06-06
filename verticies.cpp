#include "verticies.h"
#define M_PI       3.14159265358979323846   // pi

extern s_globals globals;

void make_triangle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {

    Collider col1;
    std::vector<vertex> vertices = {
        // Top
        {{ 0.0f,  2.0f,  3.0f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f}},

        // Base
        {{-1.0f, -0.0f,  2.0f}, {0.5f, 0.5f, 0.5f}, {0.5f, 0.0f}},
        {{ 1.0f, -0.0f,  2.0f}, {0.5f, 0.5f, 0.5f}, {1.0f, 0.5f}},
        {{ 1.0f, -0.0f,  4.0f}, {0.5f, 0.5f, 0.5f}, {0.5f, 1.0f}},
        {{-1.0f, -0.0f,  4.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.5f}},
    };

    std::vector<GLuint> indices = {
    0, 2, 1,
    0, 3, 2,
    0, 4, 3,
    0, 1, 4,
    2, 4, 1,
    4, 2, 3
    };
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    col1 = { min, max };
    globals.colliders.push_back(col1);
}

void make_floor(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {

    Collider col1;
    std::vector<vertex> vertices = {
        {{ -50.0f,  0.0f,  -50.0f}, {0.8f, 0.1f, 0.1f}, {0.5f, 0.5f}},
        {{ 50.0f, 0.0f,  -50.0f}, {0.8f, 0.1f, 0.1f}, {0.5f, 0.0f}},
        {{ -50.0f, 0.0f,  50.0f}, {0.8f, 0.1f, 0.1f}, {1.0f, 0.5f}},
        {{ 50.0f, 0.0f,  50.0f}, {0.8f, 0.1f, 0.1f}, {0.5f, 1.0f}},
    };

    std::vector<GLuint> indices = {
    0,2,1, 1,2,3
    };
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    col1 = { min, max };
    globals.colliders.push_back(col1);
}

void make_triangle2(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
    Collider col1;
    std::vector<vertex> vertices = {
        //souradnice        //barva
    { { 10.0f, 1.0f, 0.0f}, {0.8f,0.6f,0.1f}, {0.5f,1.0f} }, //vrchol
    { { 9.5f, -0.0f, 0.5f}, {0.8f,0.6f,0.1f}, {0.0f,1.0f}}, //1.spodek
    { { 9.5f, -0.0f, -0.5f}, {0.8f,0.6f,0.1f}, {0.0f,0.0f}}, //2.vrchol
    { { 10.5f, -0.0f, -0.5f}, {0.8f,0.6f,0.1f}, {0.0f,1.0f}},
    { { 10.5f, -0.0f, 0.5f}, {0.8f,0.6f,0.1f}, {0.0f,0.0f} }
    };
    std::vector<GLuint> indices = { 0,2,1,  0,1,4,  0,4,3, 0,3,2 };
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    col1 = { min, max };
    globals.colliders.push_back(col1);
}

void make_cube(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
    Collider col1;
    std::vector<vertex> vertices = {
        //souradnice        //barva
    { { 1.0f,0.0f,2.0f}, {0.0f,0.6f,0.8f}, {1.0f,1.0f}}, //0
    { { 2.0f,0.0f,2.0f}, {0.0f,0.6f,0.8f}, {0.0f,1.0f}}, //1
    { { 1.0f,0.0f,1.0f}, {0.0f,0.6f,0.8f}, {0.0f,1.0f}}, //2
    { { 2.0f,0.0f,1.0f}, {0.0f,0.6f,0.8f}, {1.0f,1.0f}}, //3
    { {1.0f,1.0f,2.0f}, {0.0f,0.6f,0.8f}, {1.0f,0.0f}},  //4
    { {2.0f,1.0f,2.0f}, {0.0f,0.6f,0.8f}, {0.0f,0.0f}},  //5
    { {2.0f,1.0f,1.0f}, {0.0f,0.6f,0.8f}, {1.0f,0.0f}},  //6
    { { 1.0f,1.0f,1.0f}, {0.0f,0.6f,0.8f}, {0.0f,0.0f}}  //7
    };

    std::vector<GLuint> indices = { 0,1,4,  1,5,4,  1,3,6,  6,5,1,  3,2,7, 7,6,3, 2,0,4, 4,7,2, 4,5,6, 6,7,4, 0,1,3, 3,2,0};
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    glm::vec3 margin = { 0.15f, 0.15f,0.15f };
    col1 = { min - margin, max + margin };
    globals.colliders.push_back(col1);
}

void make_cube2(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
    Collider col1;
    std::vector<vertex> vertices = {
        //souradnice        //barva
    { { 5.0f,0.0f,2.0f}, {0.3f,0.6f,0.4f}, {1.0f,1.0f}}, //0
    { { 6.0f,0.0f,2.0f}, {0.3f,0.6f,0.4f}, {0.0f,1.0f}}, //1
    { { 5.0f,0.0f,1.0f}, {0.3f,0.6f,0.4f}, {0.0f,1.0f}}, //2
    { { 6.0f,0.0f,1.0f}, {0.3f,0.6f,0.4f}, {1.0f,1.0f}}, //3
    { {5.0f,1.0f,2.0f}, {0.3f,0.6f,0.4f}, {1.0f,0.0f}},  //4
    { {6.0f,1.0f,2.0f}, {0.3f,0.6f,0.4f}, {0.0f,0.0f}},  //5
    { {6.0f,1.0f,1.0f}, {0.3f,0.6f,0.4f}, {1.0f,0.0f}},  //6
    { {5.0f,1.0f,1.0f}, {0.3f,0.6f,0.4f}, {0.0f,0.0f}}  //7
    };

    std::vector<GLuint> indices = { 0,1,4,  1,5,4,  1,3,6,  6,5,1,  3,2,7, 7,6,3, 2,0,4, 4,7,2, 4,5,6, 6,7,4, 0,1,3, 3,2,0 };
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    glm::vec3 margin = { 0.15f, 0.15f,0.15f };
    col1 = { min - margin, max + margin };
    globals.colliders.push_back(col1);
}


void make_circle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
    Collider col1;
    std::vector<vertex> vertices = {
   { {0.0f,0.0f,0.0f}, {1.0f,0.0f,0.0f} },
    };
    std::vector<GLuint> indices = { 0 };
    int n = 30;
    float r = 0.6f;
    for (size_t i = 1; i <= n; i++)
    {
        float x = cos(2 * M_PI / n * i) * r;
        float y = sin(2 * M_PI / n * i) * r;
        vertex vert = { {x,y,0.0f}, {1.0f,0.0f,0.0f} };
        vertices.push_back(vert);
        indices.push_back(i);
    };
    float x = cos(2 * M_PI / n * 1) * r;
    float y = sin(2 * M_PI / n * 1) * r;
    vertex vert = { {x,y,0.0f}, {1.0f,0.0f,0.0f} };
    vertices.push_back(vert);
    indices.push_back(n + 1);
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    col1 = { min, max };
    globals.colliders.push_back(col1);
}


void make_checker(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr, size_t number_of_col, size_t number_of_rows) {
    Collider col1;
    std::vector<vertex> vertices = {};
    float start_x = -20.0f;
    float end_x = 20.0f;
    float start_z = 20.0f;
    float end_z = -20.0f;
    float size_rows = -(start_x - end_x) / number_of_rows;
    float size_col = -(-start_z + end_z) / number_of_col;
    glm::vec3 color;
    vertex vert;
    std::vector<GLuint> indices = {};
    for (size_t col = 0; col < number_of_col; col++) {
        for (size_t rows = 0; rows < number_of_rows; rows++) {
            if (rows % 2 == col % 2) {
                color = { 1.0f,1.0f,1.0f };
            }
            else {
                color = { 0.5f, 0.5f, 0.5f };
            }
            vert = { {start_x + size_rows * rows, -1.0f * 0.0f, -1.0f * (start_z - size_col * col)} ,color };
            vertices.push_back(vert);
            indices.push_back(0 + rows * 4 + col * 4 * number_of_rows);
            vert = { {start_x + size_rows * rows, -1.0f * 0.0f, -1.0f * (start_z - size_col - size_col * col)} ,color };
            vertices.push_back(vert);
            indices.push_back(1 + rows * 4 + col * 4 * number_of_rows);
            vert = { {start_x + size_rows + size_rows * rows, -1.0f * 0.0f, -1.0f * (start_z - size_col * col)} ,color };
            vertices.push_back(vert);
            indices.push_back(2 + rows * 4 + col * 4 * number_of_rows);
            vert = { {start_x + size_rows + size_rows * rows, -1.0f * 0.0f, -1.0f * (start_z - size_col - size_col * col)} ,color };
            vertices.push_back(vert);
            indices.push_back(3 + rows * 4 + col * 4 * number_of_rows);
        }
        indices.push_back(4 * number_of_rows - 1 + col * 4 * number_of_rows);
        indices.push_back(1 + col * 4 * number_of_rows);
    }
    *indices_ptr = indices;
    *vertex_ptr = vertices;

    // Calculate AABB collider
    glm::vec3 min = vertices[0].position;
    glm::vec3 max = vertices[0].position;
    for (const auto& v : vertices) {
        min = glm::min(min, v.position);
        max = glm::max(max, v.position);
    }
    glm::vec3 margin = { 0.1f, 0.1f,0.1f };
    col1 = { min-margin, max+margin};
    globals.colliders.push_back(col1);
}


