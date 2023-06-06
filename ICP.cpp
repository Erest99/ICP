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
#include "collider.h"
#include <SFML/Audio.hpp>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "OBJloader.h"

s_globals globals;
Camera camera = Camera();

glm::vec3 computeModelWorldPosition(const std::vector<vertex>& vertices, const glm::mat4& modelMatrix) {
    glm::vec3 center(0.0f, 0.0f, 0.0f);

    // Compute center of the model in local space
    for (const auto& vertex : vertices) {
        center += vertex.position;
    }

    center /= static_cast<float>(vertices.size());

    // Transform the center position into world space using the model matrix
    glm::vec4 centerWorld = modelMatrix * glm::vec4(center, 1.0f);

    // Convert to glm::vec3 and return
    return glm::vec3(centerWorld);
}

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

int main() {
    init_opengl();                                      
    set_all_callbacks();

    std::vector<GLuint> indices_mesh;
    std::vector<vertex> vertex_mesh;
    GLuint shader_mesh;

    //make_shader("resources/basic.vert", "resources/basic.frag", &shader_mesh); 
    make_shaders();

    //Prepare Meshes
    make_triangle(&indices_mesh, &vertex_mesh);  //nahraj data do indicies_mesh a vertex_mesh
    mesh mesh_triangle = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh,GL_TRIANGLES);
    mesh_triangle.add_texture_id("Images/tre.png",false);

    make_triangle2(&indices_mesh, &vertex_mesh);  
    mesh mesh_triangle2 = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_triangle2.add_texture_id("Images/test.png",false);

    make_cube2(&indices_mesh, &vertex_mesh);  
    mesh mesh_cube2 = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_cube2.add_texture_id("Images/transparent.png",true);

    make_cube(&indices_mesh, &vertex_mesh);
    mesh mesh_cube = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);
    mesh_cube.add_texture_id("Images/brick.png",false);

    make_floor(&indices_mesh, &vertex_mesh);
    mesh mesh_floor = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh, GL_TRIANGLES);


    //make_checker(&indices_mesh, &vertex_mesh, 200, 200);
    //mesh mesh_checker = mesh(globals.shader["mesh"], vertex_mesh, indices_mesh,GL_TRIANGLE_STRIP);

    /*
    mesh from_file = mesh();
    createMesh("resources/from_file.obj", globals.shader["texture"], from_file, { 0.0f, 0.0f, 0.0f });
    from_file.add_material({ 0.3f,0.15f,0.0f }, { 0.8f, 0.4f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 30.0);
    from_file.add_texture_id("Images/brick.png",false);
    globals.mesh["from_file"] = from_file;
    */

    int frame_cnt = 0;
    double last_fps = glfwGetTime();

    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    float ratio = static_cast<float>(width) / height;
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(60.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90� (extra wide) and 30� (quite zoomed in)
        ratio,			     // Aspect Ratio. Depends on the size of your window.
        0.1f,                // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        20000.0f              // Far clipping plane. Keep as little as possible.
    );
    


    // set visible area
    glViewport(0, 0, width, height);

    ma_decoder decoder;
    ma_device device;

    if (initialize_audio(&decoder, &device) < 0) {
        return -1;
    }



    while (!glfwWindowShouldClose(globals.window)) {

        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glfwPollEvents();

         

        glm::mat4 v_m = glm::lookAt(camera.position, camera.point_of_sight, camera.up_direction);
        glm::mat4 m_m = glm::identity<glm::mat4>();
        glm::mat4 m_m2 = glm::identity<glm::mat4>();
        glm::mat4 m_m3 = glm::identity<glm::mat4>();
        glm::mat4 transform = glm::identity<glm::mat4>();

        //m_m = glm::scale(m_m, glm::vec3(5.0f));
        //m_m = glm::translate(m_m, glm::vec3(0.0f, 0.0f, 0.0f));
        //glm::vec3 center_of_rotation = glm::vec3(-10.0f, 0.0f, 10.0f);
        //m_m = glm::translate(m_m, center_of_rotation); // Step 1
        //m_m = glm::rotate(m_m, glm::radians(25.0f * std::sin((float)glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
        //m_m = glm::translate(m_m, -center_of_rotation); // Step 3

        m_m = glm::translate(m_m, glm::vec3(3.0f, 0.0f, 4.0f) * 0.5f *std::sin((float)glfwGetTime()));

        //transform = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f, 0.0f, 0.0f));
        //transform = glm::scale(transform, glm::vec3(5.0f));
        //transform = glm::translate(transform, center_of_rotation);
        //transform = glm::rotate(transform, glm::radians(100.0f * std::sin((float)glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
        //transform = glm::translate(transform, -center_of_rotation);

        //glm::vec3 min_transformed = transform * glm::vec4(col1.min, 1.0f);
        //glm::vec3 max_transformed = transform * glm::vec4(col1.max, 1.0f);

        //col1.min = min_transformed;
        //col1.max = max_transformed;



        //m_m3 = glm::scale(m_m3, glm::vec3(3.0f));
        mesh_cube.draw(m_m3, v_m, projectionMatrix);
        mesh_triangle2.draw(m_m3, v_m, projectionMatrix);
        mesh_triangle.draw(m_m, v_m, projectionMatrix);
        //mesh_checker.draw(m_m2, v_m, projectionMatrix);
        mesh_floor.draw(m_m2, v_m, projectionMatrix);
        mesh_cube2.draw(m_m3, v_m, projectionMatrix);
        
        //m_m2 = glm::scale(m_m2, glm::vec3(10.0f));
        //m_m2 = glm::rotate(m_m2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // Moving collider -------------------------
        glm::vec3 colPos = computeModelWorldPosition(mesh_triangle.vertices, m_m);
        glm::vec3 colSize = glm::vec3(2.0f,2.0f,2.0f);

        glm::vec3 minPoint = colPos - (colSize*0.5f);  // Calculate the minimum point of the AABB
        glm::vec3 maxPoint = colPos + (colSize * 0.5f);  // Calculate the maximum point of the AABB

        globals.colliders[0] = {minPoint,maxPoint};


        //---------------------------------------------------

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

