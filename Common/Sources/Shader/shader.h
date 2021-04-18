#pragma once
#include "common.h"
#include "math.h"
#include <vector>
#define BAD_PROGRAM 0
class Shader
{
private:
	GLuint program = BAD_PROGRAM;

public:
	Shader(){}
	Shader(std::string shader_name, GLuint shader_program);
	explicit operator bool() const
	{ 
		return program != BAD_PROGRAM;
  }
	void use() const;
	GLuint get_shader_program() const
	{
		return program;
	}
	void set_mat3x3(const char*name, const mat3x3 &matrix, bool transpose = false) const
	{
		glUniformMatrix3fv(glGetUniformLocation(program, name), 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat4x4(const char*name, const mat4x4 matrix, bool transpose = false) const
	{
		glUniformMatrix4fv(glGetUniformLocation(program, name), 1, transpose, glm::value_ptr(matrix));
	}
	void set_mat3x3(const char*name, const vector<mat3x3> &matrices, bool transpose = false) const
	{
		assert(matrices.size() > 0);
		glUniformMatrix3fv(glGetUniformLocation(program, name), matrices.size() , transpose, glm::value_ptr(matrices[0]));
	}
	void set_mat4x4(const char*name, const vector<mat4x4> &matrices, bool transpose = false) const
	{
		assert(matrices.size() > 0);
		glUniformMatrix4fv(glGetUniformLocation(program, name), matrices.size() , transpose, glm::value_ptr(matrices[0]));
	}
	void set_float(const char*name, const float &v) const
	{
		glUniform1fv(glGetUniformLocation(program, name), 1, &v);
  }
	void set_vec2(const char*name, const vec2 &v) const
	{
		glUniform2fv(glGetUniformLocation(program, name), 1, glm::value_ptr(v));
  }
	void set_vec3(const char*name, const vec3 &v) const
	{
		glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(v));
  }
	void set_vec3(const char*name, const vector<vec3> &v) const
	{
		glUniform3fv(glGetUniformLocation(program, name), v.size(), glm::value_ptr(*v.data()));
  }
	void set_vec4(const char*name, const vec4 &v) const
	{
		glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(v));
  }
};

Shader get_shader(std::string shader_name);