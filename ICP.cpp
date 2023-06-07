#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <chrono>
#include <GL/glew.h>
#include <glm/ext.hpp> 
#include "globals.h"
#include "init.h"
#include "callbacks.h"
#include "shaders.h"
#include "verticies.h"
#include "mesh.h"
#include "camera.h"
#include "texture.h"
#include <SOIL2.h>
#include <cmath>
#include "collision.h"
#include "collider.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "OBJloader.h"

s_globals globals;
Camera camera = Camera();

std::vector<GLuint> indices_mesh;
std::vector<vertex> vertex_mesh;

mesh mesh_triangle;
mesh from_file;
mesh mesh_triangle2;
mesh mesh_teapot;
mesh mesh_cube2;
mesh mesh_cube;
mesh mesh_floor;

glm::mat4 v_m = glm::lookAt(camera.position, camera.point_of_sight, camera.up_direction);
glm::mat4 m_m = glm::identity<glm::mat4>();
glm::mat4 m_m2 = glm::identity<glm::mat4>();
glm::mat4 m_m3 = glm::identity<glm::mat4>();
glm::mat4 m_m4 = glm::identity<glm::mat4>();
glm::mat4 m_m5 = glm::identity<glm::mat4>();
glm::mat4 m_m6 = glm::identity<glm::mat4>();
glm::mat4 transform = glm::identity<glm::mat4>();


void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
int initialize_audio(ma_decoder* pDecoder, ma_device* pDevice);
void prepare_meshes();
void calculate_moving_collider();

int main() {
    init_opengl();                                      
    set_all_callbacks();
    make_shaders();
    prepare_meshes();

    GLuint shader_mesh;
    int frame_cnt = 0;
    double last_fps = glfwGetTime();

    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    float ratio = static_cast<float>(width) / height;
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90 (extra wide) and 30 (quite zoomed in)
        ratio,			     // Aspect Ratio. Depends on the size of your window.
        0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        20000.0f              // Far clipping plane. Keep as little as possible.
    );
    
    globals.projectionMatrix = projectionMatrix;

    // set visible area
    glViewport(0, 0, width, height);

    //inicializace audia
    ma_decoder decoder;
    ma_device device;

    if (initialize_audio(&decoder, &device) < 0) {
        return -1;
    }

    //translate static models
    m_m4 = glm::translate(m_m4, glm::vec3(-10.0f, 1.0f, -4.0f));
    m_m6 = glm::translate(m_m6, glm::vec3(11.0f, 0.3f, 8.0f));


    while (!glfwWindowShouldClose(globals.window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glfwPollEvents();

        //reset dynamic matrixes
        m_m = glm::identity<glm::mat4>();
        m_m5 = glm::identity<glm::mat4>();
        v_m = glm::lookAt(camera.position, camera.point_of_sight, camera.up_direction);
        transform = glm::identity<glm::mat4>();
         
        //translate dynamic models
        m_m = glm::translate(m_m, glm::vec3(3.0f, 0.0f, 8.0f) * 0.5f *std::sin((float)glfwGetTime()));
        m_m5 = glm::translate(m_m5, glm::vec3(0.0f, 3.0f, 0.0f) * std::abs(std::sin((float)glfwGetTime())));

        //draw models
        mesh_cube.draw(m_m3, v_m, globals.projectionMatrix);
        mesh_triangle2.draw(m_m5, v_m, globals.projectionMatrix);
        mesh_triangle.draw(m_m, v_m, globals.projectionMatrix);
        mesh_teapot.draw_with_material(m_m6, v_m, globals.projectionMatrix, glm::uvec3(15.0f, 10.0f, 15.0f));
        mesh_floor.draw(m_m2, v_m, globals.projectionMatrix);
        from_file.draw(m_m4, v_m, globals.projectionMatrix);
        mesh_cube2.draw(m_m3, v_m, globals.projectionMatrix);
        
        //recalculate colliders for moving objects (and file loaded)
        calculate_moving_collider();
        updateInput(globals.window, &device, &decoder);

        glfwSwapBuffers(globals.window);
        frame_cnt += 1;
        double now = glfwGetTime();
        if (now - last_fps > 1.0) {
            std::cout << frame_cnt << "FPS\r";
            frame_cnt = 0;
            last_fps = now;
        }
    }
    finalize(1);
    return(EXIT_SUCCESS);
};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_uint64 framesRead = 0;
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, &framesRead);

    (void)pInput;   // Unused.
}

