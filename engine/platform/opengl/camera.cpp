#include "camera.h"
#include "utils/Logger.h"

bool leftShiftKeyPressed = false;
void Camera::update(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        Logger logger("camera.log");
        if (uMsg == WM_KEYDOWN && wParam == VK_LSHIFT) {
            leftShiftKeyPressed = true;
        }
        else if (uMsg == WM_KEYUP && wParam == VK_LSHIFT) {
            leftShiftKeyPressed = false;
        }
        // Code
        if (uMsg == WM_MOUSEMOVE) {

            xpos = LOWORD(lParam);
            ypos = HIWORD(lParam);

            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = FALSE;
            }

            xoffset = xpos - lastX;
            yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;


            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch += yoffset;

            if (pitch > 90.0f)
                pitch = 90.0f;
            if (pitch < -90.0f)
                pitch = -90.0f;

            front[0] = cos(yaw) * cos(pitch);
            front[1] = sin(pitch);
            front[2] = sin(yaw) * cos(pitch);
            normalize(front);
        }
        else if (uMsg == WM_KEYDOWN) {

            if (wParam == 'W' || wParam == 'w') {
                pos += speed * front;
            }
            if (wParam == 'S' || wParam == 's') {
                pos -= speed * front;
            }
            if (wParam == 'A' || wParam == 'a') {
                pos -= normalize(cross(front, up)) * speed;
            }
            if (wParam == 'D' || wParam == 'd') {
                pos += normalize(cross(front, up)) * speed;
            }
            if (leftShiftKeyPressed) {
                leftShiftKeyPressed = false;
            }
            if (!leftShiftKeyPressed) {
                leftShiftKeyPressed = true;
            }

            logger.debug("&& leftShiftKeyPressed: ", leftShiftKeyPressed);
        }
        else if (uMsg == WM_MOUSEWHEEL) {
     
            int delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            fov -= static_cast<float>(delta) * 0.5f; // Adjust the scaling factor as needed
            if (fov < 1.0f)
                fov = 1.0f;
            if (fov > 45.0f)
                fov = 45.0f;
        }
    
}

glm::mat4 Camera::getMatrix()
{
    // code
    return glm::lookAt(pos, pos + front, up);
}
