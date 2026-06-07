#pragma once
#include <glad/gl.h>
#include "glm-master/glm/glm.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Renderer
{
	std::string* fragment_shader_text;
	std::string* vertex_shader_text;
	GLuint program_handle;
	GLuint vertex_shader;
	GLuint fragment_shader;
	std::string* ReadShaderFromFile(const char* filename);
	bool BuildFragmentShader(const char* filename);
	bool BuildVertexShader(const char* filename);
	bool CompileProgram();

	public:
		Renderer(const char* vertex_shader_file, const char* fragment_shader_file) : fragment_shader_text(nullptr), 
																					 vertex_shader_text(nullptr), 
																					 program_handle(0), 
																					 vertex_shader(0), 
																					 fragment_shader(0)
		{
			if (BuildVertexShader(vertex_shader_file) && BuildFragmentShader(fragment_shader_file))
				CompileProgram();
			else printf("Error\n");
		};

		~Renderer() {
			delete vertex_shader_text;
			delete fragment_shader_text;

			if (program_handle) glDeleteProgram(program_handle);
			if (vertex_shader) glDeleteShader(vertex_shader);
			if (fragment_shader) glDeleteShader(fragment_shader);
		};
		const GLuint getFragmentShader() { return fragment_shader; };
		const GLuint getVertexShader() { return vertex_shader; };
		const GLuint getProgram() { return program_handle; };

		static const GLuint CreateTexture(const char*);
		static bool SetTexture(GLuint program, GLuint texture, unsigned char slot, const char* uniformName);
		static inline bool SetUniformMatrix4(GLuint program, const char* name, const glm::mat4& m)
		{
			const GLint location = glGetUniformLocation(program, name);
			if (location != -1)
				glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
			return location != -1;
		}

		static inline bool SetUniformMatrix3(GLuint program, const char* name, const glm::mat3& m)
		{
			const GLint location = glGetUniformLocation(program, name);
			if (location != -1)
				glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]);
			return location != -1;
		}

		static inline bool SetUniformFloat(GLuint program, const char* name, float v)
		{
			const GLint location = glGetUniformLocation(program, name);
			if (location != -1)
				glUniform1fv(location, 1, &v);
			return location != -1;
		}

		static inline bool SetUniformVec2(GLuint program, const char* name, const glm::vec2& v)
		{
			const GLint location = glGetUniformLocation(program, name);
			if (location != -1)
				glUniform2fv(location, 1, &v.x);
			return location != -1;
		}
		static inline bool SetUniformVec3(GLuint program, const char* name, const glm::vec3& v)
		{
			const GLint location = glGetUniformLocation(program, name);
			if (location != -1)
				glUniform3fv(location, 1, &v.x);
			return location != -1;
		}
		static inline bool SetUniformVec4(GLuint program, const char* name, const glm::vec4& v)
		{
			const GLint location = glGetUniformLocation(program, name);
			if (location != -1)
				glUniform4fv(location, 1, &v.x);
			return location != -1;
		}
};

