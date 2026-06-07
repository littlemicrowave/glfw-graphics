#include "Camera.h"
#include <iostream>


void Camera::UpdateViewMatrix()
{
   right = glm::normalize(glm::cross(direction, world_up));
   up = glm::normalize(glm::cross(right, direction));
   viewMatrix = glm::lookAt(position, position + direction, up);
}


void Camera::Move(float frametime)
{
    if (!keys.all)
        return;
    position += direction * speed * (frametime * keys.W);
    position -= direction * speed * (frametime * keys.S);
    position -= right * speed * (frametime * keys.A);
    position += right * speed * (frametime * keys.D);
    position += up * speed * (frametime * keys.E);
    position -= up * speed * (frametime * keys.R);
    UpdateViewMatrix();

}

void Camera::UpdateDirection(double new_xpos, double new_ypos, float frametime)
{
        
    
    float xoffset = new_xpos - cursor_x;
    float yoffset = cursor_y - new_ypos;

    cursor_x = new_xpos;
    cursor_y = new_ypos;

    xoffset *= mouse_speed * frametime;
    yoffset *= mouse_speed * frametime;

    yaw += xoffset;
    pitch += yoffset;
       if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    UpdateViewMatrix();
}