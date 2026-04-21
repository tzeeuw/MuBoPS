#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <ClassicalBody.hh>
#include <Simulation.hh>
#include <memory>
#include <random>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int loadShader(const char* Path, GLenum shaderType);
void onMouseClick(GLFWwindow* window, int i, int d, int k);
void onCursorMove(GLFWwindow* window, double xpos, double ypos);


bool mousePressed = false;
bool notPressed = true;
float lastX = 400.0f;
float lastY = 400.0f;
float pitch = 0.0f;
float yaw = 90.0f;
float SCR_WIDTH = 800.0;
float SCR_HEIGHT = 800.0;
float fov = 45.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
float cameraDistance = glm::length(cameraPos);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


int main() {

    glfwInit();

    // use openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // use core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create the winow object
    GLFWwindow* window = glfwCreateWindow(int(SCR_WIDTH), int(SCR_HEIGHT), "MuBoPS OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, int(SCR_WIDTH), int(SCR_HEIGHT));

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, onMouseClick);
    glfwSetCursorPosCallback(window, onCursorMove);


    // load the shaders
    unsigned int vertexShader = loadShader("core/shaders/point.vert", GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadShader("core/shaders/point.frag", GL_FRAGMENT_SHADER);
    unsigned int trailShader = loadShader("core/shaders/trail.frag", GL_FRAGMENT_SHADER);

    // link shaders to shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    unsigned int trailShaderProgram = glCreateProgram();
    glAttachShader(trailShaderProgram, vertexShader);
    glAttachShader(trailShaderProgram, trailShader);
    glLinkProgram(trailShaderProgram);

    // check compilation
    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // create the drawing objects
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glPointSize(10.0f);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    // for now generate random body positions
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> angleDist(0, 2*3.14159);
    std::uniform_real_distribution<double> radDist(0, 1);

    int step = 0;
    std::vector<std::unique_ptr<Body>> bodies;
    double minRadius = 0.1;
    double maxRadius = 0.8;
    for (int i = 0; i < 5000; i++){
        auto body = std::make_unique<ClassicalBody>(15);
        double angle = angleDist(mt);
        double radius = std::sqrt(minRadius*minRadius + radDist(mt)*(maxRadius*maxRadius - minRadius*minRadius));
        glm::dvec3 position = glm::dvec3(std::cos(angle) * radius, std::sin(angle) * radius, 0.0);
        body->setPosition(position);

        // calculate the velocity size
        double velSize = std::sqrt(6.67430e-11 * 5.972e8/radius)/radius;
        glm::dvec3 velocity = glm::dvec3(-position[1], position[0], position[2]) * velSize;
        body->setVelocity(velocity);
        body->setAcceleration({0.0, 0.0, 0.0});
        bodies.push_back(std::move(body));
    }


    Simulation sim;
    sim.addBodies(std::move(bodies));

    // set the color range for the particles

    std::vector<float> speeds = sim.getSpeeds();
    auto speedValue = std::minmax_element(speeds.begin(), speeds.end());
    float minSpeedValue = *speedValue.first;
    float maxSpeedValue = *speedValue.second;



    glUseProgram(shaderProgram);
    unsigned int minLoc = glGetUniformLocation(shaderProgram, "minSpeed");
    unsigned int maxLoc = glGetUniformLocation(shaderProgram, "maxSpeed");
    glUniform1f(minLoc, minSpeedValue);
    glUniform1f(maxLoc, maxSpeedValue);
    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        // draw a moving point that moves in a circle
        step += 1;
        for (int i = 0; i < 10; i++){
            sim.update(0.001);
        }
        std::vector<glm::vec4> positionsAndSpeeds = sim.getPositionsAndSpeed();
        int points = static_cast<int>(positionsAndSpeeds.size());

        glUseProgram(shaderProgram);

        glBufferData(GL_ARRAY_BUFFER, positionsAndSpeeds.size() * sizeof(glm::vec4), positionsAndSpeeds.data(), GL_DYNAMIC_DRAW);

        projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        trans = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        glDrawArrays(GL_POINTS, 0, points);

        // if (step % 50 == 0) {
        //     sim.addTrailPoints();
        // }
        
        // std::vector<glm::vec3> trail = sim.getTrails();
        // int trailPoints = static_cast<int>(trail.size());

        // glUseProgram(trailShaderProgram);
        // glBufferData(GL_ARRAY_BUFFER, trail.size() * sizeof(glm::vec3), trail.data(), GL_DYNAMIC_DRAW);
        // glDrawArrays(GL_POINTS, 0, trailPoints);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void onMouseClick(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT){
        if (action == GLFW_PRESS) {
            mousePressed = true;
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
            notPressed = true;
        }
    }
    
}

void onCursorMove(GLFWwindow* window, double xpos, double ypos){
    if (mousePressed) {
        if (notPressed) {
            lastX = xpos;
            lastY = ypos;
            notPressed = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        const float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch -= yoffset;

        glm::vec3 direction;
        direction.x = cameraDistance*cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = cameraDistance*sin(glm::radians(pitch));
        direction.z = cameraDistance*sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraPos = direction;
    }
}


unsigned int loadShader(const char* Path, GLenum shaderType) {

    // open file
    std::ifstream shaderFile(Path);
    if (!shaderFile.is_open()) {
        std::cout << "Failed to open shader file: " << Path << std::endl;
        return 0;
    }

    // read shader file into string
    std::string line;
    std::string fileContents;
    while (std::getline(shaderFile, line)) {
        fileContents += line + "\n";
    }

    // cast string to const char*
    const char* shaderSource = fileContents.c_str();

    // create the shader
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // check compilation
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << Path << "\n" << infoLog << std::endl;
    }

    // return the shader ID
    return shader;
}
