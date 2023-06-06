#include "camera.h"
#include <glm/gtx/string_cast.hpp>
#include <SFML/Audio.hpp>

extern s_globals globals;
extern Camera camera;
bool isSoundPlaying = false;

Camera::Camera()
{
    this->Reset();
}

void Camera::Reset() {
    this->position = glm::vec3(0.0f, 1.5f, 10.0f);
    this->point_of_sight = glm::vec3(0.0f, 1.5f - glm::radians(3.0f), 0.0f);
    this->up_direction = glm::vec3(0, 1, 0);
    glm::vec3 camera_range = this->point_of_sight - this->position;
    double a = camera_range[0];
    double b = camera_range[2];
    this->range = sqrt(a * a + b * b);
    this->Yaw = asin(b / this->range);
    this->Roll = 0.0f;
    this->Pitch = 0.0f;
}

void Camera::Rotate(bool left) {
    if (left)
    {
        this->Yaw = this->Yaw - glm::radians(2.0f);
    }
    else {
        this->Yaw = this->Yaw + glm::radians(2.0f);
    }
    double new_x = cos(this->Yaw) * this->range;
    double new_y = sin(this->Yaw) * this->range;
    this->point_of_sight = glm::vec3({ this->position[0] + new_x, this->point_of_sight[1], this->position[2] + new_y });
}

