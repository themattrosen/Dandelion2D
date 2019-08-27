#include <fstream>
#include <iostream>
#include <sstream>
#include <glad\glad.h>
#include "Shader.h"

Shader::Shader() : m_name(""), m_programId(0)
{
}

Shader::Shader(string _name)
  : m_name{ _name }, m_programId{ 0 }
{
	std::string name = (const char*)_name;
  // extract source code of shaders into const char ptrs
  std::ifstream vsfile, fsfile;
  vsfile.open("./assets/Shaders/" + name + ".vs");
  fsfile.open("./assets/Shaders/" + name + ".fs");

  ASSERT(vsfile.good() && fsfile.good())

  std::stringstream vstream, fstream;
  vstream << vsfile.rdbuf();
  fstream << fsfile.rdbuf();

  std::string vsTemp = vstream.str();
  std::string fsTemp = fstream.str();
  const GLchar * vsSource = vsTemp.c_str();
  const GLchar * fsSource = fsTemp.c_str();

  // create vertex shader id
  unsigned vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  if (!vertexShaderId)
  {
    std::cout << "Failed to create vertex shader!" << std::endl;
    ASSERT(false);
  }

  // get the vertex shader source code
  glShaderSource(vertexShaderId, 1, &vsSource, NULL);

  // compile the vertext shader code
  glCompileShader(vertexShaderId);

  // check compilation result
  int success;
  glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512] = { 0 };
    glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED - " << std::endl << infoLog << std::endl << std::endl;
	ASSERT(false);
  }

  // create fragment shader
  unsigned fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  // get the fragment shader source code
  glShaderSource(fragmentShaderId, 1, &fsSource, NULL);

  // compile the fragment shader
  glCompileShader(fragmentShaderId);

  // check compilation result
  glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[512] = { 0 };
    glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED - " << std::endl << infoLog << std::endl << std::endl;
	ASSERT(false);
  }

  // create the shader program
  m_programId = glCreateProgram();
  if (!m_programId)
  {
    std::cout << "Failed to create shader program!" << std::endl;
	ASSERT(false);
    return;
  }

  // attach the vertex and fragment shaders
  glAttachShader(m_programId, vertexShaderId);
  glAttachShader(m_programId, fragmentShaderId);

  // link the shader program
  glLinkProgram(m_programId);

  // check link status
  glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
  if (!success)
  {
    char infoLog[512] = { 0 };
    glGetProgramInfoLog(m_programId, 512, NULL, infoLog);
    std::cout << "ERROR::SHADERPROGRAM::LINK_FAILED - " << std::endl << infoLog << std::endl << std::endl;
	ASSERT(false);
  }

  // delete the created shaders
  glDeleteShader(vertexShaderId);
  glDeleteShader(fragmentShaderId);

}

void Shader::Use() const
{
  glUseProgram(m_programId);
}

unsigned Shader::GetProgramId() const
{
  return m_programId;
}

const string & Shader::GetName() const
{
  return m_name;
}

bool Shader::Valid() const
{
  return m_name || !m_programId;
}

void Shader::SetBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(m_programId, name.c_str()), int(value));
}

void Shader::SetInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void Shader::SetVector2(const std::string& name, const vec2 & value) const
{
  glUniform2f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y);
}

void Shader::SetVector3(const std::string& name, const vec3 & value) const
{
  glUniform3f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVector4(const std::string& name, const vec4 & value) const
{
  glUniform4f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix3(const std::string& name, const mat3 & value) const
{
  glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &(value[0][0]));
}

void Shader::SetMatrix4(const std::string& name, const mat4 & value) const
{
  glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &(value[0][0]));
}

void Shader::SetIntArray(const std::string& name, int count, const int * data) const
{
  glUniform1iv(glGetUniformLocation(m_programId, name.c_str()), count, data);
}
