#include "verticies.h"
#define M_PI       3.14159265358979323846   // pi

extern s_globals globals;

void make_triangle(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {

    Collider col1;
    std::vector<vertex> vertices = {
        // Top
        {{ 0.0f,  2.0f,  3.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.5f}},

        // Base
        {{-1.0f, -0.0f,  2.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 0.0f}},
        {{ 1.0f, -0.0f,  2.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.5f}},
        {{ 1.0f, -0.0f,  4.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.5f, 1.0f}},
        {{-1.0f, -0.0f,  4.0f}, {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f}},
    };

    std::vector<GLuint> indices = {
    0, 2, 1,
    0, 3, 2,
    0, 4, 3,
    0, 1, 4,
    2, 4, 1,
    4, 2, 3
    };

    for (int i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = vertices[indices[i + 0]].position;
        glm::vec3 v1 = vertices[indices[i + 1]].position;
        glm::vec3 v2 = vertices[indices[i + 2]].position;

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        // Assign the computed normal to each vertex normal
        vertices[indices[i + 0]].normal = normal;
        vertices[indices[i + 1]].normal = normal;
        vertices[indices[i + 2]].normal = normal;
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
    col1 = { min, max };
    globals.colliders.push_back(col1);
}

void make_floor(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {

    Collider col1;
    std::vector<vertex> vertices = {
        {{ -20.0f,  0.0f,  -20.0f}, {0.3f, 0.2f, 0.45f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 20.0f, 0.0f,  -20.0f}, {0.4f, 0.2f, 0.4f}, {0.0f, 1.0f, 0.0f}, {50.0f, 0.0f}},
        {{ -20.0f, 0.0f,  20.0f}, {0.4f, 0.2f, 0.4f}, {0.0f, 1.0f, 0.0f}, {50.0f, 50.0f}},
        {{ 20.0f, 0.0f,  20.0f}, {0.6f, 0.2f, 0.4f}, {0.0f, 1.0f, 0.0f}, {0.0f, 50.0f}},
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
        //coordinates        //color           //normal, will be overwritten       //texture coords
        { { 10.0f, 1.0f, 0.0f}, {0.8f,0.6f,0.1f}, {0.0f, 0.0f, 0.0f}, {0.5f,0.5f} },
        { { 9.5f, -0.0f, 0.5f}, {0.8f,0.6f,0.1f}, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f}},
        { { 9.5f, -0.0f, -0.5f}, {0.8f,0.6f,0.1f}, {0.0f, 0.0f, 0.0f}, {1.0f,1.0f}},
        { { 10.5f, -0.0f, -0.5f}, {0.8f,0.6f,0.1f}, {0.0f, 0.0f, 0.0f}, {0.0f,1.0f}},
        { { 10.5f, -0.0f, 0.5f}, {0.8f,0.6f,0.1f}, {0.0f, 0.0f, 0.0f}, {1.0f,0.0f}}
    };

    // Calculate normals for each face
    glm::vec3 normal_0 = glm::normalize(glm::cross(vertices[2].position - vertices[0].position, vertices[1].position - vertices[0].position));
    glm::vec3 normal_1 = glm::normalize(glm::cross(vertices[1].position - vertices[0].position, vertices[4].position - vertices[0].position));
    glm::vec3 normal_2 = glm::normalize(glm::cross(vertices[4].position - vertices[0].position, vertices[3].position - vertices[0].position));
    glm::vec3 normal_3 = glm::normalize(glm::cross(vertices[3].position - vertices[0].position, vertices[2].position - vertices[0].position));
    glm::vec3 normal_4 = glm::normalize(glm::cross(vertices[1].position - vertices[2].position, vertices[3].position - vertices[2].position));

    // Assign the normal to each vertex of each face
    vertices[0].normal = normal_0;
    vertices[1].normal = normal_1;
    vertices[2].normal = normal_2;
    vertices[3].normal = normal_3;
    vertices[4].normal = normal_4;

    std::vector<GLuint> indices = { 0,2,1, 0,1,4, 0,4,3, 0,3,2, 2,3,4, 1,2,4 };
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
        // Position         // Color         // Normal (will be overwritten) // Texture Coords
        { { 1.0f,0.0f,2.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {1.0f,1.0f}}, //0
        { { 2.0f,0.0f,2.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {0.0f,1.0f}}, //1
        { { 1.0f,0.0f,1.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {0.0f,1.0f}}, //2
        { { 2.0f,0.0f,1.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {1.0f,1.0f}}, //3
        { {1.0f,1.0f,2.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {1.0f,0.0f}},  //4
        { {2.0f,1.0f,2.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f}},  //5
        { {2.0f,1.0f,1.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {1.0f,0.0f}},  //6
        { { 1.0f,1.0f,1.0f}, {0.0f,0.6f,0.8f}, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f}}  //7
    };

    // Calculate normals for each face and assign to each vertex
    std::vector<glm::vec3> face_normals = {
        glm::normalize(glm::cross(vertices[1].position - vertices[0].position, vertices[4].position - vertices[0].position)), // 0, 1, 4
        glm::normalize(glm::cross(vertices[3].position - vertices[1].position, vertices[6].position - vertices[1].position)), // 1, 3, 6
        glm::normalize(glm::cross(vertices[2].position - vertices[3].position, vertices[7].position - vertices[3].position)), // 3, 2, 7
        glm::normalize(glm::cross(vertices[0].position - vertices[2].position, vertices[4].position - vertices[2].position)), // 2, 0, 4
        glm::normalize(glm::cross(vertices[5].position - vertices[4].position, vertices[6].position - vertices[4].position)), // 4, 5, 6
        glm::normalize(glm::cross(vertices[1].position - vertices[0].position, vertices[3].position - vertices[0].position))  // 0, 1, 3
    };

    // Assign normals to vertices
    for (int i = 0; i < 8; ++i) {
        vertices[i].normal = face_normals[i / 2];
    }

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

void make_cube2(std::vector<GLuint>* indices_ptr, std::vector<vertex>* vertex_ptr) {
    Collider col1;
    std::vector<vertex> vertices = {
        // Position         // Color         // Normal (will be overwritten) // Texture Coords
        { { 5.0f,0.0f,2.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {1.0f,1.0f}}, //0
        { { 6.0f,0.0f,2.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {0.0f,1.0f}}, //1
        { { 5.0f,0.0f,1.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {0.0f,1.0f}}, //2
        { { 6.0f,0.0f,1.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {1.0f,1.0f}}, //3
        { {5.0f,1.0f,2.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {1.0f,0.0f}},  //4
        { {6.0f,1.0f,2.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f}},  //5
        { {6.0f,1.0f,1.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {1.0f,0.0f}},  //6
        { {5.0f,1.0f,1.0f}, {0.3f,0.6f,0.4f}, {0.0f, 0.0f, 0.0f}, {0.0f,0.0f}}  //7
    };

    // Calculate normals for each face and assign to each vertex
    std::vector<glm::vec3> face_normals = {
        glm::normalize(glm::cross(vertices[1].position - vertices[0].position, vertices[4].position - vertices[0].position)), // 0, 1, 4
        glm::normalize(glm::cross(vertices[3].position - vertices[1].position, vertices[6].position - vertices[1].position)), // 1, 3, 6
        glm::normalize(glm::cross(vertices[2].position - vertices[3].position, vertices[7].position - vertices[3].position)), // 3, 2, 7
        glm::normalize(glm::cross(vertices[0].position - vertices[2].position, vertices[4].position - vertices[2].position)), // 2, 0, 4
        glm::normalize(glm::cross(vertices[5].position - vertices[4].position, vertices[6].position - vertices[4].position)), // 4, 5, 6
        glm::normalize(glm::cross(vertices[1].position - vertices[0].position, vertices[3].position - vertices[0].position))  // 0, 1, 3
    };

    // Assign normals to vertices
    for (int i = 0; i < 8; ++i) {
        vertices[i].normal = face_normals[i / 2];
    }

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