void Camera::Move(Camera::direction direction) {
    switch (direction)
    {
    case direction::FORWARD:
        this->position = this->position + glm::vec3(0.0, 0.0, -1.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(0.0, 0.0, -1.0);
        return;
    case direction::BACKWARD:
        this->position = this->position + glm::vec3(0.0, 0.0, 1.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(0.0, 0.0, 1.0);
        return;
    case direction::LEFT:
        this->position = this->position + glm::vec3(-1.0, 0.0, 0.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(-1.0, 0.0, 0.0);
        return;
    case direction::RIGHT:
        this->position = this->position + glm::vec3(1.0, 0.0, 0.0);
        this->point_of_sight = this->point_of_sight + glm::vec3(1.0, 0.0, 0.0);
        return;
    }
}

void Camera::Move_with_camera(Camera::direction direction) {

    glm::vec3 camera_range = this->point_of_sight - this->position;
    glm::vec3 dir = glm::normalize(camera_range) * this->MovementSpeed;
    glm::vec3 right = glm::normalize(glm::cross(camera_range, glm::vec3(0.0f, 1.0f, 0.0f))) * this->MovementSpeed;
    glm::vec3 proposed_position = this->position;


    switch (direction)
    {
    case direction::FORWARD:
        proposed_position += dir;
        break;
    case direction::BACKWARD:
        proposed_position -= dir;
        break;
    case direction::LEFT:
        proposed_position -= right;
        break;
    case direction::RIGHT:
        proposed_position += right;
        break;
    }

    // Check for collision and resolve it if necessary.
    check_collision(proposed_position);

    // Apply limits.
    if (proposed_position.y < 0.3f)proposed_position.y = 0.3f;
    if (proposed_position.y > 35.0f)proposed_position.y = 35.0f;
    if (proposed_position.z < -20.0f)proposed_position.z = -20.0f;
    if (proposed_position.z > 20.0f)proposed_position.z = 20.0f;
    if (proposed_position.x < -20.0f)proposed_position.x = -20.0f;
    if (proposed_position.x > 20.0f)proposed_position.x = 20.0f;

    this->position = proposed_position;
    this->point_of_sight = this->position + camera_range;
}


bool Camera::check_collision(glm::vec3& proposed_position)
{
    for (const Collider& collider : globals.colliders)
    {
        // Check each axis for a collision
        //std::cout << "Proposed position: " << glm::to_string(proposed_position) << "\n";

        if (proposed_position.x > collider.min.x && proposed_position.x < collider.max.x &&
            proposed_position.y > collider.min.y && proposed_position.y < collider.max.y &&
            proposed_position.z > collider.min.z && proposed_position.z < collider.max.z)
        {
            // We are inside the box. Compute displacements in each direction.
            float displacements[6] = {
                fabs(proposed_position.x - collider.min.x),
                fabs(proposed_position.x - collider.max.x),
                fabs(proposed_position.y - collider.min.y),
                fabs(proposed_position.y - collider.max.y),
                fabs(proposed_position.z - collider.min.z),
                fabs(proposed_position.z - collider.max.z)
            };

            // Find the direction with the smallest displacement
            int smallestDisplacementIndex = 0;
            for (int i = 1; i < 6; i++) {
                if (displacements[i] < displacements[smallestDisplacementIndex]) {
                    smallestDisplacementIndex = i;
                }
            }

            // Move the camera in the direction of the smallest displacement
            switch (smallestDisplacementIndex) {
            case 0: proposed_position.x = collider.min.x; break;
            case 1: proposed_position.x = collider.max.x; break;
            case 2: proposed_position.y = collider.min.y; break;
            case 3: proposed_position.y = collider.max.y; break;
            case 4: proposed_position.z = collider.min.z; break;
            case 5: proposed_position.z = collider.max.z; break;
            }
            return true;

        }
    }
    return false;

}

void Camera::check_collisionState()
{
    glm::vec3 camera_range = this->point_of_sight - this->position;
    glm::vec3 proposed_position = this->position;
    if (check_collision(proposed_position))
    {
        // Apply limits.
        if (proposed_position.y < 0.3f)proposed_position.y = 0.3f;
        if (proposed_position.y > 35.0f)proposed_position.y = 35.0f;
        if (proposed_position.z < -20.0f)proposed_position.z = -20.0f;
        if (proposed_position.z > 20.0f)proposed_position.z = 20.0f;
        if (proposed_position.x < -20.0f)proposed_position.x = -20.0f;
        if (proposed_position.x > 20.0f)proposed_position.x = 20.0f;

        this->position = proposed_position;
        this->point_of_sight = this->position + camera_range;
    }
}

void mouseMove(GLFWwindow* window)
{
    // Capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    double mouseX, mouseY;
    bool outW = false;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Get the window size
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);



    // Calculate the mouse movement delta
    static double lastMouseX = mouseX;
    static double lastMouseY = mouseY;
    double deltaX = mouseX - lastMouseX;
    double deltaY = mouseY - lastMouseY;
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    // Adjust camera rotation based on mouse movement
    const float sensitivity = 0.3f;
    deltaX *= sensitivity;
    deltaY *= sensitivity;
    camera.Yaw += glm::radians(static_cast<float>(deltaX));
    camera.Pitch += glm::radians(static_cast<float>(deltaY));

    // Clamp pitch to avoid flipping
    const float maxPitch = 89.0f;
    const float minPitch = -89.0f;
    camera.Pitch = glm::clamp(camera.Pitch, glm::radians(minPitch), glm::radians(maxPitch));

    // Update the camera direction
    glm::vec3 direction;
    direction.x = cos(camera.Yaw) * cos(camera.Pitch);
    direction.y = -sin(camera.Pitch);
    direction.z = sin(camera.Yaw) * cos(camera.Pitch);
    camera.point_of_sight = camera.position + glm::normalize(direction);
}


void updateInput(GLFWwindow* window, ma_device* device, ma_decoder* decoder) {
    double now = glfwGetTime();
    if (now - globals.last_update > 0.01) {
        globals.last_update = now;
        bool movement = false;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::FORWARD);
            movement = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::LEFT);
            movement = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::BACKWARD);
            movement = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.Move_with_camera(Camera::direction::RIGHT);
            movement = true;
        }

        // If there is movement, play sound.
        if (movement) {
            ma_uint64 currentFrame, totalFrames;
            ma_result resultCurrent = ma_decoder_get_cursor_in_pcm_frames(decoder, &currentFrame);
            ma_result resultTotal = ma_decoder_get_length_in_pcm_frames(decoder, &totalFrames);
            if (resultCurrent != MA_SUCCESS || resultTotal != MA_SUCCESS) {
                printf("Failed to get current frame.\n");
                ma_device_uninit(device);
                ma_decoder_uninit(decoder);
            }
            else {  //pokud nahrávka zkoncila restart a try again
                if (currentFrame >= totalFrames) {
                    ma_decoder_seek_to_pcm_frame(decoder, 0);
                }

                if (ma_device_is_started(device) == MA_FALSE) {
                    if (ma_device_start(device) != MA_SUCCESS) {
                        printf("Failed to start playback device.\n");
                        ma_device_uninit(device);
                        ma_decoder_uninit(decoder);
                    }
                }
            }
        }
        else {
            if (ma_device_is_started(device) == MA_TRUE) {
                ma_device_stop(device);
                ma_decoder_seek_to_pcm_frame(decoder, 0);
            }
        }

        // Mouse input
        mouseMove(window);
    }

    camera.check_collisionState();


}
