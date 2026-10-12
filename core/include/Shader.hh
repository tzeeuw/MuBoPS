/*
* This file is part of the MuBoPS project (https://github.com/tzeeuw/MuBoPS)
*/
#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>



class Shader {
    public:
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();

        // avoid copies of the same shader
        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use();

        void setMat4(const char* name, const glm::mat4& m);
        void Shader::set1f(const char* name, float f);

        
    private:
        unsigned int loadShader(std::string path, GLenum shaderType);

        unsigned int shaderProgram = glCreateProgram();

};
