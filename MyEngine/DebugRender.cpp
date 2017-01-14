#include "DebugRender.h"

const float PI = 3.14159265359f;

namespace
{

	const char* VERT_SRC = R"(#version 130

in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main()
{
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
	fragmentPosition = vertexPosition;
	
	fragmentColor = vertexColor;
})";

	const char* FRAG_SRC = R"(#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

void main()
{
	color = fragmentColor;
})";
}

namespace MyEngine
{

	CDebugRender::CDebugRender()
	{
	}

	CDebugRender::~CDebugRender()
	{
		Dispose();
	}

	void CDebugRender::Init()
	{
		// Shader init
		m_program.CompileShadersFromSource(VERT_SRC, FRAG_SRC);
		m_program.AddAttribute("vertexPosition");
		m_program.AddAttribute("vertexColor");
		m_program.LinkShaders();

		// Set up bufffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// TODO: Vertex attrib pointers
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void *)offsetof(DebugVertex, color));

		glBindVertexArray(0);
	}

	void CDebugRender::End()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		// Orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, m_vertexs.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexs.size() * sizeof(DebugVertex), m_vertexs.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		// Orphan the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		// Upload the data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numElements = m_indices.size();
		m_indices.clear();
		m_vertexs.clear();

	}

	glm::vec2 RotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newv;
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;
	}

	void CDebugRender::DrawBox(const glm::vec4 & destRect, const ColorRGBA8& color, float angle)
	{
		int i = m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + 4);

		glm::vec2 halfDism(destRect.z / 2.0f, destRect.w / 2.0f);

		// Get points centered at origin
		glm::vec2 tl(-halfDism.x, halfDism.y);
		glm::vec2 bl(-halfDism.x, -halfDism.y);
		glm::vec2 br(halfDism.x, -halfDism.y);
		glm::vec2 tr(halfDism.x, halfDism.y);

		glm::vec2 positionOffset(destRect.x, destRect.y);

		// Rotate the points
		m_vertexs[i].position = RotatePoint(tl, angle) + halfDism + positionOffset;
		m_vertexs[i + 1].position = RotatePoint(bl, angle) + halfDism + positionOffset;
		m_vertexs[i + 2].position = RotatePoint(br, angle) + halfDism + positionOffset;
		m_vertexs[i + 3].position = RotatePoint(tr, angle) + halfDism + positionOffset;

		for (int j = i; j < i + 4; j++)
		{
			m_vertexs[j].color = color;
		}

		m_indices.reserve(m_indices.size() + 8);

		m_indices.push_back(i);
		m_indices.push_back(i + 1);

		m_indices.push_back(i + 1);
		m_indices.push_back(i + 2);

		m_indices.push_back(i + 2);
		m_indices.push_back(i + 3);

		m_indices.push_back(i + 3);
		m_indices.push_back(i);

	}

	void CDebugRender::DrawCircle(const glm::vec2 & center, const ColorRGBA8 & color, float radius)
	{
		static const int NUM_VERTS = 100;
		int start = m_vertexs.size();
		m_vertexs.resize(m_vertexs.size() + NUM_VERTS);
		for (int i = 0; i < NUM_VERTS; i++)
		{
			float angle = ((float)i / NUM_VERTS) * PI * 2.0f;
			m_vertexs[start + i].position.x = cos(angle) * radius + center.x;
			m_vertexs[start + i].position.y = sin(angle) * radius + center.y;
			m_vertexs[start + i].color = color;
		}
		// Set up indices for indexed drawing
		m_indices.reserve(m_indices.size() + NUM_VERTS * 2);
		for (int i = 0; i < NUM_VERTS - 1; i++)
		{
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}
		m_indices.push_back(start + NUM_VERTS - 1);
		m_indices.push_back(start);
	}

	void CDebugRender::Render(const glm::mat4& projectionMatrix, float lineWidth)
	{
		m_program.Use();

		// Camera matrix
		GLint pUniform = m_program.GetUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		glLineWidth(lineWidth);
		glBindVertexArray(m_vao);

		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		m_program.UnUse();
	}

	void CDebugRender::Dispose()
	{
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
		}

		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
		}

		if (m_ibo)
		{
			glDeleteBuffers(1, &m_ibo);
		}

		m_program.Dispose();
	}
}