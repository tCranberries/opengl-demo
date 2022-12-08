#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


void CheckProgramLinkStatus(GLuint program);

std::string LoadShaderCode(const char* shaderPath);

GLuint CompileVertexShader(const char* vertexFilePath);

GLuint CompileFragmentShader(const char* fragmentFilePath);

GLuint CompileGeometryShader(const char* geometryFilePath);

GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);

GLuint LoadShaders(const char* vertexFilePath, const char* geometryFilePath, const char* fragmentFilePath);


void SetUniform(GLuint program, const char* name, glm::mat4 matrix);

void SetUniform(GLuint program, const char* name, glm::mat3 matrix);

void SetUniform(GLuint program, const char* name, float value);

void SetUniform(GLuint program, const char* name, int value);

void SetUniform(GLuint program, const char* name, GLuint value);

void SetUniform(GLuint program, const char* name, glm::vec3 value);

void SetUniform(GLuint program, const char* name, GLint count, glm::vec3 value[]);

#endif
