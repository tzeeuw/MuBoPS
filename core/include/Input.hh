
# pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// define input structure to use as output of Input and pass to Renderer
struct InputState {
    float moveForward = 0.0f;
    float moveRight = 0.0f;
    float moveUp = 0.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
    float zoom = 0.0f;
    bool toggleOctree = false;
    bool shouldClose = false;
};

class Input {
    public:
        Input(GLFWwindow* window) : window(window) {}

        // avoid copies of the input
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        void setSpeed(float newMoveSpeed) {moveSpeed = newMoveSpeed;}
        void setSens(float newSensitivity) {sensitivity = newSensitivity;}
        void setRollSpeed(float newRollSpeed) {rollSpeed = newRollSpeed;}
        void setScroll(float newScroll) {scroll = newScroll;}
      
        std::pair<float, float> updateMouse();
        InputState update(float deltaTime);

    private:
        GLFWwindow* window;
        float moveSpeed = 1.5f;
        float rollSpeed = 25.0f;
        float sensitivity = 0.3f;
        float scroll = 0.0f;
        bool kPressed = false;

        glm::vec2 mousePos = {0.0f, 0.0f};
        glm::vec2 nextMousePos = {0.0f, 0.0f};

        float axis(int positiveKey, int negativeKey);
};
