/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

#include <Camera.hh>


// input is in degrees
void Camera::rotate(float yaw, float pitch, float roll) {

    // roll rotations
    glm::mat4 rollRot = glm::rotate(glm::mat4(1.0f), glm::radians(-roll), front);
    
    // yaw and pitch rotations
    glm::mat4 yawRot = glm::rotate(glm::mat4(1.0f), glm::radians(-yaw), up);
    glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), right);
    
    front = glm::vec3(yawRot * pitchRot * glm::vec4(front, 0.0f));
    up = glm::normalize(glm::vec3(rollRot * glm::vec4(up, 0.0f)));

    // normalize so that the camera does not drift over time and enforce them to be orthogonal
    orthonormalize();
}

// positive delta zooms in
void Camera::zoom(double delta) {
    FOV -= delta;

    // force FOV between two values so that zoom is contained and can not be 0 or infinite
    if (FOV < 1.0f) {
        FOV = 1.0f;
    }
    else if (FOV > 60.0f) {
        FOV = 60.0f;
    }
}

void Camera::orthonormalize() {

    // normalize the 3 basis vectors of the camera
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, up));

    // recompute up so that it does not drift over time
    up = glm::normalize(glm::cross(right, front));
}
