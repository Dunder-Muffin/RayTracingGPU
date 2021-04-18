#include "shader.h"
#include <map>
#include <iostream>

static std::map<std::string, Shader> shaderMap;
static Shader badShader("badShader", BAD_PROGRAM);
Shader::Shader(std::string shader_name, unsigned int shader_program):
    program(shader_program)
{
  shaderMap[shader_name] = *this;
}
Shader get_shader(std::string shader_name)
{
  auto shader_iter = shaderMap.find(shader_name);
  if (shader_iter != shaderMap.end())
  {
    return shader_iter->second;
  }
  else
  {
    std::cerr << "Can't find shader " << shader_name << std::endl;
    return badShader;
  }
}
void Shader::use() const
{
  glUseProgram(program);
}