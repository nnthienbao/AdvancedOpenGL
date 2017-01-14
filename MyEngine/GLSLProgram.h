#pragma once

#include <string>
#include <GL/glew.h>

namespace MyEngine
{
	class CGLSLProgram
	{
	public:
		CGLSLProgram();
		~CGLSLProgram();

		void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragShaderFilePath);

		void CompileShadersFromSource(const char* vertexSource, const char* fragmentSource);

		void LinkShaders();

		void AddAttribute(const std::string attibuteName);

		GLint GetUniformLocation(const std::string& uniformName);

		void Use();
		void UnUse();

		void Dispose();

	private:

		void CompileShader(const char* source, const std::string& name, GLuint id);

		int _numAttributes;

		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}