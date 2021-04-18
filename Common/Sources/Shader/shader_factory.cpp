#include "common.h"
#include "shader_factory.h"
#include "shader.h"
#include "Application/application.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <vector>


namespace fs = filesystem;
/*program finds shaders with extensions below and compile them*/
enum SHADER_TYPE
{
  VERTEX_SHADER = GL_VERTEX_SHADER,
  FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
  COMPUTE_SHADER = GL_COMPUTE_SHADER,
  GEOMETRY_SHADER,
  TESSELATION_CONTROL_SHADER,
  TESSELATION_EVALUATION_SHADER
};
map<string, SHADER_TYPE> shaderTypeMap = {{"vert", VERTEX_SHADER}, {"frag", FRAGMENT_SHADER}, {"comp", COMPUTE_SHADER}, 
{"geom", GEOMETRY_SHADER}, {"tesc", TESSELATION_CONTROL_SHADER}, {"tese", TESSELATION_EVALUATION_SHADER}};

map<string, vector<pair<SHADER_TYPE, string>>> shaderMap;
void read_directories(string path)
{
  for (const auto & entry : fs::directory_iterator(path))
  {
    string nextPath = entry.path();
    
    
    if (entry.is_directory())
    {
      read_directories(nextPath);
    } 
    else
    if (entry.is_regular_file())
    {
      int pointPos = nextPath.find_last_of('.');
      string extension = nextPath.substr(pointPos + 1);
      auto shaderTypeIter = shaderTypeMap.find(extension);
      if (shaderTypeIter != shaderTypeMap.end())
      {
        int slash = nextPath.find_last_of('/') + 1;
        string shaderName = nextPath.substr(slash, pointPos - slash);
        string shaderCode;
        ifstream shaderFile;
        shaderFile.exceptions(ifstream::badbit);
        try 
        {
          shaderFile.open(nextPath);
          stringstream shaderStream;
          shaderStream << shaderFile.rdbuf();  
          shaderFile.close();
          shaderCode = shaderStream.str();

          shaderMap[shaderName].push_back({shaderTypeIter->second, shaderCode});
        }
        catch(ifstream::failure &e)
        {
          debug_error("Shader in %s not successfuly read!", nextPath.c_str());
        }
      }
    }
    
  }
}

void compile_shaders(const map<string, vector<pair<SHADER_TYPE, string>>> &shaderMap, bool createShaders)
{

  for (auto &shader:shaderMap)
  {
    string shaderName = shader.first;
    vector<GLuint> shaders;
    GLchar infoLog[512];
    GLint success;
    for (auto & codes : shader.second)
    {
      GLuint shaderProg = glCreateShader(codes.first);
      const GLchar * shaderCode = codes.second.c_str();
      glShaderSource(shaderProg, 1, &shaderCode, NULL);
      glCompileShader(shaderProg);
      glGetShaderiv(shaderProg, GL_COMPILE_STATUS, &success);
      if(!success)
      {
          glGetShaderInfoLog(shaderProg, 512, NULL, infoLog);
          string extension;
          for (auto p: shaderTypeMap)
            if (p.second == codes.first)
              extension = p.first;

        debug_error("Shader (%s.%s) compilation failed!\n Log: %s", shaderName.c_str(), extension.c_str(), infoLog);
      };
      shaders.push_back(shaderProg);
    }


    GLuint program = glCreateProgram();
    for (GLuint shaderProg: shaders)
      glAttachShader(program, shaderProg);
    
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        debug_error("Shader programm (%s) linking failed!\n Log: %s", shaderName.c_str(), infoLog);
    }

    for (GLuint shaderProg: shaders)
      glDeleteShader(shaderProg);
    debug_log("Shader %s was compiled.", shaderName.c_str());
    if (createShaders)
    {
      Shader(shaderName, program);
    }
  }
}
void precompile_shaders()
{
  shaderMap.clear();
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  compile_shaders(shaderMap, false);
}
void compile_shaders()
{
  shaderMap.clear();
  read_directories(Application::instance().commonShaderPath);
  read_directories(Application::instance().projectShaderPath);
  compile_shaders(shaderMap, true);
}