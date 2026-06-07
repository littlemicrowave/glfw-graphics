#pragma once
#include "glm-master/glm/glm.hpp"
#include "glm-master/glm/gtc/matrix_transform.hpp"
#include "glm-master/glm/gtc/random.hpp"

struct Keystates
{
    union
    {
        struct
        {
            unsigned char W : 1;
            unsigned char S : 1;
            unsigned char A : 1;
            unsigned char D : 1;
            unsigned char E : 1;
            unsigned char R : 1;
        };

        unsigned char all = 0;
    };
};

class Camera
{
    float pitch;
    float yaw;
    float roll;
    float speed;
    float mouse_speed;
    float fov;
    glm::mat4 viewMatrix;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 right;
    public:
        glm::vec3 world_up;
        double cursor_x;
        double cursor_y;
        struct Keystates keys;

        bool active;

        Camera(glm::vec3 pos = { 1.f, 1.f, -2.f },
            float pitch = -45.f,
            float yaw = 110.f,
            float roll = 0.f,
            float speed = 2.f,
            float mouse_speed = 50.f,
            float fov = 45.f): position(pos), pitch(pitch), yaw(yaw), roll(roll), speed(speed), mouse_speed(mouse_speed), fov(fov)
        {
            world_up = glm::vec3(0.f, 1.f, 0.f);
            viewMatrix = glm::mat4(1.f);
            cursor_x = 0.f;
            cursor_y = 0.f;
            active = false;
            UpdateDirection(0, 0, 0);
            UpdateViewMatrix();
        }

        void Move(float frametime);
        void UpdateViewMatrix();
        void UpdateDirection(double, double, float);
        const glm::mat4 GetViewMatrix() { return viewMatrix; };
};

