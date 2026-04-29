#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <ClassicalBody.hh>
#include <QuantumBody.hh>
#include <Simulation.hh>
#include <memory>
#include <random>
#include <algorithm>
#include <map>
#include <glm/gtc/type_ptr.hpp>


class Renderer {
    public:
        Renderer(Simulation& simulation): simulation(simulation) {};
        ~Renderer() {};

        int setupWindow(int SCR_WIDTH = 800, int SCR_HEIGHT = 800);

        void processInput(GLFWwindow* window);
        
        static void onResize(GLFWwindow* window, int width, int height);
        void handleResize(int width, int height);
        
        static void onMouseClick(GLFWwindow* window, int i, int d, int k);
        void handleMouseClick(int button, int action, int mods);
        
        static void onCursorMove(GLFWwindow* window, double xpos, double ypos);
        void handleCursorMove(double xpos, double ypos);
        
        void setupShaders();
        unsigned int loadShader(const char* Path, GLenum shaderType);
        unsigned int createShaderProgram(const char* vertPath, const char* fragPath);

        void setupObjects();
        void setupCamera();
        void updateCamera(unsigned int projectionLoc, unsigned int transformLoc);
        int startRenderLoop();

    private:
        Simulation& simulation;
        GLFWwindow* window;

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        float cameraDistance = glm::length(cameraPos);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        
        bool mousePressed = false;
        bool notPressed = true;
        float lastX = 400.0f;
        float lastY = 400.0f;
        float pitch = 0.0f;
        float yaw = 90.0f;
        float SCR_WIDTH;
        float SCR_HEIGHT;
        float fov = 45.0;

        glm::mat4 trans = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        std::map<std::string, unsigned int> shaderPrograms;
        std::map<std::string, unsigned int> vertexObjects;
        std::map<std::string, std::pair<unsigned int, unsigned int>> cameraLoc;
};