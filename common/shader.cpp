

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>
#include "shader.hpp"


GLuint CompileVertexShader(const char* vertexFilePath) {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexFilePath);
        getchar();
        return 0;
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertexFilePath);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    return VertexShaderID;
}

GLuint CompileFragmentShader(const char* fragmentFilePath) {
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragmentFilePath);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , nullptr);
    glCompileShader(FragmentShaderID);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0 ) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    return FragmentShaderID;
}

GLuint CompileGeometryShader(const char* geometryFilePath) {
    GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    // Read the Fragment Shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(geometryFilePath, std::ios::in);
    if (shaderStream.is_open()) {
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", geometryFilePath);
    char const * sourcePointer = shaderCode.c_str();
    glShaderSource(GeometryShaderID, 1, &sourcePointer , nullptr);
    glCompileShader(GeometryShaderID);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Check Fragment Shader
    glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0 ) {
        std::vector<char> ErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(GeometryShaderID, InfoLogLength, nullptr, &ErrorMessage[0]);
        printf("%s\n", &ErrorMessage[0]);
    }

    return GeometryShaderID;
}


GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint VertexShaderID = CompileVertexShader(vertexFilePath);
    GLuint FragmentShaderID = CompileFragmentShader(fragmentFilePath);

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


GLuint LoadShaders(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath) {

    GLint Result = GL_FALSE;
    int InfoLogLength;

    GLuint VertexShaderID;
    GLuint GeometryShaderID;
    GLuint FragmentShaderID;

    GLuint ProgramID = glCreateProgram();
    if (vertexFilePath) {
        VertexShaderID = CompileVertexShader(vertexFilePath);
        glAttachShader(ProgramID, VertexShaderID);
    }

    if (geometryFilePath) {
        GeometryShaderID = CompileGeometryShader(geometryFilePath);
        glAttachShader(ProgramID, GeometryShaderID);
    }
    if (fragmentFilePath) {
        FragmentShaderID = CompileFragmentShader(fragmentFilePath);
        glAttachShader(ProgramID, FragmentShaderID);
    }

    // Link the program
    printf("Linking program\n");
    glLinkProgram(ProgramID);
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }


    if (vertexFilePath) {
        glDetachShader(ProgramID, VertexShaderID);
        glDeleteShader(VertexShaderID);
    }

    if (geometryFilePath) {
        glDetachShader(ProgramID, GeometryShaderID);
        glDeleteShader(GeometryShaderID);
    }
    if (fragmentFilePath) {
        glDetachShader(ProgramID, FragmentShaderID);
        glDeleteShader(FragmentShaderID);
    }

    return ProgramID;
}


void SetUniform(GLuint program, const char* name, glm::mat4 matrix) {
    GLuint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv((GLint)location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void SetUniform(GLuint program, const char* name, glm::mat3 matrix) {
    GLuint location = glGetUniformLocation(program, name);
    glUniformMatrix3fv((GLint)location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void SetUniform(GLuint program, const char* name, glm::vec3 value) {
    GLuint location = glGetUniformLocation(program, name);
    glUniform3fv((GLint)location, 1, glm::value_ptr(value));
}

void SetUniform(GLuint program, const char* name, float value) {
    GLuint location = glGetUniformLocation(program, name);
    glUniform1f((GLint)location, value);
}

void SetUniform(GLuint program, const char* name, int value) {
    GLuint location = glGetUniformLocation(program, name);
    glUniform1i((GLint)location, value);
}

void SetUniform(GLuint program, const char* name, GLint count, glm::vec3 value[]) {
    GLuint location = glGetUniformLocation(program, name);
    glUniform3fv((GLint)location, count, &value[0][0]);
}












