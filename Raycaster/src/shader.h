#pragma once 

#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glad/glad.h>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const char* name, int value);
	void SetUniform1f(const char* name, float value);
	void SetUniform2f(const char* name, float v1, float v2);
	void SetUniform3f(const char* name, float v1, float v2, float v3);
	void SetUniform4f(const char* name, float v1, float v2, float v3, float v4);

	void SetUniform2f(const char* name, const glm::vec2& vec);
	void SetUniform3f(const char* name, const glm::vec3& vec);
	void SetUniform4f(const char* name, const glm::vec4& vec);

	void SetMatrix4(const char* name, const glm::mat4& matrix);

private:
	GLuint CompileShader(const char* path, GLenum type);
	void CheckError(GLuint object, GLenum status);


private:
	GLuint m_Shader;
	std::unordered_map<const char*, int>  m_LocationsCache;
};