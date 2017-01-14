#include "GLSLProgram.h"

#include "MyEngineError.h"
#include "IOManager.h"
#include <fstream>
#include <vector>

namespace MyEngine
{
	CGLSLProgram::CGLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	CGLSLProgram::~CGLSLProgram()
	{
	}

	void CGLSLProgram::AddAttribute(const std::string attibuteName)
	{
		glBindAttribLocation(_programID, _numAttributes++, attibuteName.c_str());
	}

	GLint CGLSLProgram::GetUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader!");
		}
		return location;
	}

	void CGLSLProgram::Use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void CGLSLProgram::UnUse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void CGLSLProgram::Dispose()
	{
		if (_programID)
		{
			glDeleteProgram(_programID);
		}
	}

	void CGLSLProgram::CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragShaderFilePath)
	{
		std::string vertSource;
		std::string fragSource;

		CIOManager::ReadFileToBuffer(vertexShaderFilePath, vertSource);
		CIOManager::ReadFileToBuffer(fragShaderFilePath, fragSource);

		CompileShadersFromSource(vertSource.c_str(), fragSource.c_str());
	}

	void CGLSLProgram::CompileShadersFromSource(const char* vertexSource, const char* fragmentSource)
	{
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.
		//Get a program object.
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
		{
			fatalError("Vertex shader failed to be created!");
		}

		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
		{
			fatalError("Fragment shader failed to be created!");
		}

		CompileShader(vertexSource, "Vertex shader", _vertexShaderID);
		CompileShader(fragmentSource, "Fragmen shader", _fragmentShaderID);
	}

	void CGLSLProgram::CompileShader(const char* source, const std::string& name, GLuint id)
	{
		glShaderSource(id, 1, &source, nullptr);

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(id); // Don't leak the shader.

			std::printf("%s", &(errorLog[0]));
			fatalError("Shader " + name + " failed to compile!");
		}
	}

	void CGLSLProgram::LinkShaders()
	{
		//Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			std::printf("%s", &(infoLog[0]));
			fatalError("Shaders failed to compile!");
		}

		//Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}
}