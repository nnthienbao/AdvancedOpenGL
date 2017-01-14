#include "SpriteBatch.h"

#include <algorithm>

namespace MyEngine
{
	CGlyph::CGlyph(const glm::vec4& destRect,
		const  glm::vec4& uvRect,
		GLuint texture,
		float depth,
		const ColorRGBA8& color) :
		_texture(texture),
		_depth(depth)
	{
		_topLeft.color = color;
		_topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
		_topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

		_bottomLeft.color = color;
		_bottomLeft.SetPosition(destRect.x, destRect.y);
		_bottomLeft.SetUV(uvRect.x, uvRect.y);

		_topRight.color = color;
		_topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		_topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_bottomRight.color = color;
		_bottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
		_bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);
	}

	CGlyph::CGlyph(const glm::vec4& destRect,
		const  glm::vec4& uvRect,
		GLuint texture,
		float depth,
		const ColorRGBA8& color,
		float angle) :
		_texture(texture),
		_depth(depth)
	{
		glm::vec2 halfDism(destRect.z / 2.0f, destRect.w / 2.0f);

		// Get points centered at origin
		glm::vec2 tl(-halfDism.x, halfDism.y);
		glm::vec2 bl(-halfDism.x, -halfDism.y);
		glm::vec2 br(halfDism.x, -halfDism.y);
		glm::vec2 tr(halfDism.x, halfDism.y);

		// Rotate the points
		tl = RotatePoint(tl, angle) + halfDism;
		bl = RotatePoint(bl, angle) + halfDism;
		br = RotatePoint(br, angle) + halfDism;
		tr = RotatePoint(tr, angle) + halfDism;

		_topLeft.color = color;
		_topLeft.SetPosition(destRect.x + tl.x, destRect.y + tl.y);
		_topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

		_bottomLeft.color = color;
		_bottomLeft.SetPosition(destRect.x + bl.x, destRect.y + bl.y);
		_bottomLeft.SetUV(uvRect.x, uvRect.y);

		_topRight.color = color;
		_topRight.SetPosition(destRect.x + tr.x, destRect.y + tr.y);
		_topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_bottomRight.color = color;
		_bottomRight.SetPosition(destRect.x + br.x, destRect.y + br.y);
		_bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);
	}

	glm::vec2 CGlyph::RotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newv;
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;
	}

	CSpriteBatch::CSpriteBatch() : _vbo(0), _vao(0)
	{
	}


	CSpriteBatch::~CSpriteBatch()
	{
	}

	void CSpriteBatch::Init()
	{
		CreateVertexArray();
	}

	void CSpriteBatch::Begin(GlyphSortType sortType)
	{
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void CSpriteBatch::End()
	{
		_glyphsPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); i++)
		{
			_glyphsPointers[i] = &_glyphs[i];
		}
		SortGlyph();
		CreateRenderBatches();
	}

	void CSpriteBatch::Draw(const glm::vec4& destRect,
							const  glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const ColorRGBA8& color)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void CSpriteBatch::Draw(const glm::vec4& destRect,
							const glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const ColorRGBA8& color,
							float angle)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void CSpriteBatch::Draw(const glm::vec4& destRect,
							const glm::vec4& uvRect,
							GLuint texture,
							float depth,
							const ColorRGBA8& color,
							glm::vec2& dir)
	{
		const glm::vec2 right(1.0f, 0.0f);

		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0.0f)
		{
			angle = -angle;
		}

		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void CSpriteBatch::RenderBatch()
	{
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i]._texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i]._offset, _renderBatches[i]._numVertices);
		}

		glBindVertexArray(0);
	}

	void CSpriteBatch::CreateRenderBatches()
	{
		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);

		if (_glyphs.empty())
		{
			return;
		}

		int offset = 0;
		int cv = 0; //current vertex
		_renderBatches.emplace_back(0, 6, _glyphsPointers[0]->_texture);

		vertices[cv++] = _glyphsPointers[0]->_topLeft;
		vertices[cv++] = _glyphsPointers[0]->_bottomLeft;
		vertices[cv++] = _glyphsPointers[0]->_bottomRight;
		vertices[cv++] = _glyphsPointers[0]->_bottomRight;
		vertices[cv++] = _glyphsPointers[0]->_topRight;
		vertices[cv++] = _glyphsPointers[0]->_topLeft;
		offset += 6;

		for (int cg = 1; cg < _glyphsPointers.size(); cg++)
		{
			if (_glyphsPointers[cg]->_texture != _glyphsPointers[cg - 1]->_texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphsPointers[cg]->_texture);
			}
			else
			{
				_renderBatches.back()._numVertices += 6;
			}

			vertices[cv++] = _glyphsPointers[cg]->_topLeft;
			vertices[cv++] = _glyphsPointers[cg]->_bottomLeft;
			vertices[cv++] = _glyphsPointers[cg]->_bottomRight;
			vertices[cv++] = _glyphsPointers[cg]->_bottomRight;
			vertices[cv++] = _glyphsPointers[cg]->_topRight;
			vertices[cv++] = _glyphsPointers[cg]->_topLeft;

			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void CSpriteBatch::CreateVertexArray()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color attribute
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// UV attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void CSpriteBatch::SortGlyph()
	{
		switch (_sortType)
		{
		case GlyphSortType::BACK_TO_FONT:
			std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), CompareBackToFont);
			break;
		case GlyphSortType::FONT_TO_BACK:
			std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), CompareFontToBack);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphsPointers.begin(), _glyphsPointers.end(), CompareTexture);
			break;
		}
	}

	bool CSpriteBatch::CompareFontToBack(CGlyph* a, CGlyph * b)
	{
		return (a->_depth < b->_depth);
	}

	bool CSpriteBatch::CompareBackToFont(CGlyph* a, CGlyph * b)
	{
		return (a->_depth > b->_depth);
	}

	bool CSpriteBatch::CompareTexture(CGlyph* a, CGlyph * b)
	{
		return (a->_texture < b->_texture);
	}
}