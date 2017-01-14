#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace MyEngine
{
	enum class GlyphSortType
	{
		NONE,
		FONT_TO_BACK,
		BACK_TO_FONT,
		TEXTURE
	};


	class CGlyph
	{
	public:
		CGlyph() {};
		CGlyph(const glm::vec4& destRect,
				const  glm::vec4& uvRect,
				GLuint texture,
				float depth,
				const ColorRGBA8& color);
		CGlyph(const glm::vec4& destRect,
				const  glm::vec4& uvRect,
				GLuint texture,
				float depth,
				const ColorRGBA8& color,
				float angle);
		GLuint _texture;
		float _depth;

		Vertex _topLeft;
		Vertex _bottomLeft;
		Vertex _topRight;
		Vertex _bottomRight;
	private:
		glm::vec2 RotatePoint(glm::vec2, float angle);
	};

	class CRenderBatch
	{
	public:
		CRenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : _offset(offset),
			_numVertices(numVertices),
			_texture(texture)
		{
		}

		GLuint _offset;
		GLuint _numVertices;
		GLuint _texture;
	};

	class CSpriteBatch
	{
	public:
		CSpriteBatch();
		~CSpriteBatch();

		void Init();

		void Begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void End();

		void Draw(const glm::vec4& destRect,
					const glm::vec4& uvRect,
					GLuint texture,
					float depth,
					const ColorRGBA8& color);

		void Draw(const glm::vec4& destRect,
			const glm::vec4& uvRect,
			GLuint texture,
			float depth,
			const ColorRGBA8& color,
			float angle);

		void Draw(const glm::vec4& destRect,
			const glm::vec4& uvRect,
			GLuint texture,
			float depth,
			const ColorRGBA8& color,
			glm::vec2& dir);

		void RenderBatch();
	private:
		void CreateRenderBatches();
		void CreateVertexArray();
		void SortGlyph();

		static bool CompareFontToBack(CGlyph* a, CGlyph * b);
		static bool CompareBackToFont(CGlyph* a, CGlyph * b);
		static bool CompareTexture(CGlyph* a, CGlyph * b);

		GlyphSortType _sortType;
		
		GLuint _vbo;
		GLuint _vao;
	
		std::vector<CGlyph*> _glyphsPointers;
		std::vector<CGlyph> _glyphs;
		std::vector<CRenderBatch> _renderBatches;
	};
}