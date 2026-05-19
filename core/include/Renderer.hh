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

    struct ShaderUniforms {
        unsigned int model;
        unsigned int view;
        unsigned int projection;
        unsigned int fov;
    };

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
        
        static void onMouseClick(GLFWwindow* window, int button, int action, int mods);
        void handleMouseClick(int button, int action, int mods);
        
        static void onCursorMove(GLFWwindow* window, double xpos, double ypos);
        void handleCursorMove(double xpos, double ypos);

        static void onScroll(GLFWwindow* window, double xoffset, double yoffset);
        void handleScroll(double xoffset, double yoffset);
        
        void updateCamera(ShaderUniforms uniforms);
        /// @}
            
        /// @name Shader utility methods
        /// @{
        unsigned int loadShader(const char* Path, GLenum shaderType);
        unsigned int createShaderProgram(const char* vertPath, const char* fragPath);
        std::vector<glm::vec3> generateBox(std::pair<glm::dvec3, double> boxData);
        /// @}

    private:
        Simulation& simulation;                                 ///< Reference to the Simulation instance being rendered
        GLFWwindow* window;                                     ///< Pointer to the GLFW window used for rendering
   
        float SCR_WIDTH;                                        ///< Width of the window in pixels, stored for use in projection matrix calculations and resizing
        float SCR_HEIGHT;                                       ///< Height of the window in pixels, stored for use in projection matrix calculations and resizing
        
        bool mousePressed = false;                              ///< Flag to track whether the left mouse button is currently pressed, used for enabling camera rotation based on mouse movement
        bool firstPress = true;                                 ///< Flag to track the initial state of mouse press, used to prevent sudden jumps in camera orientation when the mouse is first pressed
        bool showOctree = false;
        bool octreePressed = false;
        float lastX = 400.0f;                                   ///< Initial last X position of the mouse, set to the center of the window for calculating mouse movement offsets
        float lastY = 400.0f;                                   ///< Initial last Y position of the mouse, set to the center of the window for calculating mouse movement offsets
        
        glm::vec3 cameraPos;                                    ///< Position of the camera in world space
        glm::vec3 cameraFront;                                  ///< Front vector for the camera, used for calculating the view matrix
        glm::vec3 cameraRight;                                  ///< right vector for the camera used for calculating the view matrix
        glm::vec3 cameraUp;                                     ///< Up vector for the camera, used for calculating the view matrix and ensuring correct orientation of the camera in world space
        
        float FOV;                                              ///< Field of view for the perspective projection, used in the projection matrix to control the zoom level of the camera
        float pitch = 0.0f;                                     ///< Initial pitch angle of the camera, used for calculating the camera's front vector based on mouse movement
        float yaw = -90.0f;                                     ///< Initial yaw angle of the camera, set to 90 degrees to face towards the negative Z direction, used for calculating the camera's front vector based on mouse movement
        float cameraSpeed;                                      ///< Speed of camera for keyboard input
        float sensitivity;                                      ///< Speed of camera for mouse input
        float deltaTime = 0.0f;     // keep track of frame generation speed
        float lastFrame = 0.0f;
        
        glm::mat4 model;                                        ///< Model matrix for transforming local to world coordinates
        glm::mat4 view;                                         ///< View matrix for transforming world coordinates to camera view
        glm::mat4 projection;                                   ///< Projection matrix for the perspective projection

        std::map<std::string, unsigned int> shaderPrograms;     ///< Map to store shader program IDs with string keys for easy access when rendering different types of bodies (classical vs quantum)
        std::map<std::string, unsigned int> vertexObjects;      ///< Map to store vertex array and buffer object IDs with string keys for easy access when setting up vertex data for rendering
        std::map<std::string, ShaderUniforms> cameraLoc;        ///< Map to store uniform locations for camera transformation and projection matrices for different shader programs, allowing for easy updates to the camera view during rendering
};
