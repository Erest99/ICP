#include "mesh.h"
#include <SOIL2.h>


mesh::mesh(GLuint shader_type, std::vector<vertex>& vertices, std::vector<GLuint>& indices, GLuint primitive)
:vertices(vertices), indices(indices), shader_id(shader_type), primitive(primitive)
{
    GLuint VAO;
    GLuint VBO, EBO;


    // Generate the VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind VAO (set as the current)
    glBindVertexArray(VAO);
    // Bind the VBO, set type as GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Fill-in data into the VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_DYNAMIC_DRAW);
    // Bind EBO, set type GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Fill-in data into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_DYNAMIC_DRAW);

    // Set Vertex Attribute to explain OpenGL how to interpret the VBO
    /*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, position)));
    // Enable the Vertex Attribute 0 = position
    glEnableVertexAttribArray(0);
    // Set end enable Vertex Attribute 1 = Vertex Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, color)));
    glEnableVertexAttribArray(1);
    // Set end enable Vertex Attribute 2 = Texture Coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, texcoord)));
    glEnableVertexAttribArray(2);
    */

    // Set Vertex Attribute to explain OpenGL how to interpret the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, position)));
    // Enable the Vertex Attribute 0 = position
    glEnableVertexAttribArray(0);
    // Set end enable Vertex Attribute 1 = Vertex Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, color)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(0 + offsetof(vertex, texcoord)));
    glEnableVertexAttribArray(3);

    // Bind VBO and VAO to 0 to prevent unintended modification
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->VAO = VAO;
    


}

void mesh::add_texture_id(const char* path, bool transp) {
    //this->texture_id = generate_texture(path, false, false);
    this->texture_id = textureInit(path, false, transp);
}

void mesh::draw(const glm::mat4& M, const glm::mat4& V, const glm::mat4& P) {
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(M));

    //set texture unit
    glActiveTexture(GL_TEXTURE0);
    //send texture unit number to FS
    glUniform1i(glGetUniformLocation(shader_id, "texture0"), 0);
    // draw object using VAO (Bind+DrawElements+Unbind)
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mesh::draw_with_material(const glm::mat4& M,const glm::mat4& V, const glm::mat4& P, const glm::vec3& light_position) {
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uP_m"), 1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uV_m"), 1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(glGetUniformLocation(shader_id, "uM_m"), 1, GL_FALSE, glm::value_ptr(M));

    glUniform3fv(glGetUniformLocation(shader_id, "ambient_material"), 1, glm::value_ptr(ambient_material));
    glUniform3fv(glGetUniformLocation(shader_id, "diffuse_material"), 1, glm::value_ptr(diffuse_material));
    glUniform3fv(glGetUniformLocation(shader_id, "specular_material"), 1, glm::value_ptr(specular_material));
    glUniform3fv(glGetUniformLocation(shader_id, "light_position"), 1, glm::value_ptr(light_position));
    glUniform1f(glGetUniformLocation(shader_id, "specular_shinines"), specular_shinines);

    //set texture unit
    glActiveTexture(GL_TEXTURE0);
    //send texture unit number to FS
    glUniform1i(glGetUniformLocation(shader_id, "texure0"), 0);
    // draw object using VAO (Bind+DrawElements+Unbind)
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mesh::add_material(glm::vec3 ambient_material, glm::vec3 diffuse_material, glm::vec3 specular_material, float specular_shinines) {
    this->ambient_material = ambient_material;
    this->diffuse_material = diffuse_material;
    this->specular_material = specular_material;
    this->specular_shinines = specular_shinines;
}


