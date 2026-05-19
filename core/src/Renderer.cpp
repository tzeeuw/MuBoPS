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

    float velocity = cameraSpeed * deltaTime;
    float rollSpeed = cameraSpeed * 0.5f;

    // WASD input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += velocity * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= velocity * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // for the right/left directions take the cross product with camera up and the front
        cameraPos -= velocity * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += velocity * glm::normalize(glm::cross(cameraFront, cameraUp));
    }

    // up and down
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += velocity * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        cameraPos -= velocity * cameraUp;
    }

    // rol on q and e
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glm::mat4 rollRot = glm::rotate(glm::mat4(1.0f), glm::radians(-rollSpeed), cameraFront);
        cameraUp = glm::normalize(glm::vec3(rollRot * glm::vec4(cameraUp, 0.0f)));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glm::mat4 rollRot = glm::rotate(glm::mat4(1.0f), glm::radians(rollSpeed), cameraFront);
        cameraUp = glm::normalize(glm::vec3(rollRot * glm::vec4(cameraUp, 0.0f)));
    }

    // shift for speeding of camera
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed = 3.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        cameraSpeed = 1.5f;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        octreePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
        if (octreePressed){
            showOctree = !showOctree;
            octreePressed = false;
        }
    }

    // update camera local coordinates after every input to enforce them being orthogonal
    cameraFront = glm::normalize(cameraFront);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));
}


void Renderer::onScroll(GLFWwindow* window, double xoffset, double yoffset) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->handleScroll(xoffset, yoffset);
}

void Renderer::handleScroll(double xoffset, double yoffset) {
    FOV -= float(yoffset);
    if (FOV < 1.0f) {
        FOV = 1.0f;
    }
    if (FOV > 60.0f) {
        FOV = 60.0f;
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
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstPress = true;
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

        // prevent jitter during the first press of the mouse button
        if (firstPress) {
            lastX = xpos;
            lastY = ypos;
            firstPress = false;
        }

        // calculate the offset of the mouse movement
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;       // range is from bottom to top so reverse

        lastX = xpos;
        lastY = ypos;

        // apply sensitivity to the mouse movement
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        // rotate around the camera's local axes
        glm::mat4 yawRot = glm::rotate(glm::mat4(1.0f), glm::radians(-xoffset), cameraUp);
        glm::mat4 pitchRot = glm::rotate(glm::mat4(1.0f), glm::radians(yoffset), cameraRight);

        cameraFront = glm::vec3(yawRot * pitchRot * glm::vec4(cameraFront, 0.0f));

        // renormalize the basis after rotation
        cameraFront = glm::normalize(cameraFront);
        cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
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

    // set all callbacks
    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetMouseButtonCallback(window, onMouseClick);
    glfwSetCursorPosCallback(window, onCursorMove);
    glfwSetScrollCallback(window, onScroll);

    return 0;
}


void Renderer::setupShaders() {
    
    // add shaders to shader program map
    shaderPrograms["classical"] = createShaderProgram("core/shaders/point.vert", "core/shaders/classical.frag");
    shaderPrograms["quantum"] = createShaderProgram("core/shaders/point.vert", "core/shaders/quantum.frag");
    shaderPrograms["octree"] = createShaderProgram("core/shaders/octree.vert", "core/shaders/octree.frag");
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
    
    // add objects to the map
    vertexObjects["VBO"] = VBO;
    vertexObjects["VAO"] = VAO;

    unsigned int octreeVBO, octreeVAO;
    glGenVertexArrays(1, &octreeVAO);
    glGenBuffers(1, &octreeVBO);
    glBindVertexArray(octreeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, octreeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vertexObjects["octreeVBO"] = octreeVBO;
    vertexObjects["octreeVAO"] = octreeVAO;
}


void Renderer::setupCamera(){

    // setup camera speeds and position
    cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    view = glm::lookAt(glm::vec3(0.0f), cameraFront, cameraUp);
    model = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);

    cameraSpeed = 1.5f;
    sensitivity = 0.3f;
    FOV = 45.0;


    // setup camera projection and view matrices
    cameraLoc["classical"].projection = glGetUniformLocation(shaderPrograms["classical"], "projection");
    cameraLoc["classical"].model = glGetUniformLocation(shaderPrograms["classical"], "model");
    cameraLoc["classical"].view = glGetUniformLocation(shaderPrograms["classical"], "view");
    cameraLoc["classical"].fov = glGetUniformLocation(shaderPrograms["classical"], "fov");
    cameraLoc["quantum"].projection = glGetUniformLocation(shaderPrograms["quantum"], "projection");
    cameraLoc["quantum"].model = glGetUniformLocation(shaderPrograms["quantum"], "model");
    cameraLoc["quantum"].view = glGetUniformLocation(shaderPrograms["quantum"], "view");
    cameraLoc["quantum"].fov = glGetUniformLocation(shaderPrograms["quantum"], "fov");
    cameraLoc["octree"].projection = glGetUniformLocation(shaderPrograms["octree"], "projection");
    cameraLoc["octree"].model = glGetUniformLocation(shaderPrograms["octree"], "model");
    cameraLoc["octree"].view = glGetUniformLocation(shaderPrograms["octree"], "view");
}

void Renderer::updateCamera(ShaderUniforms uniforms){

    // update the camera projection and view matrices in the shader
    glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniforms.model, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniforms.view, 1, GL_FALSE, glm::value_ptr(view));
    glUniform1f(uniforms.fov, FOV);
}

