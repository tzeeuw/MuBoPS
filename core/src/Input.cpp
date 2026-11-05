#include <Input.hh>


// small private helper function to avoid a long list of if statements
float Input::axis(int positiveKey, int negativeKey) {
    
    int axisTot = 0;

    if (glfwGetKey(window, positiveKey) == GLFW_PRESS) {
        axisTot += 1;
    }
    if (glfwGetKey(window, negativeKey) == GLFW_PRESS) {
        axisTot -= 1;
    }

    return axisTot;
}


std::pair<float, float> Input::updateMouse() {

    // calculate offsets based on yaw and pitch
    float xoffset = nextMousePos.x - mousePos.x;
    float yoffset = mousePos.y - nextMousePos.y; // lowest y is at the bottom so must reverse

    // scale with sensitivity
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    std::pair<float, float> YawPitch(xoffset, yoffset);

    return YawPitch;
}


InputState Input::update(float deltaTime) {

    // declare new input state each time so that defaults are applied automatically
    InputState in;

    // all toggables
    in.shouldClose = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);
    in.toggleOctree = (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS);

    // set speed based on shift key
    // FIXME: moveSpeed and rollSPeed are now set here, fix so that it can be changed in a single setting
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        moveSpeed = 3.0f;
        rollSpeed = 50.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        moveSpeed = 1.5f;
        rollSpeed = 25.0f;
    }

    // use helper function to define the movements
    in.moveForward = axis(GLFW_KEY_W, GLFW_KEY_S) * moveSpeed * deltaTime;
    in.moveRight = axis(GLFW_KEY_D, GLFW_KEY_A) * moveSpeed * deltaTime;
    in.moveUp = axis(GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL) * moveSpeed * deltaTime;
    in.roll = axis(GLFW_KEY_Q, GLFW_KEY_E) * rollSpeed * deltaTime;

    // handle the zooming, because scrolling can only be handled through callbacks, requires different approach
    in.zoom = scroll;
    // set to 0 for the next frame so that zooming is not indefinite
    scroll = 0;

    // handle mouse moving
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    nextMousePos = {xpos, ypos};
    std::pair<float, float> newYP(0.0f, 0.0f);

    // create new yaw and pitch values only if left mouse button is pressed
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        newYP = updateMouse();

        // hide the cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    // show the cursor if left mouse button is released
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    in.yaw = newYP.first;
    in.pitch = newYP.second;

    // update mousePos so that we do not need a flag to avoid jitter (see earlier releases)
    mousePos = nextMousePos;


    return in;
}
