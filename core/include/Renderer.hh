/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/

/**
 * @file Renderer.hh
 * @brief Class responsible for rendering the MuBoPS simulation using OpenGL.
 * 
 * The Renderer class manages the OpenGL context, shaders, and rendering loop for visualizing the simulation.
 * It handles window creation, input processing, camera setup, and rendering of both classical and quantum bodies based on the data provided by the Simulation class.
 * The class also includes callback functions for window resizing and mouse input to allow for interactive control of the camera.
 */
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Simulation.hh>
#include <map>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class Renderer
 * @brief Manages OpenGL rendering for the MuBoPS simulation.
 * 
 * The Renderer class is responsible for setting up the OpenGL context, compiling shaders, and rendering the simulation data.
 * It includes methods for handling user input, resizing the window, and updating the camera view based on mouse interactions.
 * The class interacts with the Simulation class to retrieve data for rendering both classical and quantum bodies in the simulation.
 */
class Renderer {
    public:

        /// @brief Constructs a Renderer with a reference to the Simulation instance.
        /// @param simulation Reference to the Simulation instance to be rendered
        Renderer(Simulation& simulation): simulation(simulation) {};
        ~Renderer() {};

        /// @brief Initializes the GLFW window and OpenGL context for rendering.
        /// @param SCR_WIDTH Width of the window in pixels
        /// @param SCR_HEIGHT Height of the window in pixels
        int setupWindow(int SCR_WIDTH = 800, int SCR_HEIGHT = 800);
        
        /// @name Setup methods
        /// @{ 
        void setupObjects();
        void setupCamera();
        void setupShaders();
        int startRenderLoop();
        /// @}
        
        /// @name Event handling methods
        /// @{
        void processInput(GLFWwindow* window);
        
        static void onResize(GLFWwindow* window, int width, int height);
        void handleResize(int width, int height);
        
        static void onMouseClick(GLFWwindow* window, int i, int d, int k);
        void handleMouseClick(int button, int action, int mods);
        
        static void onCursorMove(GLFWwindow* window, double xpos, double ypos);
        void handleCursorMove(double xpos, double ypos);
        
        void updateCamera(unsigned int projectionLoc, unsigned int transformLoc);
        /// @}
            
        /// @name Shader utility methods
        /// @{
        unsigned int loadShader(const char* Path, GLenum shaderType);
        unsigned int createShaderProgram(const char* vertPath, const char* fragPath);
        /// @}

    private:
        Simulation& simulation;                                 ///< Reference to the Simulation instance being rendered
        GLFWwindow* window;                                     ///< Pointer to the GLFW window used for rendering

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      ///< Initial position of the camera in world space
        float cameraDistance = glm::length(cameraPos);          ///< Distance of the camera from the origin, used for calculating camera movement based on mouse input
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   ///< Initial direction the camera is facing, used for calculating the view matrix
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       ///< Up vector for the camera, used for calculating the view matrix and ensuring correct orientation of the camera in world space
        
        bool mousePressed = false;                              ///< Flag to track whether the left mouse button is currently pressed, used for enabling camera rotation based on mouse movement
        bool notPressed = true;                                 ///< Flag to track the initial state of mouse press, used to prevent sudden jumps in camera orientation when the mouse is first pressed
        float lastX = 400.0f;                                   ///< Initial last X position of the mouse, set to the center of the window for calculating mouse movement offsets
        float lastY = 400.0f;                                   ///< Initial last Y position of the mouse, set to the center of the window for calculating mouse movement offsets
        float pitch = 0.0f;                                     ///< Initial pitch angle of the camera, used for calculating the camera's front vector based on mouse movement
        float yaw = 90.0f;                                      ///< Initial yaw angle of the camera, set to 90 degrees to face towards the negative Z direction, used for calculating the camera's front vector based on mouse movement
        float SCR_WIDTH;                                        ///< Width of the window in pixels, stored for use in projection matrix calculations and resizing
        float SCR_HEIGHT;                                       ///< Height of the window in pixels, stored for use in projection matrix calculations and resizing
        float fov = 45.0;                                       ///< Field of view for the perspective projection, used in the projection matrix to control the zoom level of the camera

        glm::mat4 trans = glm::mat4(1.0f);                      ///< Transformation matrix for the camera view, initialized to the identity matrix and updated based on camera position and orientation
        glm::mat4 projection = glm::mat4(1.0f);                 ///< Projection matrix for the perspective projection, initialized to the identity matrix and updated based on field of view and aspect ratio

        std::map<std::string, unsigned int> shaderPrograms;     ///< Map to store shader program IDs with string keys for easy access when rendering different types of bodies (classical vs quantum)
        std::map<std::string, unsigned int> vertexObjects;      ///< Map to store vertex array and buffer object IDs with string keys for easy access when setting up vertex data for rendering
        std::map<std::string, std::pair<unsigned int, unsigned int>> cameraLoc;         ///< Map to store uniform locations for camera transformation and projection matrices for different shader programs, allowing for easy updates to the camera view during rendering
};
