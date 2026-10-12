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
#include <Shader.hh>
#include <Camera.hh>
#include <optional>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

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
        ~Renderer();

        /// @brief Initializes the GLFW window and OpenGL context for rendering.
        /// @param SCR_WIDTH Width of the window in pixels
        /// @param SCR_HEIGHT Height of the window in pixels
        int setupWindow(int SCR_WIDTH = 800, int SCR_HEIGHT = 800);
        
        /// @name Setup methods
        /// @{ 
        void setupObjects();
        void setupShaders();
        int startRenderLoop();
        void createSkyBox();
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
        void handleScroll(double yoffset);
        
        void applyCamera(Shader& shaderProgram);
        /// @}
            
        /// @name Shader utility methods
        /// @{
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
                                  
        float cameraSpeed = 1.5f;                                      ///< Speed of camera for keyboard input
        float sensitivity = 0.3f;                                      ///< Speed of camera for mouse input
        float deltaTime = 0.0f;     // keep track of frame generation speed
        float lastFrame = 0.0f;
        
        std::optional<Shader> octreeShader, classicalShader;
        std::optional<Shader> quantumShader, skyboxShader;

        Camera camera;

        std::map<std::string, unsigned int> vertexObjects;      ///< Map to store vertex array and buffer object IDs with string keys for easy access when setting up vertex data for rendering

        // skybox
        unsigned int cubemapTexture;
        float skyboxVertices[24] = 
            {
                //   Coordinates
                -1.0f, -1.0f,  1.0f,//        7--------6
                1.0f, -1.0f,  1.0f,//        /|       /|
                1.0f, -1.0f, -1.0f,//       4--------5 |
                -1.0f, -1.0f, -1.0f,//      | |      | |
                -1.0f,  1.0f,  1.0f,//      | 3------|-2
                1.0f,  1.0f,  1.0f,//       |/       |/
                1.0f,  1.0f, -1.0f,//       0--------1
                -1.0f,  1.0f, -1.0f
            };

        unsigned int skyboxIndices[36] = 
            {
                // Right px
                1, 2, 6,
                6, 5, 1,
                // Left nx
                0, 4, 7,
                7, 3, 0,
                // Top py
                4, 5, 6,
                6, 7, 4,
                // Bottom ny
                0, 3, 2,
                2, 1, 0,
                // Back pz
                0, 1, 5,
                5, 4, 0,
                // Front nz
                3, 7, 6,
                6, 2, 3
            };

    };
