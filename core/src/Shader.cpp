#include <Shader.hh>
#include <filesystem>
#include <fstream>
#include <iostream>



Shader::Shader(const char* vertFile, const char* fragFile){
    std::string filePathSTR = __FILE__;
    std::filesystem::path filePath = filePathSTR;
    std::string shaderPath = filePath.parent_path().parent_path().string() + "/shaders/";

    // load vertex and fragment shaders
    unsigned int vertShader = loadShader(shaderPath + vertFile, GL_VERTEX_SHADER);
    unsigned int fragShader = loadShader(shaderPath + fragFile, GL_FRAGMENT_SHADER);

    // attach and link the shaderprogram
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // check if shader linked correctly
    int succes;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &succes);
    if (!succes) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they are now linked in the shaderprogram
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
};

Shader::~Shader() {

    // delete shader on destruction
    glDeleteProgram(shaderProgram);
}

void Shader::use() {
    glUseProgram(shaderProgram);
}

unsigned int Shader::loadShader(std::string path, GLenum shaderType){

    // open file
    std::ifstream shaderFile(path);
    if (!shaderFile.is_open()) {
        std::cout << "Failed to open shader file: " << path << std::endl;
        return 0;
    }
    
    std::string line;
    std::string fileContents;
    while (std::getline(shaderFile, line)) {
        fileContents += line + "\n";
    }

    // cast file contents to const char
    const char* shaderSource = fileContents.c_str();

    // create the shader
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // check if shader was able to compile
    int succes;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);
    if (!succes) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << path << "\n" << infoLog << std::endl;
    }

    // if shader compiled bueno
    return shader;
};


void Shader::setMat4(const char* name, const glm::mat4& m) {
    unsigned int loc = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
};


void Shader::set1f(const char* name, float f){
    unsigned int loc = glGetUniformLocation(shaderProgram, name);
    glUniform1f(loc, f);
};
