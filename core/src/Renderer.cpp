/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#include <Renderer.hh>
#include <Shader.hh>
#include <iostream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void Renderer::processInput(GLFWwindow* window) {

    // close the window if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float velocity = cameraSpeed * deltaTime;
    float rollSpeed = cameraSpeed * 0.5f;

    // WASD input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward(velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveForward(-velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveRight(-velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight(velocity);
    }

    // up and down
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.moveUp(velocity);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.moveUp(-velocity);
    }

    // rol on q and e
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.rotate(0, 0, rollSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.rotate(0, 0, -rollSpeed);
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
}


void Renderer::onScroll(GLFWwindow* window, double xoffset, double yoffset) {
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->handleScroll(yoffset);
}

void Renderer::handleScroll(double yoffset) {
    camera.zoom(yoffset);
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

        // rotate the camera
        camera.rotate(xoffset, yoffset, 0);
    }
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

    // only use forward compatible context (enables MacOS support)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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
    
    skyboxShader.emplace("skybox.vert", "skybox.frag");
    quantumShader.emplace("point.vert", "quantum.frag");
    classicalShader.emplace("point.vert", "classical.frag");
    octreeShader.emplace("octree.vert", "octree.frag");
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


    // Octree VAO, VBO's
    unsigned int octreeVBO, octreeVAO;
    glGenVertexArrays(1, &octreeVAO);
    glGenBuffers(1, &octreeVBO);
    glBindVertexArray(octreeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, octreeVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vertexObjects["octreeVBO"] = octreeVBO;
    vertexObjects["octreeVAO"] = octreeVAO;

    // Skybox VAO, VBO's
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    vertexObjects["skyboxVBO"] = skyboxVBO;
    vertexObjects["skyboxVAO"] = skyboxVAO;
    vertexObjects["skyboxEBO"] = skyboxEBO;
    
}



void Renderer::applyCamera(Shader& shaderProgram){

    // first use the program
    shaderProgram.use();

    // update the camera projection and view matrices in the shader
    // NOTE: because camera is ALWAYS at origin, 4th column for the skybox does not have to be 0, as it is always 0
    // If camera behaviour changes to where camera can also move away form origin then translational motion of skybox
    // becomes issue
    // reason why camera is always at origin is due to double precision before casting it to floats for glm processing.
    shaderProgram.setMat4("projection", camera.getProjection(float(SCR_WIDTH) / float(SCR_HEIGHT)));
    shaderProgram.setMat4("model", glm::mat4(1.0f));        // currenlty all models have the same model matrix, could change in later versions
    shaderProgram.setMat4("view", camera.getView());
    shaderProgram.set1f("fov", camera.getFOV());
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


void Renderer::createSkyBox(){

    // skybox images
    std::string filePathSTR = __FILE__;
    std::filesystem::path filePath = filePathSTR;
    std::string skyboxPath = filePath.parent_path().parent_path().string() + "/images/skybox/";
    std::string Cubemap[6] = 
    {
        skyboxPath + "right.jpg",
        skyboxPath + "left.jpg",
        skyboxPath + "top.jpg",
        skyboxPath + "bottom.jpg",
        skyboxPath + "front.jpg",
        skyboxPath + "back.jpg"
    };

    // create the cubemap texture object
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // cycle through all the textures and attach them to the texture object
    for (unsigned int i = 0; i < 6; i ++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(Cubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                width,
                height,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture:" << Cubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }

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

    // create skybox (FIXME make propper setup pipeline)
    createSkyBox();

    simulation.initGravityPairs();
    
    while (!glfwWindowShouldClose(window)) {
        step += 1;
        
        // calculate speed of frame rendering for consistent camera movement
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        // // background color
        // glClearColor(7.0/255.0, 15.0/255.0, 28.0/255.0, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // update the simulation for a few steps before rendering the next frame to ensure smooth animation
        float now = glfwGetTime();
        for (int i = 0; i < 10; i++){
            simulation.update(0.001);
        }
        // if (step % 1 == 0){
        //     std::cout << glfwGetTime() - now  << "s" << std::endl;
        // }
        

        // render the skybox
        // use GL EQUAL so that skybox is rendered at depth 1.0
        glDepthFunc(GL_LEQUAL);


        glBindVertexArray(vertexObjects["skyboxVAO"]);
        applyCamera(*skyboxShader);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // use GL LESS for normal behaviour again
        glDepthFunc(GL_LESS);

        // first render the classical data
        std::vector<glm::vec4> classicalRenderData = simulation.getClassicalRenderData(camera.getPosition());
        int pointsClassical = static_cast<int>(classicalRenderData.size());
        
        glBindVertexArray(vertexObjects["VAO"]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexObjects["VBO"]);

        applyCamera(*classicalShader);
        
        glBufferData(GL_ARRAY_BUFFER, classicalRenderData.size() * sizeof(glm::vec4), classicalRenderData.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pointsClassical);
        
        
        // now render the quantum data
        std::vector<glm::vec4> quantumRenderData = simulation.getQuantumRenderData(camera.getPosition());
        int pointsQuantum = static_cast<int>(quantumRenderData.size());
        
        // make sure that alpha actually works by disabling depth
        glDepthMask(GL_FALSE);
        applyCamera(*quantumShader);
        
        glBufferData(GL_ARRAY_BUFFER, quantumRenderData.size() * sizeof(glm::vec4), quantumRenderData.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pointsQuantum);
        
        // enable it after rendering the quantum points
        glDepthMask(GL_TRUE);

        // if octree rendering is enabled render te boxes
        if (showOctree){
            std::vector<glm::vec3> renderData;
            std::vector<std::pair<glm::dvec3, double>> data = simulation.getOctreeRenderData(camera.getPosition());
            for (auto& datapoint: data){
                std::vector<glm::vec3> boxData = generateBox(datapoint);
                renderData.insert(renderData.end(), boxData.begin(), boxData.end());
            }

            glBindVertexArray(vertexObjects["octreeVAO"]);
            glBindBuffer(GL_ARRAY_BUFFER, vertexObjects["octreeVBO"]);
            applyCamera(*octreeShader);

            glBufferData(GL_ARRAY_BUFFER, renderData.size() * sizeof(glm::vec3), renderData.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_LINES, 0, static_cast<int>(renderData.size()));

        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    // delete shaders and buffers, terminate GLFW
    glDeleteBuffers(1, &vertexObjects["octreeVBO"]);
    glDeleteVertexArrays(1, &vertexObjects["octreeVAO"]);
    glDeleteBuffers(1, &vertexObjects["VBO"]);
    glDeleteVertexArrays(1, &vertexObjects["VAO"]);
    glDeleteBuffers(1, &vertexObjects["skyboxVBO"]);
    glDeleteBuffers(1, &vertexObjects["skyboxEBO"]);
    glDeleteVertexArrays(1, &vertexObjects["skyboxVAO"]);
    glDeleteTextures(1, &cubemapTexture);
    
    return 0;
}


Renderer::~Renderer() {

    // requires custom destructor so that shaders are deleted before glfw is terminated
    classicalShader.reset();
    quantumShader.reset();
    octreeShader.reset();
    skyboxShader.reset();

    glfwTerminate();
}
