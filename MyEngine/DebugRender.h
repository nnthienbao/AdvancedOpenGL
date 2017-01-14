#pragma once

#include "GLSLProgram.h"
#include "Vertex.h"

#include <glm/glm.hpp>

#include <vector>

namespace MyEngine
{

	class CDebugRender
	{
	public:
		CDebugRender();
		~CDebugRender();

		void Init();
		void End();
		void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
		void DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
		void Render(const glm::mat4& projectionMatrix, float lineWidth);
		void Dispose();

		struct DebugVertex
		{
			glm::vec2 position;
			MyEngine::ColorRGBA8 color;
		};

	private:
		MyEngine::CGLSLProgram m_program;
		std::vector<DebugVertex> m_vertexs;
		std::vector<GLuint> m_indices;
		GLuint m_vbo = 0, m_ibo = 0, m_vao = 0;
		int m_numElements = 0;
	};

}