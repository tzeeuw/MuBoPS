/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#pragma once

#include <string>
#include <glad/glad.h>



class Shader {
    public:
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();

        // avoid copies of the same shader
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use();

        unsigned int getShaderID() {return shaderProgram;}

        
    private:
        unsigned int loadShader(std::string path, GLenum shaderType);

        unsigned int shaderProgram = glCreateProgram();

};
