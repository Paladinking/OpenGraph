#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

void validate_shader(unsigned int shader, std::string desc) {
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::" << desc << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void validate_shader_program(unsigned int program, std::string desc) {
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::" << desc << "::LINKING_FAILED\n" << infoLog << std::endl;
	}

}

ShaderProgram::ShaderProgram(std::vector<Shader> shaders) {
	id = glCreateProgram();
	for (Shader& shader : shaders) {
		std::ifstream file;
		std::string shaderSource;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			file.open(shader.path);
			std::stringstream shaderStream;
			shaderStream << file.rdbuf();
			file.close();
			shaderSource = shaderStream.str();
		}
		catch (std::ifstream::failure) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: "<< shader.path << std::endl;
		}
		const char* shaderSrc = shaderSource.c_str();
		unsigned int shader_id = glCreateShader(shader.type);
		glShaderSource(shader_id, 1, &shaderSrc, NULL);
		glCompileShader(shader_id);
		validate_shader(shader_id, shader.path);

		glAttachShader(id, shader_id);
	}
	glLinkProgram(id);
	validate_shader_program(id, "SHADER_PROGRAM");
}

void ShaderProgram::use() const {
	glUseProgram(id);
}	

void ShaderProgram::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float f1, float f2) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), f1, f2);
}

void ShaderProgram::setFloat(const std::string& name, float f1, float f2, float f3) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), f1, f2, f3);
}

void ShaderProgram::setFloat(const std::string& name, float f1, float f2, float f3, float f4) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), f1, f2, f3, f4);
}