std::vector<glm::vec3> Renderer::generateBox(std::pair<glm::dvec3, double> boxData){
    std::vector<glm::vec3> renderData;
    glm::dvec3 offset({0.0,0.0,0.0});
    int indexpairs[3][2] = {
        {1,2},
        {0,2},
        {0,1},
    };

    for (int i=0; i < 3; i++){
        for (int jk = 0; jk < 4; jk++){
            int j = (jk & 1) ? 1 : -1;
            int k = (jk & 2) ? 1 : -1;

            offset[indexpairs[i][0]] = j;
            offset[indexpairs[i][1]] = k;
            offset[i] = 1;
            renderData.push_back(boxData.first + boxData.second*offset);
            offset[i] = -1;
            renderData.push_back(boxData.first + boxData.second*offset);
        }
    }
    return renderData;
}


int Renderer::startRenderLoop() {
    
    
    // enable point size based on distance
    glEnable(GL_PROGRAM_POINT_SIZE);
    
    // enable depth of points
    glEnable(GL_DEPTH_TEST);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int step = 0;

    simulation.initGravityPairs();
    
    while (!glfwWindowShouldClose(window)) {
        step += 1;
        
        // calculate speed of frame rendering for consistent camera movement
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        // background color
        glClearColor(7.0/255.0, 15.0/255.0, 28.0/255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // update the simulation for a few steps before rendering the next frame to ensure smooth animation
        float now = glfwGetTime();
        for (int i = 0; i < 10; i++){
            simulation.update(0.001);
        }
        if (step % 10 == 0){
            std::cout << glfwGetTime() - now  << "s" << std::endl;
        }
        
        // update the camera projection and view matrices based on the current camera position and orientation
        projection = glm::perspective(glm::radians(FOV), float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1f, 100.0f);
        view = glm::lookAt(glm::vec3(0.0f), cameraFront, cameraUp);
        
        // first render the classical data
        std::vector<glm::vec4> classicalRenderData = simulation.getClassicalRenderData(cameraPos);
        int pointsClassical = static_cast<int>(classicalRenderData.size());
        
        glBindVertexArray(vertexObjects["VAO"]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexObjects["VBO"]);

        glUseProgram(shaderPrograms["classical"]);
        updateCamera(cameraLoc["classical"]);
        
        glBufferData(GL_ARRAY_BUFFER, classicalRenderData.size() * sizeof(glm::vec4), classicalRenderData.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pointsClassical);
        
        
        // now render the quantum data
        std::vector<glm::vec4> quantumRenderData = simulation.getQuantumRenderData(cameraPos);
        int pointsQuantum = static_cast<int>(quantumRenderData.size());
        
        // make sure that alpha actually works by disabling depth
        glDepthMask(GL_FALSE);
        glUseProgram(shaderPrograms["quantum"]);
        updateCamera(cameraLoc["quantum"]);
        
        glBufferData(GL_ARRAY_BUFFER, quantumRenderData.size() * sizeof(glm::vec4), quantumRenderData.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pointsQuantum);
        
        // enable it after rendering the quantum points
        glDepthMask(GL_TRUE);

        // if octree rendering is enabled render te boxes
        if (showOctree){
            std::vector<glm::vec3> renderData;
            std::vector<std::pair<glm::dvec3, double>> data = simulation.getOctreeRenderData(cameraPos);
            for (auto& datapoint: data){
                std::vector<glm::vec3> boxData = generateBox(datapoint);
                renderData.insert(renderData.end(), boxData.begin(), boxData.end());
            }

            glBindVertexArray(vertexObjects["octreeVAO"]);
            glBindBuffer(GL_ARRAY_BUFFER, vertexObjects["octreeVBO"]);
            glUseProgram(shaderPrograms["octree"]);
            updateCamera(cameraLoc["octree"]);
            glBufferData(GL_ARRAY_BUFFER, renderData.size() * sizeof(glm::vec3), renderData.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_LINES, 0, static_cast<int>(renderData.size()));

        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // delete shaders and buffers, terminate GLFW
    glDeleteProgram(shaderPrograms["classical"]);
    glDeleteProgram(shaderPrograms["quantum"]);
    glDeleteProgram(shaderPrograms["octree"]);
    glDeleteBuffers(1, &vertexObjects["octreeVBO"]);
    glDeleteVertexArrays(1, &vertexObjects["octreeVAO"]);
    glDeleteBuffers(1, &vertexObjects["VBO"]);
    glDeleteVertexArrays(1, &vertexObjects["VAO"]);
    glfwTerminate();
    
    return 0;
}
