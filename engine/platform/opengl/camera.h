#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>

class Camera {
    
    // code
    public:
        // function declaration
        void update(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        glm::mat4 getMatrix();
        float fov = 45.0f;


    private:
        // mouse variable declaration
        double xpos;
        double ypos;
        float xoffset;
        float yoffset;

        // mouse sensitivity
        float sensitivity = 0.001f;

        // camera speed with keyboard
        const float speed = 0.05f;

        // camera initial positon
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        // Initial Mouse postion
        float lastX = 400, lastY = 300, yaw = -90.0f, pitch;
        bool firstMouse = true;
    
    };
