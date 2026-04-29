/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <Renderer.hh>
#include <iostream>
#include <fstream>


void Renderer::processInput(GLFWwindow* window) {

    // close the window if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}


void Renderer::onResize(GLFWwindow* window, int width, int height) {

    // get the Renderer instance from the window user pointer and call the non-static resize handler
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->handleResize(width, height);
}

void Renderer::handleResize(int width, int height) {
    this->SCR_WIDTH = width;
    this->SCR_HEIGHT = height;
    glViewport(0, 0, width, height);
}


void Renderer::onMouseClick(GLFWwindow* window, int button, int action, int mods) {

    // get the Renderer instance from the window user pointer and call the non-static mouse click handler
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->handleMouseClick(button, action, mods);
}

void Renderer::handleMouseClick(int button, int action, int mods) {
    // if the left mouse button is pressed, set the mousePressed flag to true so that camera is updated
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


void Renderer::onCursorMove(GLFWwindow* window, double xpos, double ypos) {

    // get the Renderer instance from the window user pointer and call the non-static cursor move handler
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->handleCursorMove(xpos, ypos);
}

void Renderer::handleCursorMove(double xpos, double ypos){

    // update camera position based on mouse movement if the left mouse button is pressed
    if (mousePressed) {
        if (notPressed) {
            lastX = xpos;
            lastY = ypos;
            notPressed = false;
        }

        // calculate the offset of the mouse movement
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        // apply sensitivity to the mouse movement
        const float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch -= yoffset;

        // calculate the new camera position based on the updated yaw and pitch angles
        glm::vec3 direction;
        direction.x = cameraDistance*cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = cameraDistance*sin(glm::radians(pitch));
        direction.z = cameraDistance*sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        cameraPos = direction;
    }
}


unsigned int Renderer::loadShader(const char* Path, GLenum shaderType) {
    
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
unsigned int Renderer::createShaderProgram(const char* vertPath, const char* fragPath) {
    
    // load vertex and fragment shaders
    unsigned int vertexShader = loadShader(vertPath, GL_VERTEX_SHADER);
    unsigned int fragmentShader = loadShader(fragPath, GL_FRAGMENT_SHADER);
    
    // create shaderprogam and link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check if shader program complied correctly
    int success;
    char infoLog[512];
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they are now linked into the program and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}


int Renderer::setupWindow(int SCR_WIDTH, int SCR_HEIGHT) {

    // store the screen width and height for later use in the projection matrix and viewport
    this->SCR_WIDTH = SCR_WIDTH;
    this->SCR_HEIGHT = SCR_HEIGHT;

    glfwInit();

    // use openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // use core profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create the winow object
    window = glfwCreateWindow(int(SCR_WIDTH), int(SCR_HEIGHT), "MuBoPS OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // required as GLFW is C library so it has no concept of this when it comes to OOP
    glfwSetWindowUserPointer(window, this);

    glfwMakeContextCurrent(window);

    // initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, int(SCR_WIDTH), int(SCR_HEIGHT));

    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetMouseButtonCallback(window, onMouseClick);
    glfwSetCursorPosCallback(window, onCursorMove);

    return 0;
}


void Renderer::setupShaders() {
    
    // add shaders to shader program map
    shaderPrograms["classical"] = createShaderProgram("core/shaders/point.vert", "core/shaders/classical.frag");
    shaderPrograms["quantum"] = createShaderProgram("core/shaders/point.vert", "core/shaders/quantum.frag");
    shaderPrograms["trail"] = createShaderProgram("core/shaders/point.vert", "core/shaders/trail.frag");
}


void Renderer::setupObjects(){

    // create vertex buffer and vertex array objects for rendering points
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
    
    // add objects to the map
    vertexObjects["VBO"] = VBO;
    vertexObjects["VAO"] = VAO;
}


void Renderer::setupCamera(){

    // setup camera projection and view matrices
    cameraLoc["classical"].first = glGetUniformLocation(shaderPrograms["classical"], "transform");
    cameraLoc["classical"].second = glGetUniformLocation(shaderPrograms["classical"], "projection");
    cameraLoc["quantum"].first = glGetUniformLocation(shaderPrograms["quantum"], "transform");
    cameraLoc["quantum"].second = glGetUniformLocation(shaderPrograms["quantum"], "projection");
}

void Renderer::updateCamera(unsigned int transformLoc, unsigned int projectionLoc){

    // update the camera projection and view matrices in the shader
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}



int Renderer::startRenderLoop() {
    
    
    while (!glfwWindowShouldClose(window)) {
        
        processInput(window);
        // background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        // update the simulation for a few steps before rendering the next frame to ensure smooth animation
        for (int i = 0; i < 10; i++){
            simulation.update(0.001);
        }
        
        // update the camera projection and view matrices based on the current camera position and orientation
        projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        trans = glm::lookAt(cameraPos, glm::vec3(0.0f), cameraUp);
        
        // first render the classical data
        std::vector<glm::vec4> classicalRenderData = simulation.getClassicalRenderData();
        int pointsClassical = static_cast<int>(classicalRenderData.size());
        
        glUseProgram(shaderPrograms["classical"]);
        updateCamera(cameraLoc["classical"].first, cameraLoc["classical"].second);
        
        glBufferData(GL_ARRAY_BUFFER, classicalRenderData.size() * sizeof(glm::vec4), classicalRenderData.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pointsClassical);
        
        
        // now render the quantum data
        std::vector<glm::vec4> quantumRenderData = simulation.getQuantumRenderData();
        int pointsQuantum = static_cast<int>(quantumRenderData.size());
        
        glUseProgram(shaderPrograms["quantum"]);
        updateCamera(cameraLoc["quantum"].first, cameraLoc["quantum"].second);
        
        glBufferData(GL_ARRAY_BUFFER, quantumRenderData.size() * sizeof(glm::vec4), quantumRenderData.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pointsQuantum);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // delete shaders and buffers, terminate GLFW
    glDeleteProgram(shaderPrograms["classical"]);
    glDeleteProgram(shaderPrograms["quantum"]);
    glDeleteBuffers(1, &vertexObjects["VBO"]);
    glDeleteVertexArrays(1, &vertexObjects["VAO"]);
    glfwTerminate();
    
    return 0;
}
