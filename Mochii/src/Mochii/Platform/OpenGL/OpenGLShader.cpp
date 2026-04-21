#include "Mochii/Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "mzpch.h"

namespace Mochii {
static GLenum ShaderTypeFromString(const std::string& type) {
  if (type == "vertex") return GL_VERTEX_SHADER;
  if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

  MI_CORE_ASSERT(false, "Unknown shader type!");
  return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
  MI_PROFILE_FUNCTION();

  std::string source = ReadFile(filepath);
  auto shaderSources = PreProcess(source);
  Compile(shaderSources);

  // Extract name from filepath
  auto lastSlash = filepath.find_last_of("/\\");
  lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
  auto lastDot = filepath.rfind('.');
  auto count = lastDot == std::string::npos ? filepath.size() - lastSlash
                                            : lastDot - lastSlash;
  _Name = filepath.substr(lastSlash, count);
}

OpenGLShader::OpenGLShader(const std::string& name,
                           const std::string& vertexSrc,
                           const std::string& fragmentSrc)
    : _Name(name) {
  MI_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> sources;
  sources[GL_VERTEX_SHADER] = vertexSrc;
  sources[GL_FRAGMENT_SHADER] = fragmentSrc;
  Compile(sources);
}

OpenGLShader::~OpenGLShader() {
  MI_PROFILE_FUNCTION();

  glDeleteProgram(_RendererID);
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
  MI_PROFILE_FUNCTION();

  std::string result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    if (size != -1) {
      result.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&result[0], size);
    } else {
      MI_CORE_ERROR("Could not read from file '{0}'", filepath);
    }
  } else {
    MI_CORE_ERROR("Could not open file '{0}'", filepath);
  }

  return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(
    const std::string& source) {
  MI_PROFILE_FUNCTION();

  std::unordered_map<GLenum, std::string> shaderSources;

  const char* typeToken = "#type";
  size_t typeTokenLength = strlen(typeToken);
  size_t pos = source.find(typeToken, 0);
  while (pos != std::string::npos) {
    size_t eol = source.find_first_of("\r\n", pos);
    MI_CORE_ASSERT(eol != std::string::npos, "Syntax error");
    size_t begin = pos + typeTokenLength + 1;
    std::string type = source.substr(begin, eol - begin);
    MI_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

    size_t nextLinePos = source.find_first_not_of("\r\n", eol);
    MI_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
    pos = source.find(typeToken, nextLinePos);

    shaderSources[ShaderTypeFromString(type)] =
        (pos == std::string::npos)
            ? source.substr(nextLinePos)
            : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

void OpenGLShader::Compile(
    const std::unordered_map<GLenum, std::string>& shaderSources) {
  MI_PROFILE_FUNCTION();

  GLuint program = glCreateProgram();
  MI_CORE_ASSERT(shaderSources.size() <= 2,
                 "We only support 2 shaders for now");
  std::array<GLenum, 2> glShaderIDs;
  int glShaderIDIndex = 0;
  for (auto& kv : shaderSources) {
    GLenum type = kv.first;
    const std::string& source = kv.second;

    GLuint shader = glCreateShader(type);

    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

      glDeleteShader(shader);

      MI_CORE_ERROR("Shader compilation failed:\n{0}", infoLog.data());
      MI_CORE_ASSERT(false, "Shader compilation failure!");
      return;
    }

    glAttachShader(program, shader);
    glShaderIDs[glShaderIDIndex++] = shader;
  }

  _RendererID = program;

  // Link our program
  glLinkProgram(program);

  // Note the different functions here: glGetProgram* instead of glGetShader*.
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
  if (isLinked == GL_FALSE) {
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    MI_CORE_ERROR("Shader link failed: {}", infoLog.data());
#if defined(MI_DEBUG)
    MI_CORE_ASSERT(false, "Shader link failure!");
#endif

    glDeleteProgram(program);
    for (auto id : glShaderIDs) glDeleteShader(id);
    return;
  }

  for (auto id : glShaderIDs) {
    glDetachShader(program, id);
    glDeleteShader(id);
  }
}

void OpenGLShader::Bind() const {
  MI_PROFILE_FUNCTION();

  glUseProgram(_RendererID);
}

void OpenGLShader::Unbind() const {
  MI_PROFILE_FUNCTION();

  glUseProgram(0);
}

void OpenGLShader::SetInt(const std::string& name, int value) {
  MI_PROFILE_FUNCTION();

  UploadUniformInt(name, value);
}

void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count) {
  MI_PROFILE_FUNCTION();

  UploadUniformIntArray(name, values, count);
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
  MI_PROFILE_FUNCTION();

  UploadUniformFloat(name, value);
}

void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
  MI_PROFILE_FUNCTION();

  UploadUniformFloat3(name, value);
}

void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
  MI_PROFILE_FUNCTION();

  UploadUniformFloat4(name, value);
}

void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
  MI_PROFILE_FUNCTION();

  UploadUniformMat4(name, value);
}

void OpenGLShader::UploadUniformInt(const std::string& name, int value) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniform1i(location, value);
}

void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniform1iv(location, count, values);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, float value) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniform1f(location, value);
}

void OpenGLShader::UploadUniformFloat2(const std::string& name,
                                       const glm::vec2& value) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniformFloat3(const std::string& name,
                                       const glm::vec3& value) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniformFloat4(const std::string& name,
                                       const glm::vec4& value) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniformMat3(const std::string& name,
                                     const glm::mat3& matrix) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name,
                                     const glm::mat4& matrix) {
  GLint location = glGetUniformLocation(_RendererID, name.c_str());
  if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
}  // namespace Mochii
