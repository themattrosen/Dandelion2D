#pragma once

#include <Core\Definitions.h>
#include <Core\Math\MathCommon.h>
#include <string>

class Shader
{
public:
  Shader();
  Shader(string name);
  ~Shader() = default;

  void Use() const;
  unsigned GetProgramId() const;
  const string & GetName() const;
  bool Valid() const;

  // utility uniform functions
  void SetBool(const std::string & name, bool value) const;
  void SetInt(const std::string & name, int value) const;
  void SetFloat(const std::string & name, float value) const;
  void SetVector2(const std::string & name, const vec2 & value) const;
  void SetVector3(const std::string & name, const vec3 & value) const;
  void SetVector4(const std::string & name, const vec4 & value) const;
  void SetMatrix3(const std::string & name, const mat3 & value) const;
  void SetMatrix4(const std::string & name, const mat4 & value) const;
  void SetIntArray(const std::string & name, int count, const int * data) const;


private:
  string m_name;
  unsigned m_programId;
};