int initialize_audio(ma_decoder* pDecoder, ma_device* pDevice) {
    ma_result result;
    ma_device_config deviceConfig;

    result = ma_decoder_init_file("audio/scifi.wav", NULL, pDecoder);
    if (result != MA_SUCCESS) {
        return -1;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = pDecoder->outputFormat;
    deviceConfig.playback.channels = pDecoder->outputChannels;
    deviceConfig.sampleRate = pDecoder->outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = pDecoder;

    if (ma_device_init(NULL, &deviceConfig, pDevice) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(pDecoder);
        return -1;
    }

    return 0;
}

void prepare_meshes()
{
    make_triangle(&indices_mesh, &vertex_mesh);  //nahraj data do indicies_mesh a vertex_mesh
    mesh_triangle = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_triangle.add_texture_id("Images/tre.png", false);

    from_file = mesh();
    createMesh("resources/box.obj", globals.shader["texture"], from_file, { 0.0f, 0.0f, 0.0f });
    from_file.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);
    from_file.add_texture_id("Images/brick.png", false);


    make_triangle2(&indices_mesh, &vertex_mesh);
    mesh_triangle2 = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_triangle2.add_texture_id("Images/test2.png", false);

    mesh_teapot = mesh();
    createMesh("resources/teapot.obj", globals.shader["light"], mesh_teapot, { 0.0f,1.0f,0.0f });
    mesh_teapot.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);

    make_cube2(&indices_mesh, &vertex_mesh);
    mesh_cube2 = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_cube2.add_texture_id("Images/transparent.png", true);

    make_cube(&indices_mesh, &vertex_mesh);
    mesh_cube = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_cube.add_texture_id("Images/brick.png", false);

    make_floor(&indices_mesh, &vertex_mesh);
    mesh_floor = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_floor.add_texture_id("Images/wood.jpeg", false);


}

void calculate_moving_collider()
{
    glm::vec3 colPos = computeModelWorldPosition(mesh_triangle.vertices, m_m);
    glm::vec3 colSize = glm::vec3(2.0f, 3.0f, 2.0f);

    glm::vec3 minPoint = colPos - (colSize * 0.5f);  // Calculate the minimum point of the AABB
    glm::vec3 maxPoint = colPos + (colSize * 0.5f);  // Calculate the maximum point of the AABB

    globals.colliders[0] = { minPoint,maxPoint };

    colPos = computeModelWorldPosition(from_file.vertices, m_m4);
    colSize = glm::vec3(2.2f, 2.2f, 2.2f);
    minPoint = colPos - (colSize * 0.5f);  
    maxPoint = colPos + (colSize * 0.5f);  

    globals.colliders[1] = { minPoint,maxPoint };

    colPos = computeModelWorldPosition(mesh_triangle2.vertices, m_m5);
    colSize = glm::vec3(1.2f, 2.0f, 1.2f);
    minPoint = colPos - (colSize * 0.5f);  
    maxPoint = colPos + (colSize * 0.5f);  

    globals.colliders[2] = { minPoint,maxPoint };

    colPos = computeModelWorldPosition(mesh_teapot.vertices, m_m6);
    colSize = glm::vec3(2.5f, 1.3f, 1.7f);
    minPoint = colPos - (colSize * 0.5f);  
    maxPoint = colPos + (colSize * 0.5f);  

    globals.colliders[3] = { minPoint,maxPoint };

}

