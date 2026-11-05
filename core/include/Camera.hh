/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    public:


        glm::mat4 getProjection(float aspect) const {return glm::perspective(glm::radians(FOV), aspect, 0.1f, 100.0f);}
        glm::mat4 getView() const {return glm::lookAt(glm::vec3(0.0f), front, up);}
        glm::dvec3 getPosition() const {return position;}
        float getFOV() const {return FOV;}


        void moveForward(float distance) {position += distance * front;}
        void moveRight(float distance) {position += distance * right;}
        void moveUp(float distance) {position += distance * up;}

        void rotate(float yaw, float pitch, float roll);
        void zoom(double delta);

    private:

        void orthonormalize();

        // double precision for position vector as simulation is centered around the origin
        glm::dvec3 position = glm::vec3(0.0f, 0.0f, 3.0f);          ///< Position of the camera in world space
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);             ///< Front vector for the camera, used for calculating the view matrix
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);                 ///< Up vector for the camera, used for calculating the view matrix and ensuring correct orientation of the camera in world space
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);              ///< right vector for the camera used for calculating the view matrix

        // initial camera values
        float FOV = 45.0;                                           ///< Field of view for the perspective projection, used in the projection matrix to control the zoom level of the camera
};
