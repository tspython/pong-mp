#ifndef SHADER_H
#define SHADER_H

#include "util.h"

namespace Shader {
    unsigned int compileShader(const char* shaderSource, GLenum shaderType) {
        unsigned int shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "Shader compilation failed: " << infoLog << std::endl;
        }

        return shader;
    }

    unsigned int linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "Shader linking failed: " << infoLog << std::endl;
        }

        return program;
    }

    void load(const std::string& vertexPath, const std::string& fragmentPath, unsigned int& shaderProgram) {
        std::string vertexShaderStr = Util::readFile(vertexPath);
        std::string fragmentShaderStr = Util::readFile(fragmentPath);

        const char* vertexShaderSource = vertexShaderStr.c_str();
        const char* fragmentShaderSource = fragmentShaderStr.c_str();

        unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        shaderProgram = linkProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}

#endif // SHADER_H
